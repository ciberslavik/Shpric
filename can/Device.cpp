
#include "CallbackProcessor.h"
#include "Device.h"
#include "MyMaster.h"
#include <QtCore>

int Device::motionSocketpair[2];

Device::Device(QObject *parent) : QObject(parent)
{
    // Create socket pair
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, motionSocketpair)) {
        qDebug() << "UnixSignalWatcher: socketpair: " << ::strerror(errno);
        return;
    }

    _motionNotifier = new QSocketNotifier(motionSocketpair[1],QSocketNotifier::Read);
    connect(_motionNotifier, &QSocketNotifier::activated, this, &Device::OnMotionDone);

    _motionNotifier->setEnabled(true);


    _od = &MyMaster_Data;
    CallbackProcessor::setPointer(this);
    _isConnected = false;
    _driverHandle = NULL;
    _portHandle = NULL;
}

void Device::Init(DriverType driverType, QString portName)
{
    Q_UNUSED(portName)
    _connData.busname = (char *)"can0"; //    (portName.toStdString().c_str());
    _connData.baudrate = (char *)"250K";

    //Build path to driver library
    QString appDirectory =qApp->applicationDirPath();
    QString drvFile = appDirectory + "/libcanfestival_can_";

    switch (driverType) {
    case DriverType::Virtual:
        drvFile += "virtual";
        break;
    case DriverType::Socket:
        drvFile += "socket";
        break;
    }
    drvFile += ".so";
    //Initialize can timers
    TimerInit();

    //load driver library
    LIB_HANDLE driver = LoadCanDriver(drvFile.toLocal8Bit());
    if(driver == NULL)
    {
        qDebug()<<"Failed open CAN driver " + drvFile;
        return;
    }
    _driverHandle = driver;
    _isConnected = true;
    setNodeId(_od, 4);
}

void Device::DeInit()
{
    StopTimerLoop(&CallbackProcessor::StopTimer);
}

void Device::Start()
{
    //open can port
    CAN_PORT port;
    port = canOpen(&_connData, _od);
    if(!port)
    {
        qDebug()<< "Error open port: " + QString(_connData.busname);
        return;
    }
    _portHandle = port;
    StartTimerLoop(CallbackProcessor::InitNode);
}

void Device::Stop()
{
    setState(_od, Stopped);

    //StopTimerLoop()
}

void Device::setSyncCyclePeriod(unsigned int period)
{
    *_od->Sync_Cycle_Period = period;
}

bool Device::getChargeRelay()
{
    u_int32_t dataSize = 4;
    u_int32_t data = 0;

    ReadSDOSyncronus(CHARGE_RELAY,0, &data, &dataSize);

    return data == 1 ? true : false;
}

u_int16_t Device::getStatusWord()
{
    u_int32_t dataSize = 4;
    u_int32_t data = 0;

    ReadSDOSyncronus(STATUS_WORD ,0, &data, &dataSize);
    return data;
}

void Device::setControlWord(u_int16_t controlWord)
{
    WriteSDOSynchronus(CONTROL_WORD, 0, &controlWord, 2);
}

void Device::setTargetPosition(INTEGER32 targetPosition)
{
    qDebug()<< "Target:" << targetPosition;
    WriteSDOSynchronus(TARGET_POSITION, 0, &targetPosition, 4);
}

void Device::setVFactor(int16_t vFactor)
{
    INTEGER16 data = static_cast<INTEGER16>(vFactor);
    data = data * 10;
    qDebug()<< "Set velocity:" << data;
    WriteSDOSynchronus(VELOCITY_FACTOR, 0, &data, 2);
}

void Device::setMotionCommand(MotionCommand command, MotionDirection dir)
{
    UNS8 cmdByte = static_cast<UNS8>(command);
    cmdByte |= static_cast<UNS8>(dir);
    qDebug()<< "Motion command:"<< cmdByte;
    WriteSDOSynchronus(MOTION_COMMAND, 0, &cmdByte, 1);
}

void Device::setSlaveState(NMTState state)
{
    switch (state) {
    case NMTState::StartNode:
        masterSendNMTstateChange(_od,_slaveId,NMT_Start_Node);
        break;
    case NMTState::StopNode:
        masterSendNMTstateChange(_od,_slaveId,NMT_Stop_Node);
        break;
    case NMTState::ResetNode:
        masterSendNMTstateChange(_od,_slaveId,NMT_Reset_Node);
        break;
    case NMTState::EnterPreOpertional:
        masterSendNMTstateChange(_od,_slaveId,NMT_Enter_PreOperational);
        break;
    case NMTState::ResetCommunication:
        masterSendNMTstateChange(_od,_slaveId,NMT_Reset_Comunication);
        break;
    }
}

