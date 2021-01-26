#include    "master.h"

#include    <QModbusRtuSerialMaster>
#include    <QVariant>
#include    <QDebug>
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Master::Master(QObject *parent) : QObject(parent)
{
    is_connected = false;
    modbusDevice = Q_NULLPTR;

    modbusDevice = new QModbusRtuSerialMaster(this);

    connect(modbusDevice, &QModbusClient::stateChanged,
            this, &Master::stateChanged);

    connect(modbusDevice, &QModbusClient::errorOccurred,
            this, &Master::errorModbus);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Master::~Master()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool Master::init(SerialConfig *config)
{
    // Check modbus device is created
    if (modbusDevice == Q_NULLPTR)
        return false;

    // Set serial port settings (and other connection settings)
    if (modbusDevice->state() != QModbusDevice::ConnectedState)
    {
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                             config->PortName());

        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                             config->Baudrate());

        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                             config->DataBits());

        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                             config->StopBits());

        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                             config->getPariry());

        modbusDevice->setTimeout(config->Timeout());
        modbusDevice->setNumberOfRetries(config->Retries());
    }
    else
        return false;

    return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool Master::isConnected() const
{
    return is_connected;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::writeCoils(write_request_t *request)
{
    // Data unit creation
    QModbusDataUnit dataUnit(QModbusDataUnit::Coils,
                             request->address,
                             request->count);

   // Fill all required data
   for (int i = 0; i < request->count; i++)
       dataUnit.setValue(i, request->data[i]);

    sendWriteRequest(dataUnit, request->id);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::readInputRegisters(read_request_t *request)
{
    QModbusDataUnit dataUnit(QModbusDataUnit::InputRegisters,
                             request->address,
                             request->count);

    sendReadRequest(dataUnit, request->id);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::readDiscreteInputs(read_request_t *request)
{
    QModbusDataUnit dataUnit(QModbusDataUnit::DiscreteInputs,
                             request->address,
                             request->count);

    sendReadRequest(dataUnit, request->id);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::readHoldingRegisters(read_request_t *request)
{
    QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters,
                             request->address,
                             request->count);

    sendReadRequest(dataUnit, request->id);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::readCoils(read_request_t *request)
{
    QModbusDataUnit dataUnit(QModbusDataUnit::Coils,
                             request->address,
                             request->count);

    sendReadRequest(dataUnit, request->id);
}

//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::writeHoldingRegisters(write_request_t *request)
{
    QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters,
                             request->address,
                             request->count);

    for (int i = 0; i < request->count; i++)
        dataUnit.setValue(i, request->data[i]);

    sendWriteRequest(dataUnit, request->id);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::sendReadRequest(QModbusDataUnit dataUnit, quint8 id)
{
    QModbusReply *reply = modbusDevice->sendReadRequest(dataUnit, id);

    if (reply != nullptr)
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished,
                    this, &Master::onRecieved);

            connect(reply, &QModbusReply::errorOccurred,
                    this, &Master::errorModbus);
        }
        else
            reply->deleteLater();
    }
    else
    {
        statusPrint("ERROR: Master send data error");
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::sendWriteRequest(QModbusDataUnit dataUnit, quint8 id)
{
    // Send reply
    QModbusReply *reply = modbusDevice->sendWriteRequest(dataUnit, id);

    if (reply != nullptr)
    {
         if (!reply->isFinished())
         {
             connect(reply, &QModbusReply::finished,
                     this, &Master::onWrited);

             connect(reply, &QModbusReply::errorOccurred,
                     this, &Master::errorModbus);
         }
         else
             reply->deleteLater();
    }
    else
    {
        statusPrint("ERROR: Master send data error");
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
answer_request_t Master::getSlaveAnswer(QModbusReply *reply)
{
    QModbusDataUnit dataUnit = reply->result();

    answer_request_t answer;
    answer.id = static_cast<quint8>(reply->serverAddress());

    answer.count = static_cast<quint16>(dataUnit.valueCount());
    answer.address = static_cast<quint16>(dataUnit.startAddress());

    QModbusResponse respose = reply->rawResult();

    answer.func = respose.functionCode();

    answer.rawPduData = respose.data();

    for (int i = 0; i < answer.count; i++)
    {
       answer.data[i] = dataUnit.value(i);
    }

    return answer;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QModbusReply *Master::getSlaveReply()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());

    // Check by empty reply
    if (reply == nullptr)
    {
        statusPrint("ERROR: empty reply");
        return nullptr;
    }

    // Check by exception
    if (reply->rawResult().isException())
    {
        statusPrint("ERROR: Reply exception " + modbusDevice->errorString());
    }

    // Check by invalid PDU in reply
    if (reply->rawResult().isValid())
    {
        return reply;
    }
    else
    {
        statusPrint("ERROR: Invalid PDU " + modbusDevice->errorString());
        return nullptr;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool Master::openConnection()
{
    return modbusDevice->connectDevice();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::closeConnection()
{
    if(isConnected())
        modbusDevice->disconnectDevice();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::sendRequest(abstract_request_t *request)
{
    switch (request->func)
    {
    // Write single coil
    case QModbusPdu::WriteSingleCoil:
        {
            write_request_t *w_req = static_cast<write_request_t *>(request);
            w_req->count = 1;
            writeCoils(w_req);

            break;
        }

    // Write multyple coils
    case QModbusPdu::WriteMultipleCoils:
        {
            write_request_t *w_req = static_cast<write_request_t *>(request);
            writeCoils(w_req);

            break;
        }
    // Read input registers
    case QModbusPdu::ReadInputRegisters:
        {
            read_request_t *r_req = static_cast<read_request_t *>(request);
            readInputRegisters(r_req);

            break;
        }
    // Read discrete inputs
    case QModbusPdu::ReadDiscreteInputs:
        {
            read_request_t *r_req = static_cast<read_request_t *>(request);
            readDiscreteInputs(r_req);

            break;
        }

    // Write holding register
    case QModbusPdu::WriteSingleRegister:
        {
            write_request_t *w_req = static_cast<write_request_t *>(request);
            w_req->count = 1;
            writeHoldingRegisters(w_req);

            break;
        }

    // Write multiple coils
    case QModbusPdu::WriteMultipleRegisters:
        {
            write_request_t *w_req = static_cast<write_request_t *>(request);
            writeHoldingRegisters(w_req);

            break;
        }
    // Read holding registers
    case QModbusPdu::ReadHoldingRegisters:
        {
            read_request_t *r_req = static_cast<read_request_t *>(request);
            readHoldingRegisters(r_req);

            break;
        }

    // Read coils
    case QModbusPdu::ReadCoils:
        {
            read_request_t *r_req = static_cast<read_request_t *>(request);
            readCoils(r_req);

            break;
        }

    default:

        break;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::errorModbus(QModbusDevice::Error error)
{
    emit onError();
    switch (error)
    {
    case QModbusDevice::NoError:
        statusPrint(modbusDevice->errorString());
        break;

    case QModbusDevice::ReadError:
        qDebug("Read error");
        statusPrint(modbusDevice->errorString());
        break;

    case QModbusDevice::WriteError:
        qDebug("Write error");
        statusPrint(modbusDevice->errorString());
        break;

    case QModbusDevice::ConnectionError:
        qDebug("Connection error");
        qDebug() << modbusDevice->errorString();
        statusPrint(modbusDevice->errorString());
        break;

    case QModbusDevice::ConfigurationError:
        qDebug("Configuration error");
        statusPrint(modbusDevice->errorString());
        break;

    case QModbusDevice::ProtocolError:
        qDebug("Protocol error");
        statusPrint(modbusDevice->errorString());
        break;

    case QModbusDevice::TimeoutError:
        qDebug("Timeout error");
        emit timeoutError();

        statusPrint(modbusDevice->errorString());
        break;

    case QModbusDevice::UnknownError:
        qDebug("Unknown error");
        statusPrint(modbusDevice->errorString());
        break;

    default:

        break;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::stateChanged(QModbusDevice::State state)
{
    switch (state)
    {
    case QModbusDevice::ConnectedState:

        is_connected = true;
        statusPrint("Device is connected...OK");

        break;

    case QModbusDevice::UnconnectedState:

        is_connected = false;
        statusPrint("Device is disconnected...OK");

        break;

    default:

        is_connected = false;

        break;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::onWrited()
{
    QModbusReply *reply = getSlaveReply();

    if (reply != nullptr)
        emit sendAnswer(getSlaveAnswer(reply));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Master::onRecieved()
{
    QModbusReply *reply = getSlaveReply();

    if (reply != nullptr)
        emit sendAnswer(getSlaveAnswer(reply));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
RequestType getRequestType(quint8 func)
{
    switch (func)
    {
    case QModbusPdu::WriteSingleCoil:
    case QModbusPdu::WriteSingleRegister:
    case QModbusPdu::WriteMultipleCoils:
    case QModbusPdu::WriteMultipleRegisters:

        return REQ_WRITE;

    case QModbusPdu::ReadCoils:
    case QModbusPdu::ReadDiscreteInputs:
    case QModbusPdu::ReadInputRegisters:
    case QModbusPdu::ReadHoldingRegisters:

        return REQ_READ;

    default:

        break;
    }

    return REQ_UNKNOWN;
}
