#include "SwiperLogic.h"

#include <setting/machineconfig.h>

SwiperLogic::SwiperLogic(MachineController *controller):
    AbstractLogic(controller)
{
    connect(&_pauseTimer, &QTimer::timeout, this, &SwiperLogic::pauseTimeout);
    _isCycleRun = false;
    _isFirsPortion = true;
    _portions =0;
    _portions_counter =0;
    qDebug()<<"Swiper logic created";
}

SwiperLogic::~SwiperLogic()
{
    qDebug()<<"Swiper logic deleted";
}

void SwiperLogic::Reset()
{
    _isFirsPortion = true;
    _isCycleRun = false;
    _portions_counter = 0;
    emit CycleDone();
}

//void SwiperLogic::SingleStart()
//{

//}

LogicInfo SwiperLogic::getLogicInfo()
{
    LogicInfo info;
    info.logicName = "Swiper";
    return info;
}

void SwiperLogic::SwiperMoveDone()
{
    qDebug()<<"Swiper MoveDone elapsed:";
    //<<_timer.elapsed();
    LogicCycle();
}

void SwiperLogic::PedalToggled(bool state)
{
    if(LogicMode()==LogicMode::Stop)
        return;
    //Rising edge
    if((state == true) && (_prevPedalState == false))
    {
        qDebug() << "Pedal Rising edge";
    }
    //Faling edge
    if((state == false) && (_prevPedalState == true))
    {
        qDebug() << "Pedal Faling edge";
        if(_isFirsPortion)
        {
            _currentStepp = LogicStepp::FirstPortion;
            _isFirsPortion = false;
            _isCycleRun = true;
            _controller->setSpeed(_presset->Temp());
            LogicCycle();
        }
        else
        {
            if(_isPaused)
            {
                qDebug()<<"UnPaused";
                _isCycleRun = true;
                _isPaused = false;
                _controller->setSpeed(_presset->Temp());
                _currentStepp = LogicStepp::Forward;
            }
            else
            {
                qDebug()<<"Paused";
                _isCycleRun = false;
                _isPaused = true;
            }
            LogicCycle();
        }
    }
    _prevPedalState = state;
}


void SwiperLogic::ServoMoveDone()
{
    qDebug()<<"Servo MoveDone";
    LogicCycle();
}

//void SwiperLogic::CycleStart()
//{
//    if(_isFirsPortion)
//    {
//        _currentStepp = LogicStepp::FirstPortion;
//    }
//    if(_isPaused)
//    {
//        _isPaused = false;
//        _currentStepp = LogicStepp::Forward;
//    }
//    _isCyclic = _isCycleRun = true;

//    LogicCycle();
//}

void SwiperLogic::CycleStop()
{
    _isCycleRun = false;
}
bool SwiperLogic::CheckLogicPresset(PressetConfig *presset)
{
    if(presset->Logic() == static_cast<int>(Logic::Swiper))
        return true;
    else
        return false;
}

void SwiperLogic::pauseTimeout()
{
    _pauseTimer.stop();
    _currentStepp = _nextStepp;
    LogicCycle();
}


void SwiperLogic::DoFirstPortion()
{
    qDebug()<<"First Portion";
    _controller->ForwardMove(_presset->FirstPortion());
}

void SwiperLogic::DoSwipe()
{
    qDebug()<<"Swiping";
    //_timer.start();

    _controller->Swipe(_presset->SwiperPulse());
}

void SwiperLogic::DoReverse()
{
    _controller->ReverseMove(_presset->Reverse());
}

void SwiperLogic::DoPause()
{
    qDebug()<<"Pause" <<_presset->Pause();
    _pauseTimer.setInterval(_presset->Pause());
    _pauseTimer.setSingleShot(true);
    _pauseTimer.start();
}

void SwiperLogic::DoForward()
{
    _controller->ForwardMove(_presset->Mass());
}

void SwiperLogic::LogicCycle()
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
    case Swiping:
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
        DoSwipe();
        _portions_counter++;
        emit CycleDone();
        break;
    case Reverse:
        DoReverse();
        _currentStepp = LogicStepp::Pause;
        _nextStepp = LogicStepp::Swiping;
        break;
    case Stop:

        break;
    }
}

//void SwiperLogic::cycleDone()
//{
//}




Logic_t SwiperLogic::Logic()
{
    return Logic::Swiper;
}


//void SwiperLogic::CyclePause()
//{
//    qDebug()<<"CyclePause";
//    _isPaused = true;
//    _isCycleRun = false;
//}
