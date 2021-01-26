#include "AbstractLogic.h"

bool AbstractLogic::LoadPresset(PressetConfig *presset)
{
    if(presset == nullptr)
        return false;

    if(!CheckLogicPresset(presset))
        return false;

    if((_presset == nullptr)||(_presset != presset))
    {
        _presset = presset;
        setPressetConfig();
        emit LogicPressetChanged();
        return true;
    }
    return false;
}



AbstractLogic::AbstractLogic(MachineController *controller)
{
    _controller = controller;
    m_TargetPortions = 0;
}