void Device::setExecute()
{
    qDebug()<< "set Execute";
    UNS16 data = 0x0001;
    WriteSDOSynchronus(POSI_CONTROL_WORD, 0, &data, 2);

    //_isMoving = true;
    //QThread::usleep(100);
    //    data = 0x0001;
    //WriteSDOSynchronus(POSI_CONTROL_WORD, 0, &data, 2);
}

void Device::resetExecute()
{
    qDebug()<< "reset Execute";
    UNS16 data = 0x0000;
    WriteSDOSynchronus(POSI_CONTROL_WORD, 0, &data, 2);
}

void Device::OnMotionDone(int sockfd)
{
    int plc;
    unsigned long nBytes = ::read(sockfd, &plc, sizeof(plc));
    Q_UNUSED(nBytes)
    //_plcLocker.lock();
    _plcState = plc & 0x0F;
    if(_plcState != _oldPlcState)
    {
        if((_oldPlcState==3) && (_plcState == 2 || _plcState == 1))
        {
            // UNS16 data = 0x0000;
            // WriteSDOSynchronus(POSI_CONTROL_WORD, 0, &data, 2);
            emit MotionDone();
        }

        _oldPlcState = _plcState;
    }
    //_plcLocker.unlock();
    //qDebug()<<"OnMotionDone:"<<_plcState<<":"<<nBytes;

}

void Device::WriteSDOSynchronus(u_int16_t index, u_int8_t subIndex, void *data, u_int32_t size)
{
    u_int32_t abortCode;
    u_int8_t res;
    u_int8_t err = writeNetworkDict(_od, _slaveId, index, subIndex, size, 0, data, 0);

    if (err)
    {
        qDebug("error read dict Slave ID:%2.2x, index:%4.4x, sub index:%2.2x",_slaveId,  index, subIndex);
        emit CommunicationError(0);
        return;
    }
    for(;;)
    {
        res = getWriteResultNetworkDict(_od, _slaveId, &abortCode);

        QThread::yieldCurrentThread();

        if (res != SDO_DOWNLOAD_IN_PROGRESS)
            break;

        continue;
    }
    if(abortCode)
        emit CommunicationError(abortCode);

    closeSDOtransfer(_od, _slaveId, SDO_CLIENT);
}

void Device::ReadSDOSyncronus(u_int16_t index, u_int8_t subIndex, void *data, u_int32_t *size)
{
    UNS32 abortCode = 0;
    UNS8 res = SDO_UPLOAD_IN_PROGRESS;
    // Read SDO
    //EnterMutex();
    UNS8 err = readNetworkDict (_od, _slaveId, index, subIndex, 0, 0);
    //LeaveMutex();
    if (err)
    {
        qDebug()<<"error read dict" << err;
        emit CommunicationError(0);
        return;
    }
    for(;;)
    {
        res = getReadResultNetworkDict (_od, _slaveId, data, size, &abortCode);
        //qDebug() << "Read result:"<<res;
        QThread::yieldCurrentThread();

        if (res == SDO_FINISHED)
            break;

        continue;
    }
    if(abortCode)
        qDebug()<< "SDO Abort code:" << abortCode;// emit CommunicationError(abortCode);


    closeSDOtransfer(_od, _slaveId, SDO_CLIENT);

}


void Device::OnHeartbeatError(CO_Data*, u_int8_t)
{

}

void Device::OnInitialisation(CO_Data *d)
{
    qDebug()<<"Master Initialization";
    UNS32 rxPDO1_COBID = 0x0180 + _slaveId;
    UNS32 size = sizeof(UNS32);

    //Set RPDO1 COB ID
    writeLocalDict( d, /*CO_Data* d*/
                    0x1400, /*UNS16 index*/
                    0x01, /*UNS8 subind*/
                    &rxPDO1_COBID, /*void * pSourceData,*/
                    &size, /* UNS8 * pExpectedSize*/
                    RW);  /* UNS8 checkAccess */
    emit DeviceInitialization();
    //setState(d, e_nodeState::Pre_operational);
}

void Device::OnPreOperational(CO_Data*)
{
    qDebug()<<"Master Pre operational";
    emit DevicePreOperational();
    setState(_od, Operational);
}

