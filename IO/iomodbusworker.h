#ifndef IOMODBUSWORKER_H
#define IOMODBUSWORKER_H

#include "PIRegulator.h"
#include "iodevstate.h"
#include "ioworkerconfig.h"
#include "master.h"
#include "request.h"
#include "unistd.h"
#include <QObject>
#include <QBitArray>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>
#include <signal.h>
#include <QList>
#include <QQueue>
#include <sys/time.h>

class IOModbusWorker : public QThread
{
    Q_OBJECT
    enum
    {
        REG_DIGITAL_INPUT   =1000,
        REG_ANALOG_INPUT    =1001,

        REG_DIGITAL_OUTPUT  =1000,
        REG_PULSE_WIDTH     =1003,
        REG_PULSE_ON        =1005,
        REG_PWM_EN          =1001,
        REG_PWM_VAL         =1002
    };

public:
    explicit IOModbusWorker(QObject *parent = nullptr);
    void Init(WorkerConfig *config);
    QBitArray &getDigitalOutputs();
protected:
    void run() override;
public slots:
    void writeOutput(QBitArray values);
    void writePWM(double value);
    void writePulse(quint16 value);
    void pulseOn(bool state);

    void Process();
    void stopWorker();
private slots:
    void slaveAnswer(answer_request_t answer);

    /// Put raw data to raw data log
    void rawDataReceive(QByteArray rawData);

    void modbusError();
    void onTimeoutError();
    void onSwiperTimeout();
signals:
    void InputDataReady(QBitArray inputs,quint16 analog);
    void statusPrint(QString msg);
    void rawDatareceived(QString data);
    void devStateRecived(DevState state);
    void SwiperMoveDone();
    //void discreteInputChanged(QBitArray inputs);
private:
    bool _is_running;
    bool _is_connected;
    bool _is_proccessingRequest;
    bool _is_write_need;
    bool _write_work;
    bool _is_first_cycle;
    bool _checkingPulseCNT;
    bool _swiperProcess;

    bool _pwmEnabled, _pulseEnabled;
    int _cycleCounter;
    int _pulseWidth;
    Master *_master;
    QBitArray _discreteOutputs;
    QBitArray _discreteInputs;

    float _analog;
    unsigned short _prevDiscreteInputs;
    QTimer *_cycleTimer;

    QQueue<abstract_request_t*> *_requestBuf;


    WorkerConfig *_workerConf;

    void ProcessQueue();
    void EnqueueRequest(abstract_request_t *request);

//    abstract_request_t *getDigitalReadRequest();
//    abstract_request_t *getAnalogReadRequest();
//    abstract_request_t *getStatusReadRequest();
//    abstract_request_t *getStatusWriteRequest(DevState status);

    abstract_request_t *getWriteRegRequest16(quint16 addr, quint16 value);
    abstract_request_t *getReadInputRegRequest(quint16 addr, quint16 count);
//    abstract_request_t *getWriteRegRequest32(quint16 addr, quint32 value);
//    abstract_request_t *getReadRegRequest(quint16 addr,quint16 count);



    quint16 intFromBits(QBitArray *bits);
    QBitArray bitsFromByte(quint16 &data);
    PIRegulator *_pid;
    double _vacuum_setpoint, _vacuum_value;
    qint16 _vacuum_pwm;
};

#endif // IOMODBUSWORKER_H
