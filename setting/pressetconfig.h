#ifndef PRESSETCONFIG_H
#define PRESSETCONFIG_H

#include <QObject>
typedef enum class Logic : int
{
    Portion=0,
    Swiper=1,
    Stuffing=2
}Logic_t;

class PressetConfig : public QObject
{
    Q_OBJECT
public:


    explicit PressetConfig(QObject *parent = nullptr);
    PressetConfig(const PressetConfig &config);
    Q_PROPERTY(QString PressetName READ PressetName WRITE setPressetName STORED true)
    Q_PROPERTY(int Temp READ Temp WRITE setTemp STORED true)
    Q_PROPERTY(int ClipperPulse READ ClipperPulse WRITE setClipperPulse STORED true)
    Q_PROPERTY(int Mass READ Mass WRITE setMass STORED true)
    Q_PROPERTY(int Pause READ Pause WRITE setPause STORED true)
    Q_PROPERTY(int FirstPortion READ FirstPortion WRITE setFirstPortion STORED true)
    Q_PROPERTY(int Reverse READ Reverse WRITE setReverse STORED true)
    Q_PROPERTY(int Logic READ Logic WRITE setLogic STORED true)
    Q_PROPERTY(int SwiperPulse READ SwiperPulse WRITE setSwiperPulse STORED true)

    int SwiperPulse(){return _swiperPulse;}
    void setSwiperPulse(int value);

    QString PressetName(){return _pressetName;}
    void setPressetName(QString value);

    int Temp(){return _temp;}
    void setTemp(int value);

    int ClipperPulse(){return _clipperPulse;}
    void setClipperPulse(int value);

    int Mass(){return _mass;}
    void setMass(int value);

    int Pause(){return _pause;}
    void setPause(int value);

    int FirstPortion(){return _firstPortion;}
    void setFirstPortion(int value);

    int Reverse(){return _reverse;}
    void setReverse(int value);

    int Logic(){return _logic;}
    void setLogic(int mode);

    bool isModified(){return _isModified;}

    bool isEqual(PressetConfig *presset)
    {
        if(_pressetName!=presset->PressetName())
            return false;
        if(_temp!=presset->Temp())
            return false;
        if(_clipperPulse!=presset->ClipperPulse())
            return false;
        if(_mass!=presset->Mass())
            return false;
        if(_pause!=presset->Pause())
            return false;
        if(_firstPortion!=presset->FirstPortion())
            return false;
        if(_reverse!=presset->Reverse())
            return false;
        if(_logic!=presset->Logic())
            return false;
        if(_swiperPulse!=presset->SwiperPulse())
            return false;

        return true;

    }
signals:

private:
    QString _pressetName;
    int _temp;
    int _clipperPulse;
    int _mass;
    int _pause;
    int _firstPortion;
    int _reverse;
    int _logic;
    int _swiperPulse;
    bool _isModified = false;
};

#endif // PRESSETCONFIG_H
