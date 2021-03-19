#ifndef SERVODATA_H
#define SERVODATA_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>

class ServoData : public QObject
{
    Q_OBJECT
public:
    explicit ServoData(QObject *parent = nullptr);
    uint8_t getPLCOpenState()
    {
        return _plcOpenState;
    }
    uint8_t getMotionStatusByte()
    {
        return _motionStatusByte;
    }
    uint16_t getStatusWord()
    {
        return _statusWord;
    }
public slots:
    void setPLCOpenState(uint8_t value)
    {
        _plcOpenState = value;
    }
    void setMotionStatusByte(uint8_t value)
    {
        _motionStatusByte = value;
    }

    void setStatusWord(uint16_t value)
    {
        _statusWord = value;
    }
signals:

private:
    QMutex writeMutex;
    QMutex readMutex;

    uint8_t _plcOpenState;
    uint8_t _motionStatusByte;
    uint16_t _statusWord;
};

#endif // SERVODATA_H
