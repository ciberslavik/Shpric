//#ifndef MACHINEIO_H
//#define MACHINEIO_H

//#include <QObject>
//#include <QBitArray>
//#include <QThread>
//#include <QTimer>

//#include "master.h"

//typedef enum ControlInputs
//{

//}ControlInputs_t;
//typedef enum MachineFault
//{
//    Vacuum,

//}MachineFault_ts;

//class MachineIO : public QObject
//{
//    Q_OBJECT
//public:
//    explicit MachineIO(QObject *parent = nullptr);

//    void Init(int cycleTime);
//    void DeInit();
//    bool isConnected();
//public slots:
//    ///Pulse swiper millisecond
//    void swiperPulse(int msec);
//    void swiperPulseTimeout();
//    void setDigitalOutput(QBitArray &bits);


//    void statusPrint(QString msg);


//signals:
//    void onPedalToggle(bool state);
//    void onBunkerToggle(bool state);
//    void onVacuumToggle(bool state);
//    void onSwiperToggle(bool state);
//    void onFault();
//    void isStarted(bool *started);
//    void swiperMoveDone();
//    void rawDataReceived(QString data);
//private:
//    //Varibles

//    QBitArray _discreteInputs;
//    u_int16_t _prevDiscreteInputs;
//    QTimer _swiperTimer;
//    QThread _ioThread;
//    u_int8_t _slaveId;
//    bool _is_thread_started;
//    bool _err_status;
//    int _cycle_time;
//    //Functions


//    void parseDiscreteInput(QBitArray inputs);
//    QBitArray bitsFromByte(u_int16_t &data, int size);
//    u_int16_t intFromBits(const QBitArray &data);

//};

//#endif // MACHINEIO_H
