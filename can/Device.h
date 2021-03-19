#ifndef DEVICE_H
#define DEVICE_H

#include "can/festival/inc/canfestival.h"
#include <sys/socket.h>
#include <unistd.h>
#include <QMap>
#include <QObject>
#include <QSocketNotifier>
#include <QMutex>
#define CONTROL_WORD        0x6040
#define POSI_CONTROL_WORD   0x30D2
#define STATUS_WORD         0x6041
#define CHARGE_RELAY        0x280E
#define MOTION_COMMAND      0x30D3
#define MOTION_STATUS_BYTE  0x30C9
#define TARGET_POSITION     0x30D5
#define VELOCITY_FACTOR     0x30D7

enum class DriverType
{
    Socket,
    Virtual
};
enum class NMTState
{
    StartNode,
    StopNode,
    EnterPreOpertional,
    ResetNode,
    ResetCommunication
};

enum class MotionCommand : UNS8
{
    MoveAbsolute = 0x1U,
    MoveRelative = 0x2U,
    MoveAdditive = 0x3U,
    MoveVelocity = 0x4U,
    Stop         = 0x5U,
    Home         = 0x6U,
    Reset        = 0x7U
};
enum class MotionDirection : UNS8
{
    Forward = 0x40U,
    Reverse = 0x80U
};

typedef struct StatusWordStruct
{
    bool ReadyToSwitchOn;
    bool SwitchedOn;
    bool OpEnabled;
    bool Fault;
    bool VoltageEnabed;
    bool QuickStop;
    bool SwitchOnDisable;
    bool Warning;
    bool Message;
    bool TargetReached;

}StatusWord_t;

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    void Init(DriverType driverType, QString portName);
    void DeInit();
    void Start();
    void Stop();

    void setSyncCyclePeriod(unsigned int period);
    void setSlaveID(UNS8 slaveId){_slaveId = slaveId;}

public slots:
    bool getChargeRelay();
    UNS16 getStatusWord();
    void setControlWord(UNS16 controlWord);
    void setTargetPosition(INTEGER32 targetPosition);
    void setVFactor(INTEGER16 vFactor);
    void setMotionCommand(MotionCommand command, MotionDirection dir);

    void setSlaveState(NMTState state);
    void setExecute();
    void resetExecute();
    UNS32 getLastError(){return _lastError;}

private slots:
    void OnMotionDone(int sockfd);
signals:
    void DeviceInitialization();
    void DevicePreOperational();
    void DeviceOperational();

    void MotionDone();
    void Fault();

    void CommunicationError(UNS32 abortCode);
private:
    CO_Data *_od;
    LIB_HANDLE _driverHandle;
    CAN_PORT _portHandle;
    s_BOARD _connData;
    bool _isConnected;
    UNS8 _slaveId;

    bool _isMoving;
    bool _oldWindowReached;
    StatusWord_t _statusWord;
    UNS16 _controlWord;
    UNS32 _lastError;
    UNS8 _oldPlcState;
    UNS8 _plcState;

    UNS8 _oldMotionStatus;
    UNS16 _oldStatusWord;

    void WriteSDOSynchronus(UNS16 index, UNS8 subIndex, void *data, UNS32 size);
    void ReadSDOSyncronus(UNS16 index, UNS8 subIndex, void *data, UNS32 *size);

    void parseStatusWord(u_int16_t data);
    QBitArray bitsFromByte(u_int16_t *data, int size);

    void OnHeartbeatError(CO_Data* d, UNS8);
    void OnInitialisation(CO_Data* d);
    void OnPreOperational(CO_Data* d);
    void OnOperational(CO_Data* d);
    void OnStopped(CO_Data* d);
    void OnPostSync(CO_Data* d);
    void OnPostTPDO(CO_Data* d);
    void OnPostEMCY(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);
    void OnPostSlaveBootup(CO_Data* d, UNS8 nodeid);
    void OnPostSlaveStateChanged(CO_Data* d, UNS8 nodeId, e_nodeState newNodeState);
    void OnInitNode(CO_Data* d, UNS32 id);
   // void OnCheckReadSDO(CO_Data* d, UNS8 slaveId);
    //void OnCheckWriteSDO(CO_Data* d, UNS8 slaveId);
    void OnStopTimer(CO_Data* d, UNS32 id);

    u_int32_t OnStatusWordUpdate(CO_Data *d,const indextable * unsused_indextable, UNS8 unsused_bSubindex);
    u_int32_t OnMotionStatusByteUpdate(CO_Data *d,const indextable * unsused_indextable, UNS8 unsused_bSubindex);
    friend class CallbackProcessor;
    QSocketNotifier *_motionNotifier;
    QMutex _plcLocker;
    static int motionSocketpair[2];
};

#endif // DEVICE_H
