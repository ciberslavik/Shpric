#include "servocontroller.h"
#include <QDebug>
#include <QThread>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//ServoController *ServoController::_instance;
bool waiting_answer;
const QString ServoController::deviceStateStrings[] = {"Self test",
                                                       "Switch-on disable",
                                                       "Ready for switch-on",
                                                       "Switched on",
                                                       "Enabled",
                                                       "Fault",
                                                       "Fault reaction",
                                                       "Quick stop"};
LIB_HANDLE ServoController::drvHandle;
void ServoController::Connect()
{
    if(_slaveId==0)
    {
        qDebug("Error: SlaveId is null. Cannot connect!");
        return;
    }

    TimerInit();
    if(LoadCanDriver("/opt/master/bin/libcanfestival_can_socket.so")==NULL)
    {
        qDebug()<< "error load can driver";
        return;
    }
    if(!canOpen(&MasterBoard, &MyMaster_Data))
    {
        qDebug()<< "Cannot open can0\r\n";
        return;
    }
    StartTimerLoop(&InitNode);
    qDebug("Connected!");

}

void ServoController::setSlaveId(u_int8_t slaveId)
{
    if(_slaveId != slaveId)
        _slaveId = slaveId;
}

u_int8_t ServoController::SlaveId(){return _slaveId;}

StatusWord_t ServoController::getStatusWord(){return _statusWord;}

void ServoController::CanDummyConnect()
{
    TimerInit();
    LIB_HANDLE drvHandle =LoadCanDriver("/opt/master/bin/libcanfestival_can_socket.so");
    if(drvHandle==NULL)
    {
        qDebug()<< "error load can driver";
        return;
    }
    if(!canOpen(&MasterBoard, &MyMaster_Data))
    {
        qDebug()<< "Cannot open can0\r\n";
        return;
    }
    StartTimerLoop(&DummyInit);

}

