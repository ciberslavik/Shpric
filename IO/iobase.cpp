#include "iobase.h"

IOBase *IOBase::_instance = nullptr;

IOBase::IOBase(QObject *parent) : QObject(parent)
{

}
