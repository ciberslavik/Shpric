#include "iomodbusworker.h"
//#include <QCoreApplication>
#include <QQueue>

IOModbusWorker::IOModbusWorker(QObject *parent) : QThread(parent),
    _discreteOutputs(16),
    _discreteInputs(16),
    _requestBuf(nullptr)
{
    moveToThread(this);
    _is_proccessingRequest = false;
    _requestBuf = new QQueue<abstract_request_t*>();
    _pulseEnabled = false;
}

void IOModbusWorker::Init(WorkerConfig *config)
{
    _workerConf = config;
    //Configure PID
    _pid=new PIRegulator(10, 2, 3);
}

QBitArray &IOModbusWorker::getDigitalOutputs()
{
    return _discreteOutputs;
}

abstract_request_t *IOModbusWorker::getWriteRegRequest16(quint16 addr, quint16 value)
{
    write_request_t *request = new write_request_t();
    request->id = _workerConf->slaveId;
    request->func = MB_FUNC_WRITE_HOLDING_REGISTER;
    request->address = addr;
    request->count = 1;
    request->data[0] = value;
    return request;
}

abstract_request_t *IOModbusWorker::getReadInputRegRequest(quint16 addr, quint16 count)
{
    read_request_t *request = new read_request_t();
    request->id = _workerConf->slaveId;
    request->func = MB_FUNC_READ_INPUT_REGISTERS;
    request->address = addr;
    request->count = count;
    return request;
}

unsigned short IOModbusWorker::intFromBits(QBitArray *bits)
{
    unsigned short ret = 0;
    unsigned short bitsCount = 0;
    if(bits->count()>=16)
        bitsCount = 16;
    else
        bitsCount = bits->count();

    for (int i = 0; i < bitsCount; i++) {
        if (bits->testBit(i))
            ret |= (1 << i);
    }
    return ret;
}
QBitArray IOModbusWorker::bitsFromByte(unsigned short &data)
{
    QBitArray bits(16);
    qint16 mask = 0x0001;
    for(int i = 0;i<16;i++)
    {
        bits[i]=(data & mask ? true : false);
        mask = mask << 1;
    }
    return bits;
}


void IOModbusWorker::run()
{
    _master = new Master();
    _master->moveToThread(this);
    _master->init(_workerConf->serialConfig);
    connect(_master, &Master::sendAnswer, this, &IOModbusWorker::slaveAnswer);
    connect(_master, &Master::sendRawData, this, &IOModbusWorker::rawDataReceive);
    connect(_master, &Master::onError, this, &IOModbusWorker::modbusError);
    connect(_master, &Master::timeoutError, this, &IOModbusWorker::onTimeoutError);
    if(_master->openConnection())
    {
        _is_connected = true;
        _cycleTimer = new QTimer();

        connect(_cycleTimer, &QTimer::timeout, this, &IOModbusWorker::Process);
        _cycleTimer->setInterval(_workerConf->cycleTime);
        _cycleTimer->moveToThread(this);
        _cycleCounter = 0;
        _is_first_cycle = true;
        _cycleTimer->start();
        _is_running = true;
    }
    else
        _is_connected = false;

    _vacuum_pwm=0;
    _vacuum_value=0;
    _vacuum_setpoint=0;
    exec();
}

void IOModbusWorker::writeOutput(QBitArray values)
{
    if(_discreteOutputs != values)
    {
        _discreteOutputs = values;
        abstract_request_t *request = getWriteRegRequest16(REG_DIGITAL_OUTPUT, intFromBits(&_discreteOutputs));

        EnqueueRequest(request);
    }
}

void IOModbusWorker::writePWM(double value)
{
    _vacuum_setpoint = value;
    _pid->reset_itegrator();
}

void IOModbusWorker::writePulse(quint16 value)
{
    abstract_request_t *request;
    request = getWriteRegRequest16(REG_PULSE_WIDTH, value); //Pulse count
    EnqueueRequest(request);
    ProcessQueue();

}

