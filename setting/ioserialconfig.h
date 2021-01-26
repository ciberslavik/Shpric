#ifndef IOSERIALCONFIG_H
#define IOSERIALCONFIG_H
#include "AbstractConfig.h"

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialConfig :public AbstractConfig
{
    Q_OBJECT

    QString m_PortName;
    int m_Baudrate;
    int m_DataBits;
    int m_StopBits;
    QString m_Parity;
    int m_Timeout;
    int m_Retries;
    int m_CycleTime;
    int m_SlaveAddress;
public:
    Q_PROPERTY(QString PortName READ PortName WRITE setPortName NOTIFY PortNameChanged STORED true)
    Q_PROPERTY(int Baudrate READ Baudrate WRITE setBaudrate NOTIFY BaudrateChanged STORED true)
    Q_PROPERTY(int DataBits READ DataBits WRITE setDataBits NOTIFY DataBitsChanged STORED true)
    Q_PROPERTY(int StopBits READ StopBits WRITE setStopBits NOTIFY StopBitsChanged STORED true)
    Q_PROPERTY(QString Parity READ Parity WRITE setParity NOTIFY ParityChanged STORED true)
    Q_PROPERTY(int Timeout READ Timeout WRITE setTimeout NOTIFY TimeoutChanged STORED true)
    Q_PROPERTY(int Retries READ Retries WRITE setRetries NOTIFY RetriesChanged STORED true)
    Q_PROPERTY(int CycleTime READ CycleTime WRITE setCycleTime NOTIFY CycleTimeChanged STORED true)
    Q_PROPERTY(int SlaveAddress READ SlaveAddress WRITE setSlaveAddress NOTIFY SlaveAddressChanged STORED true)

    QString PortName() const
    {
        return m_PortName;
    }
    int Baudrate() const
    {
        return m_Baudrate;
    }

    int DataBits() const
    {
        return m_DataBits;
    }

    int StopBits() const
    {
        return m_StopBits;
    }

    QString Parity() const
    {
        return m_Parity;
    }

    int Timeout() const
    {
        return m_Timeout;
    }

    int Retries() const
    {
        return m_Retries;
    }
    QList<QString> getAvaliblePorts()
    {
        // Get list of available ports
        QList<QSerialPortInfo> info = QSerialPortInfo::availablePorts();
        QList<QString> ports;
        if (info.count()>0)
        {
            for(int i=0;i<info.count();i++)
            {
                ports.append(info[i].portName());
            }
        }
        return ports;
    }
    QSerialPort::Parity getPariry()
        {
            if (m_Parity == "None")
                return QSerialPort::NoParity;

            if (m_Parity == "Odd")
                return QSerialPort::OddParity;

            if (m_Parity == "Even")
                return QSerialPort::EvenParity;

            return QSerialPort::UnknownParity;
        }
public slots:
    void setPortName(QString PortName)
    {
        if (m_PortName == PortName)
            return;

        m_PortName = PortName;
        emit PortNameChanged(m_PortName);
    }
    void setBaudrate(int Baudrate)
    {
        if (m_Baudrate == Baudrate)
            return;

        m_Baudrate = Baudrate;
        emit BaudrateChanged(m_Baudrate);
    }

    void setDataBits(int DataBits)
    {
        if (m_DataBits == DataBits)
            return;

        m_DataBits = DataBits;
        emit DataBitsChanged(m_DataBits);
    }

    void setStopBits(int StopBits)
    {
        if (m_StopBits == StopBits)
            return;

        m_StopBits = StopBits;
        emit StopBitsChanged(m_StopBits);
    }

    void setParity(QString Parity)
    {
        if (m_Parity == Parity)
            return;

        m_Parity = Parity;
        emit ParityChanged(m_Parity);
    }

    void setTimeout(int Timeout)
    {
        if (m_Timeout == Timeout)
            return;

        m_Timeout = Timeout;
        emit TimeoutChanged(m_Timeout);
    }

    void setRetries(int Retries)
    {
        if (m_Retries == Retries)
            return;

        m_Retries = Retries;
        emit RetriesChanged(m_Retries);
    }

    void setCycleTime(int CycleTime)
    {
        if (m_CycleTime == CycleTime)
            return;

        m_CycleTime = CycleTime;
        emit CycleTimeChanged(m_CycleTime);
    }

    void setSlaveAddress(int SlaveAddress)
{
    if (m_SlaveAddress == SlaveAddress)
    return;

m_SlaveAddress = SlaveAddress;
emit SlaveAddressChanged(m_SlaveAddress);
}

signals:
    void PortNameChanged(QString PortName);
    void BaudrateChanged(int Baudrate);
    void DataBitsChanged(int DataBits);
    void StopBitsChanged(int StopBits);
    void ParityChanged(QString Parity);
    void TimeoutChanged(int Timeout);
    void RetriesChanged(int Retries);

    // AbstractConfig interface
    void CycleTimeChanged(int CycleTime);

    void SlaveAddressChanged(int SlaveAddress);

public:
    QString ConfigName() override
    {
        return "IOSerialConfig";
    }

    // AbstractConfig interface
public:
    AbstractConfig *CreateDefaultConfig() override;
    int CycleTime() const
    {
        return m_CycleTime;
    }
    int SlaveAddress() const
    {
        return m_SlaveAddress;
    }
};
#endif // IOSERIALCONFIG_H
