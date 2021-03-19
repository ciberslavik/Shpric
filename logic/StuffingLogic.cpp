#include "StuffingLogic.h"

StuffingLogic::StuffingLogic(MachineController *controller):
    AbstractLogic(controller)
{
    _prevPedalState = false;
    qDebug()<<"Stuffing logic created";
}

void StuffingLogic::Reset()
{

}


void StuffingLogic::CycleStop()
{
}


LogicInfo StuffingLogic::getLogicInfo()
{
    LogicInfo info;
    info.logicName = "Stuffing";
    return info;
}

Logic_t StuffingLogic::Logic()
{
    return Logic::Stuffing;
}

void StuffingLogic::ServoMoveDone()
{
}

void StuffingLogic::SwiperMoveDone()
{
}

void StuffingLogic::PedalToggled(bool state)
{
    //Rising edge
    if((state == true) && (_prevPedalState == false))
    {
        _controller->StopMoveVelocity();
    }
    //Faling edge
    if((state == false) && (_prevPedalState == true))
    {
        _controller->StartMoveVelocity(_presset->Temp());
    }
    _prevPedalState = state;
}

bool StuffingLogic::CheckLogicPresset(PressetConfig *presset)
{
    if(presset->Logic() == static_cast<int>(Logic::Stuffing))
        return true;
    else
        return false;
}


