#include "PortionLogic.h"
#include <QDebug>

PortionLogic::PortionLogic(MachineController *controller):AbstractLogic(controller)
{
    qDebug()<<"Portion logic created!";
    connect(&_pauseTimer, &QTimer::timeout, this, &PortionLogic::pauseTimeout);
    _isFirsPortion = true;
    _isCycleRun = false;
}

PortionLogic::~PortionLogic()
{
    qDebug()<<"Portion logic deleted";
}

void PortionLogic::CycleStop()
{
    _isCycleRun = false;
}

void PortionLogic::Reset()
{
    _isFirsPortion = true;
}


//void PortionLogic::SingleStart()
//{
//}

//void PortionLogic::CycleStart()
//{
//}

//void PortionLogic::CycleStop()
//{
//}

//void PortionLogic::CyclePause()
//{
//}

LogicInfo PortionLogic::getLogicInfo()
{
    LogicInfo info;
    info.logicName = "Portion";
    return info;
}

Logic_t PortionLogic::Logic()
{
    return Logic::Portion;
}

void PortionLogic::ServoMoveDone()
{
    qDebug()<<"Servo MoveDone";
    LogicCycle();
}

void PortionLogic::SwiperMoveDone()
{
    qDebug()<<"Swiper MoveDone";
    LogicCycle();
}

void PortionLogic::PedalToggled(bool state)
{
    if(LogicMode()==LogicMode::Stop)
        return;
    //Rising edge
    if((state == true) && (_prevPedalState == false))
    {
        qDebug() << "Pedal Rising edge";
        if(_isFirsPortion)
        {
            _currentStepp = LogicStepp::FirstPortion;
            _isFirsPortion = false;
            LogicCycle();
        }
    }
    //Faling edge
    if((state == false) && (_prevPedalState == true))
    {
        qDebug() << "Pedal Faling edge";
        if(!_isFirsPortion)
        {
            _currentStepp = LogicStepp::Pause;
            _nextStepp = LogicStepp::Forward;
            LogicCycle();
        }
    }
    _prevPedalState = state;
}

bool PortionLogic::CheckLogicPresset(PressetConfig *presset)
{
    if(presset->Logic() == static_cast<int>(Logic::Portion))
        return true;
    else
        return false;
}

bool PortionLogic::setPressetConfig()
{
    return true;
}

void PortionLogic::pauseTimeout()
{
    _pauseTimer.stop();
    _currentStepp = _nextStepp;
    LogicCycle();
}

void PortionLogic::DoFirstPortion()
{
    qDebug() << "First portion";
    _controller->ForwardMove(_presset->FirstPortion());
}

void PortionLogic::DoReverse()
{
    qDebug()<<"Reverse move:";
    _controller->ReverseMove(_presset->Reverse());
}

void PortionLogic::DoPause()
{
    qDebug()<<"Pause: " << _presset->Pause();
    _pauseTimer.setInterval(_presset->Pause());
    _pauseTimer.setSingleShot(true);
    _pauseTimer.start();
}

void PortionLogic::DoForward()
{
    qDebug()<<"Forward move";
    _controller->ForwardMove(_presset->Mass());
}

void PortionLogic::LogicCycle()
{
    switch (_currentStepp) {
    case FirstPortion:
        DoFirstPortion();
        _currentStepp = LogicStepp::Reverse;
        break;
    case Forward:
        DoForward();
        _currentStepp = LogicStepp::Reverse;
        break;
    case Pause:
        DoPause();
        break;
    case Reverse:
        if(m_TargetPortions > 0)
        {
            if(m_TargetPortions==_portions_counter)
                _isCycleRun = false;
        }
        //_currentStepp=LogicStepp::Pause;
        if(_isCycleRun)
            _currentStepp = LogicStepp::Forward;
        else
            _currentStepp = LogicStepp::Stop;
        DoReverse();
        _portions_counter++;
        emit CycleDone();
        break;
    case Stop:

        break;
    }
}
