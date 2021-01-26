#ifndef IOMODBUS_H
#define IOMODBUS_H

#include "iobase.h"
#include "iomodbusworker.h"

#include <QObject>

class IOModbus : public IOBase
{
    Q_OBJECT

public:
    explicit IOModbus(SerialConfig *serialConfig, QObject *parent = nullptr);
    void Init(int cycleTime) override;
    void Start() override;
    void Stop() override;
    bool Vacuum() override;

public slots:
    void SwiperMove(int moveTime) override;
    void VacuumOn(bool state) override;
    void FCPowerOn(bool state) override;
    void FCEnable(bool state) override;
    QBitArray getOutputs() override;
    void setOutputs(const QBitArray &outputs) override;
    void setVacuum(double value) override;
    void SwiperOn(bool state) override;
private slots:
    void onReciveStatus(DevState state);
    void onInputDataReady(QBitArray inputs, quint16 analog);
signals:
    void writeOutput(QBitArray states);
    void writePWM(double value);
    void writePulse(int value);
    void writePulseOn(bool state);
    void stopWorker();
private:
    IOModbusWorker *_worker;
    WorkerConfig _workerConfig;
    QTimer _swiperTimer;
    QBitArray _discreteInputs;
    bool _isInit;
    bool _isRun;
    double _aIN;
    int _ainCnt;
    void setOutput(int output, bool value);   



};

#endif // IOMODBUS_H
