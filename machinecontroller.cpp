#include "machinecontroller.h"
#include <QDebug>
#include <QThread>
#include <can/servocontroller.h>
#include "IO/iobase.h"

MachineController::MachineController(MachineConfig *config, IOBase *io, ControllerBase *servo, QObject *parent):
    QObject(parent),
    _io(io),
    _servo(servo),
    _config(config)
{
    _signalConnections.push(connect(_io, &IOBase::SwiperMoveDone, this, &MachineController::onSwiperMoveDone));
    _signalConnections.push(connect(_io, &IOBase::SwiperFault, this, &MachineController::onSwiperFault));
    _signalConnections.push(connect(_io, &IOBase::VacuumFault, this, &MachineController::onVacuumFault));
    _signalConnections.push(connect(_io, &IOBase::CommunicationFault, this, &MachineController::onIOCommFault));
    _signalConnections.push(connect(_io, &IOBase::PedalToggled, this, &MachineController::PedalToggled));
    _signalConnections.push(connect(_io, &IOBase::AnalogReady, this, &MachineController::onAnalogReady));
    _signalConnections.push(connect(_servo, &ControllerBase::MoveDone, this, &MachineController::onServoMoveDone));
    _signalConnections.push(connect(_servo, &ControllerBase::Fault, this, &MachineController::onServoFault));
}

MachineController::~MachineController()
{
    if(_signalConnections.count()>0)
    {
        while(!_signalConnections.isEmpty())
        {
            disconnect(_signalConnections.pop());
        }
    }
}

void MachineController::EnableOperation()
{
   // ((ServoController*)_servo)->Connect();
    QBitArray outputs(16, false);
    outputs[IOBase::Outputs::SWIPER_ON] = false;
    outputs[IOBase::Outputs::VACUUM_ON] = false;

    _io->setOutputs(outputs);
   // QThread::sleep(2);
    _servo->EnableOperation();

    //_servo->setMoveType(MoveType::Additive);
    _homingFlag = true;
}

void MachineController::DisableOperation()
{
    QBitArray outputs(16, false);
    outputs[IOBase::Outputs::SWIPER_ON] = false;
    outputs[IOBase::Outputs::FC_POWER] = false;
    outputs[IOBase::Outputs::FC_ENABLE] = false;
    outputs[IOBase::Outputs::VACUUM_ON] = false;

    _io->setOutputs(outputs);
    _servo->DisableOperation();
}

void MachineController::Swipe(int revolutions)
{
    emit _io->SwiperMove(revolutions);
}

void MachineController::ForwardMove(double mass)
{
    int ticsToMove = _config->MassToTicks(mass);
    _servo->Move(ticsToMove, MoveDirection::Forward);
}

void MachineController::StartMoveVelocity(int speed)
{
    if(_servo!=nullptr)
    {
        _servo->StartMoveVelocity(speed);
    }
}

void MachineController::StopMoveVelocity()
{
    if(_servo!=nullptr)
    {
        _servo->StopMoveVelocity();
    }
}

void MachineController::ReverseMove(double mass)
{
    int ticsToMove = _config->MassToTicks(mass);
    _servo->Move(ticsToMove, MoveDirection::Reverse);
}

bool MachineController::Vacuum()
{
    return _io->Vacuum();
}

void MachineController::setVacuum(bool state)
{
    QBitArray outs = _io->getOutputs();
    outs[IOBase::Outputs::VACUUM_ON] = state;
    _io->setOutputs(outs);
}

void MachineController::setVacuumValue(double value)
{
    _io->setVacuum(value);
}

void MachineController::setSpeed(int speed)
{
    _servo->setSpeed(speed);
}

void MachineController::onSwiperMoveDone()
{
    emit SwiperMoveDone();
}

void MachineController::onServoMoveDone()
{
    emit ServoMoveDone();
}

void MachineController::onSwiperFault()
{

}

void MachineController::onVacuumFault()
{

}

void MachineController::onIOCommFault()
{

}

void MachineController::onServoFault()
{
    _io->FCEnable(false);
    _io->VacuumOn(false);
    _io->FCPowerOn(false);
    emit Fault("Servo Fault");
}

void MachineController::onAnalogReady(double value)
{
    Q_UNUSED(value)
    emit AnalogDataReady(value);
}
