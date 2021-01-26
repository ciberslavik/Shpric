#ifndef MACHINE_H
#define MACHINE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <can/controllerbase.h>

#include <IO/iobase.h>

#include "logic/AbstractLogic.h"
#include "logic/PortionLogic.h"
#include "setting/machineconfig.h"
#include "machinecontroller.h"
#include "machineio_.h"

class Machine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int TargetPortions READ TargetPortions WRITE setTargetPortions NOTIFY TargetPortionsChanged);
    Q_PROPERTY(AbstractLogic* CurrentLogic READ CurrentLogic WRITE setCurrentLogic NOTIFY CurrentLogicChanged)
public:
    \
    explicit Machine(QObject *parent = nullptr);
    void InitMachine(MachineController *controller);
    //Control functions
    void EnableOperation();
    void DisableOperation();

    //Logic functions
    void LoadLogicPresset(PressetConfig *presset);
    void setLogicMode(enum LogicMode logicMode);
    enum LogicMode LogicMode(){return _currentMode;}
    //Status functions
    bool isEnabled(){return _enabled;}
    bool isInit(){return _is_init;}
    bool isCyclic(){return _isCyclic;}
    bool Vacuum(){return _controller->Vacuum();}
    void Vacuum(bool state){_controller->setVacuum(state);}
    void setVacuum(double value);


    QString getErrorString();
    int TargetPortions() const
    {
        return m_TargetPortions;
    }
    int Portions()
    {
        if(_currentLogic!=nullptr)
            return  _currentLogic->Portions();
        else
            return 0;
    }
    AbstractLogic *CurrentLogic()
    {
        if(_currentLogic!=nullptr)
            return _currentLogic;
        else
            return nullptr;
    }

public slots:
    void onPedalToggled(bool state);
    void setTargetPortions(int TargetPortions)
    {
        if (m_TargetPortions == TargetPortions)
            return;

        m_TargetPortions = TargetPortions;
        _currentLogic->setTargetPortions(m_TargetPortions);
        emit TargetPortionsChanged(m_TargetPortions);
    }
    void setCurrentLogic(AbstractLogic *logic)
    {
        if(_currentLogic == logic)
            return;

        _currentLogic = logic;
        emit CurrentLogicChanged(logic);
    }
private slots:
    void onIOFault();
    void onServoFault(QString message);
    void onLogicCycleDone();
    void onAnalogDataReady(double value);
signals:
    void onError();
    void onFault();
    void CycleDone();
    void TargetPortionsChanged(int TargetPortions);
    void CurrentLogicChanged(AbstractLogic *logic);
    void AnalogDataReady(double value);
private:

    bool _enabled;
    bool _is_init;

    bool _isCyclic;
    bool _isCyclicRun;
    bool _pedalPrevState;

    MachineController *_controller;
    AbstractLogic *_currentLogic;
    enum LogicMode _currentMode;
    QTimer _cyclicTimer;

    QStack<QMetaObject::Connection> _logicSignals;
    //methods
    void deleteCurrentLogic();
    AbstractLogic *createLogic(PressetConfig *config);
    AbstractLogic *createSwiperLogic();
    AbstractLogic *createPortionLogic();
    AbstractLogic *createStuffingLogic();

    PressetConfig *_currentPresset;
    MachineConfig *getDefaultConfig();
    int m_TargetPortions;
};

#endif // MACHINE_H
