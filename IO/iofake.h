#ifndef IOFAKE_H
#define IOFAKE_H

#include "iobase.h"

#include <QTimer>
#include <QDebug>


class IOFake : public IOBase
{
    Q_OBJECT
public:
    IOFake();

    // IOBase interface
public:
    void Init(int cycleTime) override;
    void Start() override;
    void Stop() override;
    bool Vacuum() override;
public slots:
    void SwiperMove(int moveTime) override;
    void VacuumOn(bool state) override;
    void FCPowerOn(bool state) override;
    void FCEnable(bool state) override;
private slots:
    void cyclicTimeout();
    void swiperTimeout();
private:
    int _cycleTime;
    QTimer _cycleTimer;
    QTimer _swiperTimer;

    // IOBase interface
public slots:
    QBitArray getOutputs() override;
    void setOutputs(const QBitArray &outputs) override;


    // IOBase interface
public slots:
    void setVacuum(double value) override;

    // IOBase interface
public slots:
    void SwiperOn(bool state) override;
};

#endif // IOFAKE_H
