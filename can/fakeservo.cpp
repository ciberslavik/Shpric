#include "fakeservo.h"

#include <IO/iobase.h>

FakeServo::FakeServo(IOBase *io, QObject *parent):
    ControllerBase(parent),_io(io)

{

    ControllerBase::setInstance(this);
}


void FakeServo::EnableOperation()
{
    QBitArray outs = _io->getOutputs();
    outs[IOBase::Outputs::FC_POWER] = true;
    outs[IOBase::Outputs::FC_ENABLE] = true;
    _io->setOutputs(outs);
    qDebug()<<"Enable operation";
}

void FakeServo::DisableOperation()
{
    qDebug()<<"Disable operation";
}

void FakeServo::Move(int target, MoveDirection dir)
{


    QString str;
    if(dir == MoveDirection::Forward)
        str = "F";
    else if(dir == MoveDirection::Reverse)
        str = "R";

    QTimer::singleShot((target/10)/5, this, &ControllerBase::MoveDone);
    qDebug()<<"MoveTo:"<<target<<str<<" Time:"<<(target/10)/5;
}

void FakeServo::Homing()
{
    qDebug()<<"Homing:";
}

void FakeServo::setSpeed(int speed)
{
    qDebug()<<"Set speed:"<<speed;
}



void FakeServo::Init()
{
    _isInit=true;
}

bool FakeServo::isInit()
{
    return _isInit;
}


void FakeServo::StartMoveVelocity(int velocity)
{
    qDebug() << "Move velocity:" << velocity;
}

void FakeServo::StopMoveVelocity()
{
    qDebug() << "Stop move velocity";
}
