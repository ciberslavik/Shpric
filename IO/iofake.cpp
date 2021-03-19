#include "iofake.h"

IOFake::IOFake()
{

}


void IOFake::Init(int cycleTime)
{
    _cycleTime = cycleTime;
    connect(&_cycleTimer, &QTimer::timeout,this,&IOFake::cyclicTimeout);
    connect(&_swiperTimer, &QTimer::timeout,this,&IOFake::swiperTimeout);
}

void IOFake::Start()
{
    _isRun=true;
}

void IOFake::Stop()
{
    _isRun=false;
}

void IOFake::SwiperMove(int moveTime)
{
    qDebug()<<"Swiper move:"<<moveTime;
    QTimer::singleShot(moveTime, this, &IOBase::SwiperMoveDone);
}

void IOFake::VacuumOn(bool state)
{
    Q_UNUSED(state)
}

void IOFake::FCPowerOn(bool state)
{
    Q_UNUSED(state)
}

void IOFake::FCEnable(bool state)
{
    Q_UNUSED(state)
}


void IOFake::cyclicTimeout()
{

}

void IOFake::swiperTimeout()
{
    emit SwiperMoveDone();
}


QBitArray IOFake::getOutputs()
{
    return QBitArray(16);
}

void IOFake::setOutputs(const QBitArray &outputs)
{
    Q_UNUSED(outputs)
}



bool IOFake::Vacuum()
{
    return false;
}


void IOFake::setVacuum(double value)
{
    Q_UNUSED(value)
}


void IOFake::SwiperOn(bool state)
{
    Q_UNUSED(state)
}
