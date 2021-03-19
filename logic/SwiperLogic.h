#ifndef SWIPERLOGIC_H
#define SWIPERLOGIC_H
#include <QDebug>
#include <QElapsedTimer>
#include "AbstractLogic.h"



class SwiperLogic : public AbstractLogic
{
    Q_OBJECT
    enum LogicStepp
    {
        FirstPortion,
        Forward,
        Swiping,
        Reverse,
        Pause,
        Stop
    };

public:
    SwiperLogic(MachineController *controller);
    ~SwiperLogic();
    void CycleStop() override;
    void Reset() override;
    //    void CyclePause() override;
    //    void SingleStart() override;
    //    void CycleStart() override;
    LogicInfo getLogicInfo() override;
    Logic_t Logic() override;
    //void setPortions(int portions) override;

public slots:
    void ServoMoveDone() override;
    void SwiperMoveDone() override;
    void PedalToggled(bool state) override;
    //void cycleDone() override;
protected:
    bool CheckLogicPresset(PressetConfig *presset) override;
private slots:
    void pauseTimeout();

private:
    bool _isFirsPortion,_isCyclic;
    LogicStepp _currentStepp;
    LogicStepp _nextStepp;
    QTimer _pauseTimer;
    bool _prevPedalState;

    // QElapsedTimer *_timer;
    void DoFirstPortion();
    void DoSwipe();
    void DoReverse();
    void DoPause();
    void DoForward();
    void LogicCycle();

};

#endif // SWIPERLOGIC_H
