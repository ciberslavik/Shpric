#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
typedef enum
{
    Relative,
    Absolute,
    Additive,
    Homing
}MoveType;
typedef enum
{
    Forward,
    Reverse
}MoveDirection;

class ControllerBase : public QObject
{
    Q_OBJECT
public:
    static ControllerBase *Instance()
    {
        return _instance;
    }
    virtual void Init() = 0;
    virtual void EnableOperation() = 0;
    virtual void DisableOperation() = 0;
    virtual void Move(int target, MoveDirection dir) = 0;
    virtual void StartMoveVelocity(int velocity) = 0;
    virtual void StopMoveVelocity() = 0;
    virtual void Homing() = 0;
    virtual void setSpeed(int speed) = 0;
    virtual bool isInit() = 0;
protected:
    explicit ControllerBase(QObject *parent = nullptr);
    static ControllerBase *_instance;
    static void setInstance(ControllerBase *instance)
    {
        if(_instance==nullptr)
        {
            _instance = instance;
        }
    }
signals:
    void MoveDone();
    void Fault();
private:

};

#endif // CONTROLLERBASE_H
