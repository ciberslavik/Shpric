#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H
#include "unix/canfestival.h"
#include "MyMaster.h"
#include "controllerbase.h"
#include <QObject>
#include <QBitArray>
#include <IO/iobase.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define ControlWord         0x6040
#define DeviceControlState  0x2830
#define PositionControlWord 0x30d2
#define MotionCommandByte   0x30d3
#define TargetSpeed         0x30d7
#define TargetPosition      0x30d5

#define MC_MoveAbsolute     0x1U
#define MC_MoveRelative     0x2U
#define MC_MoveAdditive     0x3U
#define MC_MoveVelocity     0x4U
#define MC_Stop             0x5U
#define MC_Home             0x6U
#define MC_Reset            0x7U

#define MC_ForwardDir       0x40
#define MC_ReverseDir       0x80

#define rCycleTime           0x2096

#define CycleTime_1ms       4
#define CycleTime_2ms       5
#define CycleTime_4ms       6
#define CycleTime_8ms       7
#define CycleTime_16ms      8
#define CycleTime_32ms      9

#define ExecutionDone          (1<<5)
//#define TargetReached          (1<<10)
#define ReadyToSwitchOnBit     (1<<0)
#define SwitchedOnBit           (1<<1)
#define OpEnabledBit            (1<<2)

typedef enum ControlMode
{
    Shutdown,
    SwitchOn,
    DisableVoltage,
    QuickStop,
    DisableOperation,
    EnableOperation,
    FaultReset
}ControlMode_t;

typedef struct StatusWord
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

class ServoController : public ControllerBase
{
    Q_OBJECT
public:
    ServoController(IOBase *io, QObject *parent = nullptr);

    void Connect();
    void setDeviceMode(ControlMode_t mode);

    void setSlaveId(u_int8_t slaveId);
    u_int8_t SlaveId();

    StatusWord_t getStatusWord();
    void CanDummyConnect();
    void EnableOperation() override;

    void setSpeed(int speed)override;
    void Move(int target, MoveDirection direction)override;
    void Home();
    void DisableOperation() override;

    void Homing() override;
    void Init() override;
    bool isInit() override;
    void StartMoveVelocity(int velocity) override;
    void StopMoveVelocity() override;
signals:
    void onDeviceReady();
    void onPostTPDO();
    void onFault(QString faultMessage);
    void positioningDone();
private slots:

private:

    u_int8_t _slaveId = 0;
    int _masterSyncCount = 0;
    int _configureSlaveStep = 0;
    bool _is_moving;
    bool _exec_flag;
    bool _isOperational;
    bool old_exec_flag;
    quint8 oldPlcState;
    s_BOARD MasterBoard;
    CO_Data *_co_data;
    StatusWord_t _statusWord;
    QString _deviceStateStr;
    //static ServoController *_instance;
    static bool waiting_answer;
    static bool slaveBootUp;
    const static QString deviceStateStrings[];
    IOBase *_io;
    //Static callbac`s
    static void heartbeatError(CO_Data* d, UNS8);
    static void initialisation(CO_Data* d);
    static void preOperational(CO_Data* d);
    static void operational(CO_Data* d);
    static void stopped(CO_Data* d);
    static void post_sync(CO_Data* d);
    static void post_TPDO(CO_Data* d);
    static void post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);
    static void post_SlaveBootup(CO_Data* d, UNS8 nodeid);
    static void post_SlaveStateChanged(CO_Data* d, UNS8 nodeId, e_nodeState newNodeState);
    static void InitNode(CO_Data* d, UNS32 id);
    static void DummyInit(CO_Data* d, UNS32 id);
    static void DeInitNode(CO_Data* d, UNS32 id);
    static void CheckSDO(CO_Data* d, UNS8 nodeId);

    static LIB_HANDLE drvHandle;
    void ReadDeviceControlState();
    static void ReadDeviceControlStateCallback(CO_Data *d, u_int8_t nodeId);

    void ConfigureSlave(CO_Data *d, u_int8_t nodeId);
    static void ConfigureSlaveCheckSDO(CO_Data *d, u_int8_t nodeId);

    void parseStatusWord(u_int16_t statusWord);

    QBitArray bitsFromByte(u_int16_t *data, int size);

    //bool ReadDCLinkState();
    //static void ReadDCLinkStateCallback(CO_Data *d, u_int8_t nodeId);

    UNS8 ReadSDO(UNS8 nodeId, UNS16 index, UNS8 subIndex, UNS8 dataType, void* data, UNS32* size);




};
//#pragma GCC diagnostic pop
#endif // SERVOCONTROLLER_H
