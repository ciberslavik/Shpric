#ifndef IOBASE_H
#define IOBASE_H

#include <QObject>
#include <QBitArray>

class IOBase : public QObject
{
    Q_OBJECT
public:
    enum Outputs
    {
        SWIPER_ON   = 0,
        FC_POWER    = 2,
        FC_ENABLE   = 3,
        VACUUM_ON   = 1
    };
    enum Inputs
    {
        PEDAL_SW    = 0,
        BUNKER_SW   = 1,
        VACUUM_ASW  = 2,
        SWIPER_ASW  = 3,
        BUNKER2_SW  = 4,
        FC_FAULT    = 5,
    };


    static IOBase *_instance;
public:
    static IOBase *instance()
    {
        return _instance;
    }
    virtual void Init(int cycleTime) = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual bool Vacuum() = 0;
    bool isRun(){return _isRun;}
    bool isFCFault(){return _fcFault;}
    bool isVacuumFault(){return _vacuumFault;}
    bool isSwiperFault(){return _swiperFault;}
    bool getPedalState(){return _pedalState;}
public slots:
    virtual void SwiperMove(int moveTime) = 0;
    virtual void SwiperOn(bool state) = 0;
    virtual void VacuumOn(bool state) = 0;
    virtual void FCPowerOn(bool state) = 0;
    virtual void FCEnable(bool state) = 0;
    virtual QBitArray getOutputs() = 0;
    virtual void setOutputs(const QBitArray &outputs) = 0;
    virtual void setVacuum(double value) = 0;
signals:
    //debuging signals
    void rawDataRecived(QString data);
    void statusPrint(QString msg);
    void SwiperMoveDone();
    void PedalToggled(bool pedalState);
    void AnalogReady(double value);
    //Fault signals
    void FCFault();
    void VacuumFault();
    void SwiperFault();
    void CommunicationFault();
protected:
    explicit IOBase(QObject *parent = nullptr);
    void setInstance(IOBase *instance)
    {
        _instance = instance;
    }
    bool _fcFault;
    bool _vacuumFault;
    bool _swiperFault;
    bool _pedalState;
    bool _isRun;
};

#endif // IOBASE_H
