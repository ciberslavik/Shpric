#ifndef ABSTRACTLOGIC_H
#define ABSTRACTLOGIC_H
#include "can/controllerbase.h"
#include "machineio_.h"

#include <IO/iobase.h>

#include <setting/pressetconfig.h>
#include <machinecontroller.h>
#include <qtimer.h>
//#include
typedef struct
{
    QString logicName;
    QString typeName;

}LogicInfo;
typedef enum class LogicMode
{
    Single = 0,
    CyclicEndless = 1,
    Cyclic = 2,
    Stop = 3
}LogicMode_t;

class AbstractLogic:public QObject
{
    Q_OBJECT

    Q_PROPERTY(int TargetPortions READ TargetPortions WRITE setTargetPortions NOTIFY TargetPortionsChanged)
    Q_PROPERTY(LogicMode_t LogicMode READ LogicMode WRITE setLogicMode NOTIFY LogicModeChanged)

    LogicMode_t m_LogicMode;

public:
   // virtual void SingleStart() = 0;
   // virtual void CycleStart() = 0;
    virtual void CycleStop() = 0;
    virtual void Reset() = 0;
   // virtual void CyclePause() = 0;
    virtual LogicInfo getLogicInfo() = 0;
    //virtual void setPortions(int portions){_portions = portions;}

    bool isCycleRun(){return _isCycleRun;}
    bool isCyclePaused(){return _isPaused;}
    bool LoadPresset(PressetConfig *presset);
    PressetConfig *Presset(){return _presset;}
    virtual Logic_t Logic()=0;
    int TargetPortions() const
    {
        return m_TargetPortions;
    }
    int Portions() {return _portions_counter;}
    LogicMode_t LogicMode() const
    {
        return m_LogicMode;
    }

public slots:
    virtual void ServoMoveDone() = 0;
    virtual void SwiperMoveDone() = 0;
    virtual void PedalToggled(bool state) = 0;
    //virtual void cycleDone() = 0;
    void setTargetPortions(int TargetPortions)
    {
        if (m_TargetPortions == TargetPortions)
            return;

        m_TargetPortions = TargetPortions;
        emit TargetPortionsChanged(m_TargetPortions);
    }

    void setLogicMode(LogicMode_t logicMode)
    {
        if (m_LogicMode == logicMode)
            return;

        m_LogicMode = logicMode;
        onLogicModeChanged(logicMode);
        emit LogicModeChanged(m_LogicMode);
    }

signals:
    void CycleDone();
    void LogicModeChanged(LogicMode_t logicMode);
    void LogicPressetChanged();
    void TargetPortionsChanged(int TargetPortions);

protected:
    AbstractLogic(MachineController *controller);

    virtual bool CheckLogicPresset(PressetConfig *presset) = 0;
    virtual void onLogicModeChanged(LogicMode_t logicMode)
    {
        Q_UNUSED(logicMode)
    };
    bool _isCycleRun;
    bool _isPaused;
    MachineController *_controller;
    QTimer _cyclicTimer;

    PressetConfig *_presset;
    int _portions;
    int _portions_counter;
    int m_TargetPortions;
};

#endif // ABSTRACTLOGIC_H
