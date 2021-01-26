#ifndef IOWORKERCONFIG_H
#define IOWORKERCONFIG_H
#include "setting/ioserialconfig.h"

#include <QObject>

struct WorkerConfig
{
    u_int8_t slaveId;
    int cycleTime;
    SerialConfig *serialConfig;

    WorkerConfig(u_int8_t slaveid)
    {
        slaveId = slaveid;
    }
};

#endif // IOWORKERCONFIG_H
