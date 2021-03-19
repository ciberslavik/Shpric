#ifndef CANCLLBACKPROCESSOR_H
#define CANCLLBACKPROCESSOR_H

#include <QObject>
#include "can/festival/inc/canfestival.h"
#include "can/Device.h"



class CallbackProcessor : public QObject
{
    Q_OBJECT

public:
    explicit CallbackProcessor(QObject *parent = nullptr);
    static void setPointer(Device *device);
    static void HeartbeatError(CO_Data* d, UNS8);
    static void Initialisation(CO_Data* d);
    static void PreOperational(CO_Data* d);
    static void Operational(CO_Data* d);
    static void Stopped(CO_Data* d);
    static void PostSync(CO_Data* d);
    static void PostTPDO(CO_Data* d);
    static void PostEMCY(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);
    static void PostSlaveBootup(CO_Data* d, UNS8 nodeid);
    static void PostSlaveStateChanged(CO_Data* d, UNS8 nodeId, e_nodeState newNodeState);
    static void InitNode(CO_Data* d, UNS32 id);
    static void StopTimer(CO_Data* d, UNS32 id);
//    static void CheckReadSDO(CO_Data* d, UNS8 slaveId);
//    static void CheckWriteSDO(CO_Data* d, UNS8 slaveId);

    static UNS32 StatusWordUpdate(CO_Data *d,const indextable * unsused_indextable, UNS8 unsused_bSubindex);
    static UNS32 MotionStatusByteUpdate(CO_Data *d,const indextable * unsused_indextable, UNS8 unsused_bSubindex);
signals:

private:
    static Device *_device;
};

#endif // CANCLLBACKPROCESSOR_H
