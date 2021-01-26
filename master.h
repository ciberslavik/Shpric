#ifndef MASTER_H
#define MASTER_H

#include    <QObject>
#include    <QModbusClient>
#include    <QMetaType>
#include <QModbusRtuSerialMaster>
//#include    "serial-config.h"
#include    "request.h"
#include <QSerialPort>

#include <setting/ioserialconfig.h>
/*!
 * \enum
 * \brief Modbus functions codes
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum
{
    MB_FUNC_NONE = 0x00,
    MB_FUNC_READ_COILS = 0x01,
    MB_FUNC_READ_DISCRETE_INPUT = 0x02,
    MB_FUNC_READ_HOLDING_REGISTERS = 0x03,
    MB_FUNC_READ_INPUT_REGISTERS = 0x04,
    MB_FUNC_WRITE_COIL = 0x05,
    MB_FUNC_WRITE_HOLDING_REGISTER = 0x06,
    MB_FUNC_WRITE_MULTIPLE_COILS = 0x0F,
    MB_FUNC_WRITE_MULTIPLE_REGISTERS = 0x10
};

/*!
 * \class
 * \brief Virtual master device
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class Master : public QObject
{
    Q_OBJECT

public:

    explicit Master(QObject *parent = Q_NULLPTR);

    virtual ~Master();

    /// Master initialization
    bool init(SerialConfig *serialConfig);

    /// Check connection state
    bool isConnected() const;

signals:

    /// Print master status
    void statusPrint(QString msg);

    /// Send answer to main window
    void sendAnswer(answer_request_t answer);

    /// Send raw data to main window
    void sendRawData(QByteArray rawData);

    ///Master modbus error
    void onError();
    void timeoutError();
protected:

    /// Modbus device object
    QModbusRtuSerialMaster   *modbusDevice;

    /// Connection flag
    bool is_connected;

    void writeCoils(write_request_t *request);

    void readInputRegisters(read_request_t *request);

    void readDiscreteInputs(read_request_t *request);

    void readHoldingRegisters(read_request_t *request);

    void readCoils(read_request_t *request);

    void writeHoldingRegisters(write_request_t *request);

    void sendReadRequest(QModbusDataUnit dataUnit, quint8 id);

    void sendWriteRequest(QModbusDataUnit dataUnit, quint8 id);

    answer_request_t getSlaveAnswer(QModbusReply *reply);

    QModbusReply *getSlaveReply();

public slots:

    /// Open connection throw serial port
    bool openConnection();

    /// Close connection
    void closeConnection();

    /// Send request
    void sendRequest(abstract_request_t *request);

private slots:

    /// Error processing
    void errorModbus(QModbusDevice::Error error);

    /// State processing
    void stateChanged(QModbusDevice::State state);

    /// Data write event
    void onWrited();

    /// Date recieve event
    void onRecieved();
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
RequestType getRequestType(quint8 func);

#endif // MASTER_H
