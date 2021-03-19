#include "machine.h"

#include <logic/StuffingLogic.h>
#include <logic/SwiperLogic.h>

#include <QBitArray>
#include <QMessageBox>

Machine::Machine(QObject *parent) : QObject(parent),
    _enabled(false),
    _is_init(false),
    _isCyclic(false),
    _controller(nullptr),
    _currentLogic(nullptr),
    _currentMode(LogicMode::Stop)
{

}

void Machine::InitMachine(MachineController *controller)
{
    _controller = controller;
    _currentMode = LogicMode::Stop;
    _is_init=true;
    connect(_controller, &MachineController::PedalToggled, this, &Machine::onPedalToggled);
    connect(_controller, &MachineController::Fault,this, &Machine::onServoFault);
    connect(_controller, &MachineController::AnalogDataReady, this, &Machine::onAnalogDataReady);
}


void Machine::EnableOperation()
{
    if(_is_init)
    {
        if(!_enabled)
        {
            _controller->EnableOperation();
            _enabled =true;
        }
    }
}

void Machine::DisableOperation()
{
    if(_is_init && _enabled)
    {
        _controller->DisableOperation();
        _enabled = false;
    }
}

void Machine::LoadLogicPresset(PressetConfig *presset)
{
    if(_currentLogic != nullptr)
    {
        //if(!_currentLogic->Presset()->isEqual(presset))
        {
            if(_currentLogic->Logic() != (Logic_t)presset->Logic())
            {
                deleteCurrentLogic();
                _currentLogic = createLogic(presset);
                if(_currentLogic!=nullptr)
                    _currentLogic->LoadPresset(presset);
            }
            else
            {
                _currentLogic->LoadPresset(presset);
            }
        }
    }
    else
    {
        _currentLogic = createLogic(presset);
        if(_currentLogic!=nullptr)
            _currentLogic->LoadPresset(presset);
    }
    if(_currentLogic!=nullptr)
        _controller->setSpeed(presset->Temp());
}

void Machine::setVacuum(double value)
{
    _controller->setVacuumValue(value);
}


void Machine::setLogicMode(LogicMode_t logicMode)
{
    if(_currentMode != logicMode)
    {
        _currentMode = logicMode;
        if(_currentMode == LogicMode::Stop)
        {
            _controller->setVacuum(false);
            _controller->setVacuumValue(100);
            _currentLogic->Reset();
        }
        else
        {
            _controller->setVacuum(true);
            //_controller->setVacuumValue(0);
        }

    }
    _currentLogic->setLogicMode(logicMode);
}

void Machine::onIOFault()
{

}

void Machine::onServoFault(QString message)
{
    QMessageBox mb;
    mb.setText(message);
    mb.show();
}

void Machine::onPedalToggled(bool state)
{
    if(_currentLogic!=nullptr)
    {
        _currentLogic->PedalToggled(state);
    }
}

void Machine::onLogicCycleDone()
{
    emit CycleDone();

}

void Machine::onAnalogDataReady(double value)
{
    emit AnalogDataReady(value);
}

void Machine::deleteCurrentLogic()
{
    if(_currentLogic!=nullptr)
    {
        _currentLogic->CycleStop();
        while(!_logicSignals.isEmpty())
        {
            disconnect(_logicSignals.pop());
        }
        delete _currentLogic;
        _currentLogic = nullptr;
    }
}

AbstractLogic *Machine::createLogic(PressetConfig *config)
{
    AbstractLogic *logic = nullptr;
    switch ((Logic)config->Logic())
    {
    case Logic::Swiper:
        logic = createSwiperLogic();
        break;
    case Logic::Portion:
        logic = createPortionLogic();
        break;
    case Logic::Stuffing:
        logic = createStuffingLogic();
        break;
       case Logic::None:
        logic = nullptr;
        break;
    }
    if(logic!=nullptr)
        logic->LoadPresset(config);
    return logic;
}

AbstractLogic *Machine::createSwiperLogic()
{
    SwiperLogic *swiper = new SwiperLogic(_controller);
    _logicSignals.push(connect(swiper,&AbstractLogic::CycleDone, this, &Machine::onLogicCycleDone));
    _logicSignals.push(connect(_controller, &MachineController::ServoMoveDone, swiper, &SwiperLogic::ServoMoveDone));
    _logicSignals.push(connect(_controller, &MachineController::SwiperMoveDone, swiper, &SwiperLogic::SwiperMoveDone));
    return swiper;
}

AbstractLogic *Machine::createPortionLogic()
{
    PortionLogic *portion = new PortionLogic(_controller);
    _logicSignals.push(connect(portion,&AbstractLogic::CycleDone, this, &Machine::onLogicCycleDone));
    _logicSignals.push(connect(_controller, &MachineController::ServoMoveDone, portion, &AbstractLogic::ServoMoveDone));
    _logicSignals.push(connect(_controller, &MachineController::SwiperMoveDone, portion, &AbstractLogic::SwiperMoveDone));
    return portion;
}

AbstractLogic *Machine::createStuffingLogic()
{
    StuffingLogic *portion = new StuffingLogic(_controller);
    _logicSignals.push(connect(portion,&AbstractLogic::CycleDone, this, &Machine::onLogicCycleDone));
    _logicSignals.push(connect(_controller, &MachineController::ServoMoveDone, portion, &AbstractLogic::ServoMoveDone));
    _logicSignals.push(connect(_controller, &MachineController::SwiperMoveDone, portion, &AbstractLogic::SwiperMoveDone));
    return portion;
}

MachineConfig *Machine::getDefaultConfig()
{
    MachineConfig *config = new MachineConfig();
    config->setEncoderTicks(1024);
    config->setGearboxRatio(16);
    config->setMassPerPumpRev(300.0);

    return config;
}
