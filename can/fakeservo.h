#ifndef FAKESERVO_H
#define FAKESERVO_H

#include "controllerbase.h"

#include <QTimer>
#include <QDebug>

#include <IO/iobase.h>


class FakeServo : public ControllerBase
{
public:
    FakeServo(IOBase *io, QObject *parent = nullptr);

    // ControllerBase interface
public:
    void EnableOperation() override;
    void DisableOperation() override;
    void Homing() override;
    void setSpeed(int speed) override;
    void Move(int target, MoveDirection dir) override;

private slots:

private:

    bool _isInit;


    // ControllerBase interface
public:


    // ControllerBase interface
public:
    void Init() override;
    bool isInit() override;

    // ControllerBase interface
public:
    void StartMoveVelocity(int velocity) override;
    void StopMoveVelocity() override;

    IOBase *_io;
};

#endif // FAKESERVO_H
