//#include "machineio.h"
//#include <QDebug>

//MachineIO::MachineIO(Master *master, QObject *parent) : QObject(parent),_master(master)
//{

//}

//void MachineIO::Init(int cycleTime)
//{
//    if(_master==nullptr)
//    {
//        qDebug()<<"Master is null";
//        return;
//    }
//    _cycle_time = cycleTime;
//    connect(_master, &Master::sendAnswer, this, &MachineIO::onSlaveAnswer);
//    connect(_master, &Master::sendRawData, this, &MachineIO::onRawDataReceive);
//    connect(_master, &Master::onError, this, &MachineIO::onFault);
//    connect(this, &MachineIO::sendRequest, _master, &Master::sendRequest);
//    connect(&_swiperTimer, &QTimer::timeout, this, &MachineIO::swiperPulseTimeout);
//    qRegisterMetaType<answer_request_t>();
//    _slaveId = 1;

//    _master->init();

//    _master->openConnection();

//    _is_thread_started = true;

//}

//void MachineIO::DeInit()
//{
//    _master->closeConnection();
//    delete _master;
//    _master = nullptr;
//}

//bool MachineIO::isConnected()
//{
//    return _master->isConnected();
//}

//void MachineIO::swiperPulse(int msec)
//{
//    _swiperTimer.setInterval(msec);
//    _discreteOutputs[0] = true;
//    _prev_state = _current_state;
//    _current_state = CycleState::WriteDigitalOutputs;
//    _swiperTimer.setSingleShot(true);
//    _swiperTimer.start();
//}

//void MachineIO::swiperPulseTimeout()
//{
//    _discreteOutputs[0] = false;
//    _prev_state = _current_state;
//    _current_state = CycleState::WriteDigitalOutputs;
//}

//void MachineIO::setDigitalOutput(QBitArray &bits)
//{
//    _discreteOutputs = bits;
//    _prev_state = _current_state;
//    _current_state = CycleState::WriteDigitalOutputs;
//}

//abstract_request_t *MachineIO::getDigitalReadRequest()
//{

//}

//abstract_request_t *MachineIO::getAnalogReadRequest()
//{

//}
//abstract_request_t *MachineIO::getDigitalWriteRequest()
//{

//}

//void MachineIO::parseDiscreteInput(QBitArray inputs)
//{
//    if(inputs[0])
//        emit onPedalToggle(inputs[0]);
//    if(inputs[1])
//        emit onVacuumToggle(inputs[1]);
//    if(inputs[2])
//        emit onSwiperToggle(inputs[2]);
//}
//void MachineIO::onSlaveAnswer(answer_request_t answer)
//{
//    if (getRequestType(answer.func) == REQ_READ)
//    {
//        if(answer.func==MB_FUNC_READ_INPUT_REGISTERS)
//        {
//            if(answer.address==REG_DIGITAL_INPUT)
//            {
//                u_int16_t inputData = answer.data[0];
//                if(inputData != _prevDiscreteInputs)
//                {
//                    _prevDiscreteInputs = inputData;
//                    _discreteInputs = bitsFromByte(inputData,16);
//                    parseDiscreteInput(_discreteInputs);
//                }

//            }
//            else if(answer.address==REG_ANALOG_INPUT)
//            {

//            }
//        }
//    }
//    else if(getRequestType(answer.func) == REQ_WRITE)
//    {
//        if(answer.func==MB_FUNC_WRITE_HOLDING_REGISTER)
//        {
//            emit statusPrint("MachineIO: Request write answer");
//        }
//    }
//}

//void MachineIO::onRawDataReceive(QByteArray rawData)
//{
//    QString buff = "Received data: ";
//    quint8 tmp = 0;

//    // Convert raw data to hexadicemal form
//    for (int i = 0; i < rawData.count(); i++)
//    {
//        tmp = static_cast<quint8>(rawData.at(i));
//        buff += QString("%1 ").arg(tmp, 2, 16, QLatin1Char('0'));
//    }
//    emit rawDataReceived(buff);
//}

//void MachineIO::Process()
//{
//    int cycle_counter = 0;
//    _current_state = CycleState::ReadDigitalInputs;
//    do
//    {
//        QThread::msleep(_cycle_time);
//        ///system("echo -ne 'Hello' > /dev/ttyUSB1");
//        switch(_current_state)
//        {
//        case CycleState::ReadDigitalInputs:
//            emit sendRequest(getDigitalReadRequest());
//            if(cycle_counter>=10)
//            {
//                cycle_counter = 0;
//                _current_state = CycleState::ReadAnalogInputs;
//            }
//            break;
//        case CycleState::ReadAnalogInputs:
//            //emit sendRequest(getAnalogReadRequest());
//            cycle_counter = 0;
//            _current_state = CycleState::ReadDigitalInputs;
//            break;
//        case CycleState::WriteDigitalOutputs:
//            //emit sendRequest(getDigitalWriteRequest());
//            //_current_state = _prev_state;
//            break;
//        }
//        cycle_counter++;

//        emit isStarted(&_is_thread_started);
//    }while(_is_thread_started);

//    emit quit();
//}

//void MachineIO::statusPrint(QString msg)
//{
//    qDebug() << "MachineIO: " <<msg;
//}

//void MachineIO::masterError()
//{
//    qDebug()<<"MachineIO: masterError";
//}
//QBitArray MachineIO::bitsFromByte(u_int16_t &data, int size)
//{
//    QBitArray bits(size);
//    qint16 mask = 0x0001;
//    for(int i = 0;i<size;i++)
//    {
//        bits[i]=(data & mask ? true : false);
//        mask = mask << 1;
//    }
//    return bits;
//}

//u_int16_t MachineIO::intFromBits(const QBitArray &data)
//{
//    u_int16_t ret = 0;
//    u_int16_t bitsCount = 0;
//    if(data.count()>=16)
//        bitsCount = 16;
//    else
//        bitsCount = data.count();

//    for (int i = 0; i < bitsCount; i++) {
//        if (data[i])
//            ret |= (1 << i);
//    }
//    return ret;
//}