void Device::OnOperational(CO_Data *d)
{
    Q_UNUSED(d)
    qDebug() << "Master Operational";
    emit DeviceOperational();
}

void Device::OnStopped(CO_Data *d)
{
    Q_UNUSED(d)
    qDebug() << "Master Stopped";
}

void Device::OnPostSync(CO_Data *d)
{
    Q_UNUSED(d)
}

void Device::OnPostTPDO(CO_Data *)
{

}

void Device::OnPostEMCY(CO_Data*, u_int8_t nodeID, u_int16_t errCode, u_int8_t errReg)
{
    qDebug("Master received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\n", nodeID, errCode, errReg);
}

void Device::OnPostSlaveBootup(CO_Data*, u_int8_t)
{
    qDebug()<<"Post Slave boot-up";
}

void Device::OnPostSlaveStateChanged(CO_Data *d, u_int8_t nodeId, e_nodeState newNodeState)
{
    Q_UNUSED(d)
    qDebug("Slave state change slave id:%2.2x to state:0x%X", nodeId, newNodeState);
}

void Device::OnInitNode(CO_Data *d, u_int32_t id)
{
    Q_UNUSED(d)
    Q_UNUSED(id)
    qDebug()<<"Init node";
    RegisterSetODentryCallBack(_od, STATUS_WORD, 0, &CallbackProcessor::StatusWordUpdate);
    RegisterSetODentryCallBack(_od, MOTION_STATUS_BYTE, 0, &CallbackProcessor::MotionStatusByteUpdate);
    setNodeId(_od,2);
    setState(_od, Initialisation);
}


void Device::OnStopTimer(CO_Data *d, u_int32_t id)
{
    Q_UNUSED(d)
    Q_UNUSED(id)
    qDebug()<<"Stop timers";
}

u_int32_t Device::OnStatusWordUpdate(CO_Data *d, const indextable *unsused_indextable, u_int8_t unsused_bSubindex)
{
    Q_UNUSED(d)
    Q_UNUSED(unsused_indextable)
    Q_UNUSED(unsused_bSubindex)
#ifdef DEBUG_CONSOLE_ON
    qDebug("Status word upd");
#endif
    if(_oldStatusWord != Statusword)
    {
        //qDebug("Status word:0x%X",Statusword);
        parseStatusWord(Statusword);
        _oldStatusWord = Statusword;
    }


    return 0x00000000;
}

u_int32_t Device::OnMotionStatusByteUpdate(CO_Data *d, const indextable *unsused_indextable, u_int8_t unsused_bSubindex)
{
    Q_UNUSED(d)
    Q_UNUSED(unsused_indextable)
    Q_UNUSED(unsused_bSubindex)
#ifdef DEBUG_CONSOLE_WAR
    qDebug("Motion status byte upd");
#endif

    unsigned long nBytes = ::write(motionSocketpair[0], &I201MotionStatusByte, sizeof(I201MotionStatusByte));
    Q_UNUSED(nBytes);
    return 0x00000000;
}


void Device::parseStatusWord(u_int16_t data)
{
    QBitArray bits;
    bits = bitsFromByte(&data,16);

    _statusWord.ReadyToSwitchOn     = bits[0];
    _statusWord.SwitchedOn          = bits[1];
    _statusWord.OpEnabled           = bits[2];

    if(!_statusWord.Fault && bits[3])
    {
        qDebug()<<"Inverter FAULT";
        _statusWord.Fault = bits[3];
        emit Fault();
    }
    else
    {
        _statusWord.Fault = bits[3];
    }
    _statusWord.VoltageEnabed       = bits[4];
    _statusWord.QuickStop           = bits[5];
    _statusWord.SwitchOnDisable     = bits[6];
    _statusWord.Warning             = bits[7];
    _statusWord.Message             = bits[8];
    if(!_statusWord.TargetReached && bits[10])
    {
        qDebug()<<"Motion DONE!!!!";
        _statusWord.TargetReached = bits[10];
        //emit MoveDone();
    }
    else
    {
        _statusWord.TargetReached = bits[10];
    }
}

QBitArray Device::bitsFromByte(u_int16_t *data, int size)
{
    QBitArray bits(size);
    qint16 mask = 0x0001;
    for(int i = 0;i<size;i++)
    {
        bits[i]=(*data & mask ? true : false);
        mask = mask << 1;
    }
    return bits;
}
