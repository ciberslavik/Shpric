#include "configframebase.h"


ConfigFrameBase::ConfigFrameBase(AbstractConfig *config, QWidget *parent) :
    QWidget(parent),
    _curentConfig(config)
{

}

ConfigFrameBase::~ConfigFrameBase()
{

}

void ConfigFrameBase::SaveConfig()
{
    if(_curentConfig!=nullptr)
    {
        _curentConfig->Save();
    }
}