void IOModbusWorker::pulseOn(bool state)
{
    abstract_request_t *request;
    quint16 value;
    if(state)
        value = 0x0001;
    else
        value = 0x0000;

    request = getWriteRegRequest16(REG_PULSE_ON, value); //Pulse count
    EnqueueRequest(request);
    ProcessQueue();
}

void IOModbusWorker::slaveAnswer(answer_request_t answer)
{
    if(answer.func == MB_FUNC_READ_INPUT_REGISTERS)
    {
        if(answer.address == REG_DIGITAL_INPUT)
        {
            QBitArray inputs = bitsFromByte(answer.data[0]);
            _analog = 1024 - answer.data[1];
            emit InputDataReady(inputs,_analog);
        }
    }
    if(answer.func == MB_FUNC_WRITE_HOLDING_REGISTER)
    {
        if(answer.address == REG_PULSE_WIDTH)
        {
            qDebug()<<"Swiper writed" << answer.data[0];
            QTimer::singleShot(answer.data[0]+5, this, &IOModbusWorker::onSwiperTimeout);
        }
    }
    if(answer.func == MB_FUNC_READ_HOLDING_REGISTERS)
    {
        if(answer.address == REG_DIGITAL_OUTPUT)
        {

        }

    }
    ProcessQueue();
}

void IOModbusWorker::rawDataReceive(QByteArray rawData)
{
    QString buff = "Received data: ";
    quint8 tmp = 0;

    // Convert raw data to hexadicemal form
    for (int i = 0; i < rawData.count(); i++)
    {
        tmp = static_cast<quint8>(rawData.at(i));
        buff += QString("%1 ").arg(tmp, 2, 16, QLatin1Char('0'));
    }
    qDebug()<<"Raw data:"<<buff;
    emit statusPrint("Raw data:" + buff);
}

void IOModbusWorker::modbusError()
{

    qDebug()<<"Modbus error:";
}

void IOModbusWorker::onTimeoutError()
{

}

void IOModbusWorker::onSwiperTimeout()
{
   emit SwiperMoveDone();

}

void IOModbusWorker::ProcessQueue()
{
    if(!_requestBuf->isEmpty())
    {
        abstract_request_t *request = _requestBuf->dequeue();
        //Send request
        _master->sendRequest(request);
        //Deleting request after processing  !!!!!!!!!!!!!!!!!!!!!!!!!
        if(request!=nullptr)
            delete request;
    }
}

void IOModbusWorker::EnqueueRequest(abstract_request_t *request)
{
        _requestBuf->enqueue(request);
}

void IOModbusWorker::stopWorker()
{
    if(_master)
    {
        if(_master->isConnected())
        {
            _master->closeConnection();
        }

        delete _master;
        _master = nullptr;
    }
    if(_cycleTimer->isActive())
    {
        _cycleTimer->stop();
        delete _cycleTimer;
        _cycleTimer = nullptr;
    }

}


void IOModbusWorker::Process()
{
    if(_is_first_cycle)
    {
        DevState state;
        state.ExtDisable = false;
        state.TimeoutDisable = false;
        _is_first_cycle = false;
        EnqueueRequest(getWriteRegRequest16(REG_PWM_EN, 0x0001));
        return;
    }
    if(_cycleCounter % 5 == 0)
    {
        _vacuum_value = ((static_cast<double>(_analog)/10)-3);

        _vacuum_pwm += (_pid->calculate(_vacuum_setpoint, _vacuum_value)*-1);
        if(_vacuum_pwm>=1023)
        {
            _vacuum_pwm = 1023;
            _pid->reset_itegrator();
        }
        else if(_vacuum_pwm<=0)
        {
            _vacuum_pwm = 0;
            _pid->reset_itegrator();
        }
        //_vacuum_pwm = 1024 - _vacuum_pwm;
        //qDebug()<<"Vacuum PWM:"<<_vacuum_pwm;
        EnqueueRequest(getWriteRegRequest16(REG_PWM_VAL, (quint16)_vacuum_pwm));
    }
    else
        EnqueueRequest(getReadInputRegRequest(REG_DIGITAL_INPUT,2));

    ProcessQueue();
    _cycleCounter++;

}
