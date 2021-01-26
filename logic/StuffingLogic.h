#ifndef STUFFINGLOGIC_H
#define STUFFINGLOGIC_H

#include "AbstractLogic.h"
#include <QDebug>


class StuffingLogic : public AbstractLogic
{
    Q_OBJECT
public:
    StuffingLogic(MachineController *controller);

    // AbstractLogic interface
public:
//    void SingleStart() override;
//    void CycleStart() override;
    void CycleStop() override;
    void Reset() override;
//    void CyclePause() override;
    LogicInfo getLogicInfo() override;
    Logic_t Logic() override;

public slots:
    void ServoMoveDone() override;
    void SwiperMoveDone() override;
    void PedalToggled(bool state) override;
protected:
    bool CheckLogicPresset(PressetConfig *presset) override;
    bool setPressetConfig() override;

    bool _prevPedalState;
};

#endif // STUFFINGLOGIC_H
