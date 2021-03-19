#ifndef STOBERSERVO_H
#define STOBERSERVO_H

#include "controllerbase.h"
//#include "can/festival/inc/canfestival.h"
#include "Device.h"
#include "MyMaster.h"

#include <QObject>
#include <QDebug>
#include <QElapsedTimer>
#include <IO/iobase.h>
#define CONTROL_WORD            0x6040
#define STATUS_WORD             0x6041
#define DEVICE_CONTROL_STATE    0x2830
#define POSITION_CONTROL_WORD   0x30d2
#define MOTION_COMMAND_BYTE     0x30d3
#define CHARGE_RELAY            0x280E
#define MOTION_STATUS_BYTE      0x30C9
#define CYCLE_TIME              0x2096

#define CYCLE_TIME_1MS       4
#define CYCLE_TIME_2MS       5
#define CYCLE_TIME_4MS       6
#define CYCLE_TIME_8MS       7
#define CYCLE_TIME_16MS      8
#define CYCLE_TIME_32MS      9
typedef enum InitStepp
{
    ToInit,
    ToWaitDCCharge,
    ToSwitchOn,
    ToEnableOp

}InitStepp_t;

class StoberServo : public ControllerBase
{
    Q_OBJECT
public:
    explicit StoberServo(IOBase *io, Device *device, QObject *parent = nullptr);
    void Connect();
    void setSlaveId(int slaveId)
    {
        if(_device != nullptr)
        {
            _device->setSlaveID(slaveId);
        }
    }
    void setChargeTimeout(int timeout);

private slots:
    void DevicePreOperational();
    void OnMotionDone();
signals:


    // ControllerBase interface
public:
    void Init() override;
    void EnableOperation() override;
    void DisableOperation() override;
    void Move(int target, MoveDirection dir) override;
    void StartMoveVelocity(int velocity) override;
    void StopMoveVelocity() override;
    void Homing() override;
    void setSpeed(int speed) override;
    bool isInit() override;
private:

    UNS8 _slaveId;
    IOBase *_io;
    Device *_device;

    int _waitTimeout = 100000;
};

#endif // STOBERSERVO_H