void ServoController::EnableOperation()
{
    UNS32 data;
    UNS32 size=4;
    qDebug()<<"Begin enable controller";// << _masterSyncCount;

    //    qDebug()<<"Logic power on";
    QBitArray outs = _io->getOutputs();

    //QThread::sleep(3);

    //setState(&MyMaster_Data, Operational);
    //qDebug()<<getState(&MyMaster_Data);
    qDebug()<<"High power on";//<< _masterSyncCount;
    outs = _io->getOutputs();
    outs[IOBase::Outputs::FC_POWER] = true;
    _io->setOutputs(outs);
    //QThread::sleep(2);

    qDebug()<<"Wait charge relay";//<< _masterSyncCount;
    do
    {
        ReadSDO(0x01,0x280E,0,0,&data,&size);
        qDebug()<<"Read E14:" << data <<"-";//<< _masterSyncCount;
    }while(data != 1);

    outs = _io->getOutputs();
    outs[IOBase::Outputs::FC_ENABLE] = true;
    _io->setOutputs(outs);

    UNS32 abortCode;
    data = 0x0006;
    UNS16 data16 = 0x0006;
    qDebug()<<"Write control word 0x06 - ";//<< _masterSyncCount;
    writeNetworkDict(_co_data, _slaveId, ControlWord, 0, 2, 0, &data16, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);


    size = 4;
    do
    {
        ReadSDO(0x01, 0x6041, 0, 0, &data, &size);
        qDebug()<<"Status word:" << data <<"_";//<< _masterSyncCount;
    }while((data & 0x01)!=0x01);

    //    while(!_statusWord.ReadyToSwitchOn)
    //    {
    //        QThread::msleep(3);
    //    }
    qDebug()<<"Ready to switch on";
    qDebug()<<"Write control word 0F - ";//<< _masterSyncCount;
    data16 = 0x000F;
    writeNetworkDict(_co_data, _slaveId, ControlWord, 0, 2, 0, &data16, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    while(!_statusWord.OpEnabled)
    {
        QThread::msleep(3);
    }

    //while(!(data & 0x02));
    qDebug()<< "Operation enabled";
    qDebug()<<"Homing";

    qDebug()<<"Execute bit 0 - ";//<< _masterSyncCount;
    data16 = 0x0000;                                                  //Execute bit
    writeNetworkDict(_co_data, _slaveId, PositionControlWord, 0, 2, 0, &data16, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    qDebug()<<"MC_Home command - ";// << _masterSyncCount;
    UNS8 data8 = MC_Home;
    writeNetworkDict(_co_data, _slaveId, MotionCommandByte, 0, 1, 0, &data8, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    qDebug()<<"Execute bit 1 - ";// << _masterSyncCount;
    data16 = 0x0001;                                                  //Execute bit
    writeNetworkDict(_co_data, _slaveId, PositionControlWord, 0, 2, 0, &data16, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);


}

void ServoController::setSpeed(int speed)
{
    //INTEGER16 velData = ((SHRT_MAX / 400)*speed)-1;

    UNS32 abortCode=0;
    INTEGER16 velData = static_cast<INTEGER16>(speed);
    velData = velData * 10;
    qDebug()<<"Send V.factor:" << speed << "("<<velData<<")";
    UNS8 res = writeNetworkDict(_co_data, _slaveId, 0x30d7, 0, 2, 0, &velData, 0);
    while (getWriteResultNetworkDict(_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    qDebug()<<"Result:"<<res << " AbortCode:"<< abortCode;
}

void ServoController::Move(int target, MoveDirection direction)
{
    // if(_is_moving)
    //   return;

    UNS32 abortCode;
    INTEGER32 data = 0x0000;
    INTEGER32 targetPos = 0x0000;
    //UNS32 size=4;

    writeNetworkDict(_co_data, _slaveId, PositionControlWord, 0, 4, 0, &data, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);
    if(direction == MoveDirection::Forward)
    {
        data = MC_MoveAdditive | MC_ForwardDir;
        targetPos = target;
        qDebug()<<"Moving:"<<data;
    }
    else if(direction == MoveDirection::Reverse)
    {
        data = MC_MoveAdditive | MC_ReverseDir;
        targetPos = target * -1;
        qDebug()<<"Moving:"<<data;
    }
    writeNetworkDict(_co_data, _slaveId, MotionCommandByte, 0, 4, 0, &data, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    data = target;
    writeNetworkDict(_co_data, _slaveId, TargetPosition, 0, 4, 0, &targetPos, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    data = 0x0001;
    writeNetworkDict(_co_data, _slaveId, PositionControlWord, 0, 4, 0, &data, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    _is_moving = true;

}

void ServoController::Home()
{

}

ServoController::ServoController(IOBase *io, QObject *parent):
    ControllerBase(parent),
    _io(io)
{
    MasterBoard.busname = (char *)"0";
    MasterBoard.baudrate = (char *)"";
    //callback`s
    MyMaster_Data.heartbeatError = heartbeatError;
    MyMaster_Data.preOperational = preOperational;
    MyMaster_Data.operational = operational;
    MyMaster_Data.initialisation = initialisation;
    MyMaster_Data.stopped = stopped;
    MyMaster_Data.post_sync = post_sync;
    MyMaster_Data.post_TPDO = post_TPDO;
    MyMaster_Data.post_emcy = post_emcy;
    MyMaster_Data.post_SlaveBootup = post_SlaveBootup;
    MyMaster_Data.post_SlaveStateChange = post_SlaveStateChanged;
    *MyMaster_Data.Sync_Cycle_Period = 8;
    _co_data = &MyMaster_Data;
    ControllerBase::setInstance(this);
    _masterSyncCount=0;
}

void ServoController::heartbeatError(CO_Data *d, u_int8_t)
{

}

void ServoController::initialisation(CO_Data *d)
{
    qDebug()<<"Master Initialization";
    UNS32 rxPDO1_COBID = 0x0180 + 1;
    UNS32 txPDO2_COBID = 0x0200 + 1;
    UNS32 size = sizeof(UNS32);

    writeLocalDict( d, /*CO_Data* d*/
                    0x1400, /*UNS16 index*/
                    0x01, /*UNS8 subind*/
                    &rxPDO1_COBID, /*void * pSourceData,*/
                    &size, /* UNS8 * pExpectedSize*/
                    RW);  /* UNS8 checkAccess */


    /*****************************************
     * Define TPDOs to match slave ID=0x40 RPDOs*
     *****************************************/
    writeLocalDict( d, /*CO_Data* d*/
                    0x1800, /*UNS16 index*/
                    0x01, /*UNS8 subind*/
                    &txPDO2_COBID, /*void * pSourceData,*/
                    &size, /* UNS8 * pExpectedSize*/
                    RW);  /* UNS8 checkAccess */

    //setState(&MyMaster_Data, Pre_operational);
}

void ServoController::preOperational(CO_Data *d)
{
    qDebug()<<"Master preOperational";
    ServoController* controller = static_cast<ServoController*>(ControllerBase::Instance());
    controller->_configureSlaveStep=0;
    controller->ConfigureSlave(d,1);
    //masterSendNMTstateChange(controller->_co_data,controller->_slaveId,NMT_Reset_Node);
}

void ServoController::operational(CO_Data *d)
{
    qDebug("Master operational mode");
    static_cast<ServoController*>(ControllerBase::Instance())->_isOperational = true;
    //startSYNC(&MyMaster_Data);
}

void ServoController::stopped(CO_Data *d)
{

}

void ServoController::post_sync(CO_Data *d)
{
    ServoController *controller = static_cast<ServoController*>(ControllerBase::Instance());
    //qDebug()<<"post_TPDO";
    //qDebug()<<"MotionStatusByte:"<<I201MotionStatusByte<<"-"<<controller->bitsFromByte((UNS16*)&I201MotionStatusByte,16);

    controller->parseStatusWord(Statusword);
    if(controller->_masterSyncCount % 50 == 0)
    {
        //qDebug()<<"Status word: "<< Statusword;
    }
    quint8 plcOpenState = (I201MotionStatusByte & 0x0F);
    qDebug()<<plcOpenState;
    //fprintf(stderr,"%i", plcOpenState);
    //fflush(stderr);

    //QThread::usleep(15);
    if(plcOpenState != controller->oldPlcState)
    {
        qDebug()<<"PLC:"<<plcOpenState;

        if((plcOpenState==2)||(plcOpenState==1))
        {
            controller->_is_moving = false;
        }
        if((controller->oldPlcState==3) && ((plcOpenState==2)||(plcOpenState==1)))
        {
            emit controller->MoveDone();
        }
    }
    controller->oldPlcState = plcOpenState;

    controller->_masterSyncCount++;
   // qDebug()<<controller->_masterSyncCount;
}

void ServoController::post_TPDO(CO_Data *d)
{


}

void ServoController::post_emcy(CO_Data *d, u_int8_t nodeID, u_int16_t errCode, u_int8_t errReg)
{
    qDebug("Master received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\n", nodeID, errCode, errReg);
}

void ServoController::post_SlaveBootup(CO_Data *d, u_int8_t nodeid)
{
    qDebug("Post Slave bootup");
    //masterSendNMTstateChange(&MyMaster_Data,1,NMT_Start_Node);          //??????????????
    ServoController *controller = static_cast<ServoController*>(ControllerBase::Instance());
    controller->_configureSlaveStep = 0;
    controller->ConfigureSlave(controller->_co_data,
                               controller->_slaveId);
}

void ServoController::post_SlaveStateChanged(CO_Data *d, u_int8_t nodeId, e_nodeState newNodeState)
{
    qDebug()<<"CANOpen node state: " << newNodeState;
}

void ServoController::CheckSDO(CO_Data *d, u_int8_t nodeId)
{
    UNS32 abortCode;
    if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
    {
        qDebug("Master : Failed in initializing slave %2.2x, step %d, AbortCode :%4.4x \n",
               nodeId, static_cast<ServoController*>(ControllerBase::Instance())->_configureSlaveStep, abortCode);
    }
    /* Finalise last SDO transfer with this node */
    closeSDOtransfer(d, nodeId, SDO_CLIENT);
}

void ServoController::InitNode(CO_Data *d, u_int32_t id)
{
    qDebug("Master InitNode");
    //setNodeId(&MyMaster_Data, 2);
    setState(&MyMaster_Data, Initialisation);
}

void ServoController::DummyInit(CO_Data *d, u_int32_t id)
{
    StopTimerLoop(&DeInitNode);
    canClose(&MyMaster_Data);
    UnLoadCanDriver(drvHandle);
}

void ServoController::DeInitNode(CO_Data *d, u_int32_t id)
{
    setState(&MyMaster_Data, Stopped);
}

void ServoController::ReadDeviceControlState()
{
    readNetworkDictCallback(_co_data,
                            _slaveId,
                            DeviceControlState,
                            0,
                            0,
                            ReadDeviceControlStateCallback,
                            0);
}

void ServoController::ReadDeviceControlStateCallback(CO_Data *d, u_int8_t nodeId)
{
    UNS32 abortCode;
    UNS32 data=0;
    UNS32 size=64;

    if(getReadResultNetworkDict(static_cast<ServoController*>(_instance)->_co_data, nodeId, &data, &size, &abortCode) != SDO_FINISHED)
        printf("Master : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeId, abortCode);
    else
    {
        static_cast<ServoController*>(_instance)->_deviceStateStr = ServoController::deviceStateStrings[data];
    }

}

void ServoController::ConfigureSlave(CO_Data *d, u_int8_t nodeId)
{
    UNS8 res;
    UNS8 data8;
    UNS16 data16;
    switch(++static_cast<ServoController*>(_instance)->_configureSlaveStep)
    {
    case 1:         //Set cycle period
        data8 = CycleTime_4ms;
        res = writeNetworkDictCallBack(d,
                                       nodeId,
                                       rCycleTime,
                                       0,
                                       1,
                                       0,
                                       &data8,
                                       ConfigureSlaveCheckSDO,
                                       0);
        qDebug("Set Cycle time - Result %d",res);
        break;
    case 2:         //Set ready to switch-on
        data16 = 0x0006;
        res = writeNetworkDictCallBack(d,
                                       nodeId,
                                       ControlWord,
                                       0,
                                       2,
                                       0,
                                       &data16,
                                       ConfigureSlaveCheckSDO,
                                       0);
        qDebug("Set Ready to switch-on - Result %d",res);
        break;
    case 3:
        I210PosiControlWord = 0x00;

        qDebug()<<"Start NODE";
        masterSendNMTstateChange(d, static_cast<ServoController*>(_instance)->_slaveId, NMT_Start_Node);
        //startSYNC(_co_data);
        setState(_co_data,Operational);
        break;
    }
}

void ServoController::ConfigureSlaveCheckSDO(CO_Data *d, u_int8_t nodeId)
{
    CheckSDO(d,nodeId);
    static_cast<ServoController*>(_instance)->ConfigureSlave(d,nodeId);
}

void ServoController::parseStatusWord(u_int16_t statusWord)
{
    QBitArray bits;
    bits = bitsFromByte(&statusWord,16);

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
        _statusWord.Fault = bits[3];

    _statusWord.Fault               = bits[3];
    _statusWord.VoltageEnabed       = bits[4];
    _statusWord.QuickStop           = bits[5];
    _statusWord.SwitchOnDisable     = bits[6];
    _statusWord.Warning             = bits[7];
    _statusWord.Message             = bits[8];
    if(!_statusWord.TargetReached && bits[10])
    {
        qDebug()<<"Motion DONE!!!!";
        _is_moving=false;
        _statusWord.TargetReached = bits[10];
        //emit MoveDone();
    }
    else
    {
        _statusWord.TargetReached = bits[10];
    }
}

QBitArray ServoController::bitsFromByte(u_int16_t *data, int size)
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

//bool ServoController::ReadDCLinkState()
//{
//    readNetworkDictCallback(_co_data, _slaveId, 0x280E, 0, 0,ReadDCLinkStateCallback, 0);
//    return true;
//}

//void ServoController::ReadDCLinkStateCallback(CO_Data *d, u_int8_t nodeId)
//{
//    ServoController *controller = static_cast<ServoController*>(ControllerBase::Instance());
//    UNS32 abortCode;
//    UNS32 data=0;
//    UNS32 size=64;
//    if(getReadResultNetworkDict(d, nodeId, &data, &size, &abortCode) != SDO_FINISHED)
//    {
//        qDebug("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeId, abortCode);
//    }
//    else
//    {
//        closeSDOtransfer(d,nodeId,SDO_CLIENT);
//        if(data==0)
//            controller->ReadDCLinkState();
//        else if(data == 1)
//        {

//        }
//    }

//}

u_int8_t ServoController::ReadSDO(u_int8_t nodeId, u_int16_t index, u_int8_t subIndex, u_int8_t dataType, void *data, u_int32_t *size)
{
    UNS32 abortCode = 0;
    UNS8 res = SDO_UPLOAD_IN_PROGRESS;
    // Read SDO
    UNS8 err = readNetworkDict (_co_data, nodeId, index, subIndex, dataType, 0);
    if (err)
    {
        qDebug()<<"error read dict";
        return 0xFF;
    }
    for(;;)
    {
        res = getReadResultNetworkDict (_co_data, nodeId, data, size, &abortCode);
        if (res != SDO_UPLOAD_IN_PROGRESS)
            break;
        QThread::sleep(1);
        continue;
    }
    closeSDOtransfer(_co_data, nodeId, SDO_CLIENT);
    if (res == SDO_FINISHED)
        return 0;

    return 0xFF;
}



void ServoController::DisableOperation()
{
    if(!_statusWord.OpEnabled)
        return;

    UNS32 abortCode;
    UNS16 data = 0x0006;
    writeNetworkDict(_co_data, _slaveId, ControlWord, 0, 2, 0, &data, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);
    return;
}


void ServoController::Homing()
{
    if(_is_moving)
        return;
    qDebug()<<"Homing";
    I210PosiControlWord = 0x0000;
    I211MotionCommandByte = MC_Home;
    I213TargetPosition = 0;
    _is_moving = true;
    _exec_flag = true;
    sendPDOevent(_co_data);
}


void ServoController::Init()
{
}

bool ServoController::isInit()
{
    return true;
}



void ServoController::StartMoveVelocity(int velocity)
{
    // if(_is_moving)
    //   return;

    INTEGER16 velData = velocity * 10;
    qDebug()<<"Send MC_MoveVelocity:" << velocity << "("<<velData<<")";
    UNS32 abortCode;
    //INTEGER32 data = 0x0000;
    //INTEGER32 targetPos = 0x0000;
    //UNS32 size=4;

    UNS16 data16 = 0x0000;
    writeNetworkDict(_co_data, _slaveId, PositionControlWord, 0, 2, 0, &data16, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);


    UNS8 data8 = MC_MoveVelocity;
    writeNetworkDict(_co_data, _slaveId, MotionCommandByte, 0, 1, 0, &data8, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);


    writeNetworkDict(_co_data, _slaveId, TargetSpeed, 0, 2, 0, &velData, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);
    qDebug() << "Set V.factor abortCode:" << abortCode;

    data16 = 0x0001;
    writeNetworkDict(_co_data, _slaveId, PositionControlWord, 0, 2, 0, &data16, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    _is_moving = true;
}

void ServoController::StopMoveVelocity()
{    // if(_is_moving)
    //   return;
    qDebug()<<"Send MC_Stop";
    UNS32 abortCode;
    //UNS32 size=4;
    UNS16 data16 = 0x0000;
    writeNetworkDict(_co_data, _slaveId, PositionControlWord, 0, 2, 0, &data16, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    UNS8 data8 = MC_Stop;

    writeNetworkDict(_co_data, _slaveId, MotionCommandByte, 0, 1, 0, &data8, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    data16 = 0x0001;
    writeNetworkDict(_co_data, _slaveId, PositionControlWord, 0, 2, 0, &data16, 0);
    while (getWriteResultNetworkDict (_co_data, _slaveId, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);

    _is_moving = false;
}
