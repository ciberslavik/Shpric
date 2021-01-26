#ifndef LOGICHELPER_H
#define LOGICHELPER_H

#include "AbstractConfig.h"

#include <QObject>

class MachineConfig :public AbstractConfig
{
    Q_OBJECT
    int m_EncoderTicks;
    double m_GearboxRatio;
    double m_MassPerPumpRev;
    int m_SwiperSpeed;
    double m_SwiperRPS;
    double m_SwiperRevTime;
    double m_SwiperGearRatio;

public:
    MachineConfig(QObject *parent = nullptr);

    Q_PROPERTY(int EncoderTicks READ EncoderTicks WRITE setEncoderTicks NOTIFY EncoderTicksChanged STORED true)
    Q_PROPERTY(double GearboxRatio READ GearboxRatio WRITE setGearboxRatio NOTIFY GearboxRatioChanged STORED true)
    Q_PROPERTY(double MassPerPumpRev READ MassPerPumpRev WRITE setMassPerPumpRev NOTIFY MassPerPumpRevChanged STORED true)
    Q_PROPERTY(int SwiperSpeed READ SwiperSpeed WRITE setSwiperSpeed NOTIFY SwiperSpeedChanged STORED true)
    Q_PROPERTY(double SwiperGearRatio READ SwiperGearRatio WRITE setSwiperGearRatio NOTIFY SwiperGearRatioChanged STORED true)


    //Calculated values
    int TicksPerPumpRev() const
    {
        return static_cast<int>(m_EncoderTicks * m_GearboxRatio);
    }
    int TicksPerSM3()const
    {
        return static_cast<int>(TicksPerPumpRev()/m_MassPerPumpRev);
    }
    int MassToTicks(double mass)
    {
        return static_cast<int>(mass * TicksPerSM3());
    }
    double SwiperRevTime()
    {
        m_SwiperRPS = (m_SwiperSpeed/m_SwiperGearRatio) / 60;
        m_SwiperRevTime = 1000 / m_SwiperRPS;
        return m_SwiperRevTime;
    }
    double SwiperRevToTime(int revolutions)
    {
        return SwiperRevTime() * revolutions;
    }
    //Stored values
    int EncoderTicks() const
    {
        return m_EncoderTicks;
    }
    double GearboxRatio() const
    {
        return m_GearboxRatio;
    }

    double MassPerPumpRev() const
    {
        return m_MassPerPumpRev;
    }

    int SwiperSpeed() const
    {
        return m_SwiperSpeed;
    }
    double SwiperGearRatio() const
    {
        return m_SwiperGearRatio;
    }
public slots:
    void setEncoderTicks(int EncoderTicks)
    {
        if (m_EncoderTicks == EncoderTicks)
            return;

        m_EncoderTicks = EncoderTicks;
        emit EncoderTicksChanged(m_EncoderTicks);
    }
    void setGearboxRatio(double GearboxRatio)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_GearboxRatio, GearboxRatio))
            return;

        m_GearboxRatio = GearboxRatio;
        emit GearboxRatioChanged(m_GearboxRatio);
    }

    void setMassPerPumpRev(double MassPerPumpRev)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_MassPerPumpRev, MassPerPumpRev))
            return;

        m_MassPerPumpRev = MassPerPumpRev;
        emit MassPerPumpRevChanged(m_MassPerPumpRev);
    }

    void setSwiperSpeed(int SwiperSpeed)
    {
        if (m_SwiperSpeed == SwiperSpeed)
            return;
        m_SwiperSpeed = SwiperSpeed;
        emit SwiperSpeedChanged(m_SwiperSpeed);
    }

    void setSwiperGearRatio(double SwiperGearRatio)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_SwiperGearRatio, SwiperGearRatio))
            return;

        m_SwiperGearRatio = SwiperGearRatio;
        emit SwiperGearRatioChanged(m_SwiperGearRatio);
    }

signals:
    void EncoderTicksChanged(int EncoderTicks);
    void GearboxRatioChanged(double GearboxRatio);
    void MassPerPumpRevChanged(double MassPerPumpRev);
    void SwiperSpeedChanged(int SwiperSpeed);

    // AbstractConfig interface
    void SwiperGearRatioChanged(double SwiperGearRatio);

public:
    QString ConfigName() override
    {
        return "MachineConfig";
    }

    // AbstractConfig interface
public:
    AbstractConfig *CreateDefaultConfig() override;

};

#endif // LOGICHELPER_H
