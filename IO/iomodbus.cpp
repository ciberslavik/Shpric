#include "iomodbus.h"

IOModbus::IOModbus(SerialConfig *serialConfig, QObject *parent) : IOBase(parent),
    _workerConfig(2),
    _discreteInputs(16)
{
    _workerConfig.serialConfig = serialConfig;
    IOBase::setInstance((IOBase*)this);
}

void IOModbus::Init(int cycleTime)
{
    _workerConfig.cycleTime = cycleTime;

    _worker = new IOModbusWorker();
    _worker->Init(&_workerConfig);

    connect(this,&IOModbus::writeOutput, _worker, &IOModbusWorker::writeOutput);
    connect(this,&IOModbus::writePWM, _worker,&IOModbusWorker::writePWM);
    connect(this,&IOModbus::writePulse, _worker,&IOModbusWorker::writePulse);
    connect(this,&IOModbus::writePulseOn, _worker, &IOModbusWorker::pulseOn);
    connect(_worker, &IOModbusWorker::SwiperMoveDone, this, &IOBase::SwiperMoveDone);
    connect(_worker, &IOModbusWorker::InputDataReady, this, &IOModbus::onInputDataReady);
    connect(this, &IOModbus::stopWorker, _worker, &IOModbusWorker::stopWorker);
}

void IOModbus::Start()
{
    _worker->start(QThread::Priority::TimeCriticalPriority);
    _isRun = true;
}

void IOModbus::Stop()
{
    if(_worker!=nullptr)
    {
        if(_worker->isRunning())
        {
            emit stopWorker();
            _worker->quit();
            _worker->wait(1000);
            _isRun = false;
        }
        delete _worker;
        _worker = nullptr;
    }
}

bool IOModbus::Vacuum()
{
    QBitArray outputs = getOutputs();
    return outputs[VACUUM_ON];
}

void IOModbus::SwiperMove(int moveTime)
{
    qDebug() << "Swiper move:" << moveTime;

    emit writePulse(moveTime);
}


void IOModbus::onReciveStatus(DevState state)
{
    qDebug("Dev state: external lock %s", state.ExtDisable ? "true":"false");
}


void IOModbus::onInputDataReady(QBitArray inputs, quint16 analog)
{

    if(inputs[PEDAL_SW] != _discreteInputs[PEDAL_SW])
    {
        _pedalState = inputs[PEDAL_SW];
        emit PedalToggled(!_pedalState);
    }
    if(inputs[FC_FAULT] != _discreteInputs[FC_FAULT])
    {
        if(inputs[FC_FAULT])
        {
            _fcFault = true;
            emit FCFault();
        }
    }
    if(inputs[VACUUM_ASW] != _discreteInputs[VACUUM_ASW])
    {
        if(inputs[VACUUM_ASW])
        {
            _vacuumFault = true;
            emit VacuumFault();
        }
    }
    if(inputs[SWIPER_ASW] != _discreteInputs[SWIPER_ASW])
    {
        if(inputs[SWIPER_ASW])
        {
            _vacuumFault = true;
            emit SwiperFault();
        }
    }

    _discreteInputs = inputs;

    if(_ainCnt%10==0)
    {

        _aIN = (static_cast<double>(analog)/10)-3;
        emit AnalogReady(_aIN);
        _ainCnt = 0;
    }
    _ainCnt++;
}

void IOModbus::setOutput(int output, bool value)
{
    QBitArray outputs = _worker->getDigitalOutputs();
    outputs[output] = value;
    emit writeOutput(outputs);
}


void IOModbus::VacuumOn(bool state)
{
    setOutput(VACUUM_ON, state);
}

void IOModbus::FCPowerOn(bool state)
{
    setOutput(FC_POWER, state);
}

void IOModbus::FCEnable(bool state)
{
    setOutput(FC_ENABLE, state);
}


QBitArray IOModbus::getOutputs()
{
    return _worker->getDigitalOutputs();
}

void IOModbus::setOutputs(const QBitArray &outputs)
{
    emit writeOutput(outputs);
}


void IOModbus::setVacuum(double value)
{
    emit writePWM(value);
}

void IOModbus::SwiperOn(bool state)
{
    emit writePulseOn(state);
}
