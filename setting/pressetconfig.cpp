#include "pressetconfig.h"

PressetConfig::PressetConfig(QObject *parent) : QObject(parent),
    _pressetName("")
{

}


void PressetConfig::setSwiperPulse(int value)
{
    if(_swiperPulse != value)
    {
        _swiperPulse = value;
        _isModified = true;
    }
}

void PressetConfig::setPressetName(QString value)
{
    if(_pressetName != value)
    {
        _pressetName = value;
        _isModified = true;
    }
}

void PressetConfig::setTemp(int value)
{
    if(_temp != value)
    {
        _temp = value;
        _isModified = true;
    }
}

void PressetConfig::setClipperPulse(int value)
{
    if(_clipperPulse != value)
    {
        _clipperPulse = value;
        _isModified = true;
    }
}

void PressetConfig::setMass(int value)
{
    if(_mass != value)
    {
        _mass = value;
        _isModified = true;
    }
}

void PressetConfig::setPause(int value)
{
    if(_pause != value)
    {
        _pause = value;
        _isModified = true;
    }
}

void PressetConfig::setFirstPortion(int value)
{
    if(_firstPortion != value)
    {
        _firstPortion = value;
        _isModified = true;
    }
}

void PressetConfig::setReverse(int value)
{
    if(_reverse != value)
    {
        _reverse = value;
        _isModified = true;
    }
}

void PressetConfig::setLogic(int mode)
{
    if(_logic != mode)
    {
        _logic = mode;
        _isModified = true;
    }
}

PressetConfig *PressetConfig::Copy()
{
    PressetConfig *config = new PressetConfig(this->parent());
    config->_pressetName = _pressetName;
    config->_temp = _temp;
    config->_mass = _mass;
    config->_firstPortion=_firstPortion;
    config->_clipperPulse = _clipperPulse;
    config->_swiperPulse = _swiperPulse;
    config->_logic = _logic;
    config->_pause = _pause;
    config->_reverse = _reverse;
    return config;
}
