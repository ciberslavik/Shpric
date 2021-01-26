#ifndef MACHINECONTROLLER_H
#define MACHINECONTROLLER_H

#include <QObject>
#include <QMetaObject>
#include <QStack>
#include <setting/machineconfig.h>
#include <IO/iobase.h>
#include <can/controllerbase.h>

class MachineController : public QObject
{
    Q_OBJECT
public:
    explicit MachineController(MachineConfig *cinfig,IOBase *io, ControllerBase *servo, QObject *parent = nullptr);
    virtual ~MachineController();
    void EnableOperation();
    void DisableOperation();
    void FaultReset();

    void Swipe(int revolutions);
    void ForwardMove(double mass);
    void StartMoveVelocity(int speed);
    void StopMoveVelocity();
    void ReverseMove(double mass);
    bool Vacuum();
    void setVacuum(bool state);
    void setVacuumValue(double value);
    void setSpeed(int speed);
signals:
    void ServoMoveDone();
    void SwiperMoveDone();
    void MachineFault(QString msg);
    void PedalToggled(bool state);
    void Fault(QString message);
    void AnalogDataReady(double value);
private slots:
    void onSwiperMoveDone();
    void onServoMoveDone();
    void onSwiperFault();
    void onVacuumFault();
    void onIOCommFault();
    void onServoFault();
    void onAnalogReady(double value);
private:
    bool _homingFlag = false;
    IOBase *_io;
    ControllerBase *_servo;
    MachineConfig *_config;

    QStack<QMetaObject::Connection> _signalConnections;
};

#endif // MACHINECONTROLLER_H
