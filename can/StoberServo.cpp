#include "StoberServo.h"

#include <QThread>



StoberServo::StoberServo(IOBase *io,Device *device, QObject *parent) : ControllerBase(parent)
{
    _io = io;
    _device = device;
    connect(_device, &Device::DevicePreOperational, this, &StoberServo::DevicePreOperational);
    connect(_device, &Device::MotionDone, this, &StoberServo::OnMotionDone);

    ControllerBase::setInstance(this);
}

void StoberServo::Connect()
{

}

void StoberServo::DevicePreOperational()
{
    _device->setSlaveState(NMTState::StartNode);
}

void StoberServo::OnMotionDone()
{
    qDebug()<< "MoveDone";
    emit MoveDone();
}


void StoberServo::Init()
{
}

void StoberServo::EnableOperation()
{
    qDebug() << "Begin ENABLE OPERATION";
    qDebug() << "Set enable voltage";

    _device->setControlWord(0x0006);

    qDebug() << "Switch power relay to ON";

    _io->FCPowerOn(true);
    qDebug() << "Power relay switched ON";
    qDebug() << "Wait charge DC Link";

    QElapsedTimer timer;
    timer.start();


    while(!_device->getChargeRelay())
    {
        if(timer.elapsed()>_waitTimeout)
        {
            qDebug() << "Timeout charge inverter";
            return;
        }
    }

    qDebug()<<"Enable servo logic";
    _io->FCEnable(true);
    qDebug()<<"Servo logic enabled";
    qDebug()<<"Wait ready to switch on";

    timer.restart();
    while(!(_device->getStatusWord() & 0x0001))
    {
        if(timer.elapsed()>_waitTimeout)
        {
            qDebug() << "Timeout Ready to Switch On";
            return;
        }
    }
    qDebug()<<"Device Ready to switch on";
    qDebug()<<"Enable operation";
    _device->setControlWord(0x000F);
    qDebug()<<"Wait operation enabled";
    timer.restart();
    while(!(_device->getStatusWord() & 0x0002))
    {
        if(timer.elapsed() > _waitTimeout)
        {
            qDebug() << "Timeout enable operation";
            return;
        }

    }
    qDebug()<<"Operation enabled";
    qDebug()<<"Device enable operation";
//    _device->setControlWord(0x000F);
//    qDebug()<<"Wait enable device";
//    while(1)
//    {
//        if(timer.elapsed() > _waitTimeout)
//        {
//            qDebug() << "Timeout Enable Device";
//            return;
//        }
//    }
//    qDebug()<<"Device enabled";
    qDebug()<<"Begin homing";
    _device->resetExecute();
    _device->setMotionCommand(MotionCommand::Home, MotionDirection::Forward);
    //QThread::usleep(100);
    _device->setExecute();
}

void StoberServo::DisableOperation()
{
}

void StoberServo::Move(int target, MoveDirection dir)
{
    INTEGER32 targetPos = 0x000000;

    _device->resetExecute();

    if(dir == MoveDirection::Forward)
    {
        _device->setMotionCommand(MotionCommand::MoveAdditive, MotionDirection::Forward);
        targetPos = target;
    }
    else if(dir == MoveDirection::Reverse)
    {
        _device->setMotionCommand(MotionCommand::MoveAdditive, MotionDirection::Reverse);
        targetPos =(target *  -1);
    }
    _device->setTargetPosition(targetPos);

    _device->setExecute();
}

void StoberServo::StartMoveVelocity(int velocity)
{
    _device->resetExecute();

    _device->setVFactor(velocity);
    _device->setMotionCommand(MotionCommand::MoveVelocity, MotionDirection::Forward);
    _device->setExecute();
}

void StoberServo::StopMoveVelocity()
{
    _device->resetExecute();
    _device->setMotionCommand(MotionCommand::Stop, MotionDirection::Forward);
    _device->setExecute();
}

void StoberServo::Homing()
{
    _device->resetExecute();
    _device->setMotionCommand(MotionCommand::Home, MotionDirection::Forward);
    _device->setExecute();
}

void StoberServo::setSpeed(int speed)
{
    INTEGER16 velData = static_cast<INTEGER16>(speed);
    _device->setVFactor(velData);
}

bool StoberServo::isInit()
{
    return true;
}

//void StoberServo::initialisation(CO_Data *d)
//{
//    qDebug()<<"Master Initialization";
//        UNS32 rxPDO1_COBID = 0x0180 + 1;
//        UNS32 txPDO2_COBID = 0x0200 + 1;
//        UNS32 size = sizeof(UNS32);
//        StoberServo* controller = static_cast<StoberServo*>(_instance);
//        writeLocalDict( controller->_od, /*CO_Data* d*/
//                        0x1400, /*UNS16 index*/
//                        0x01, /*UNS8 subind*/
//                        &rxPDO1_COBID, /*void * pSourceData,*/
//                        &size, /* UNS8 * pExpectedSize*/
//                        RW);  /* UNS8 checkAccess */


//        /*****************************************
//         * Define TPDOs to match slave ID=0x40 RPDOs*
//         *****************************************/
//        writeLocalDict( controller->_od, /*CO_Data* d*/
//                        0x1800, /*UNS16 index*/
//                        0x01, /*UNS8 subind*/
//                        &txPDO2_COBID, /*void * pSourceData,*/
//                        &size, /* UNS8 * pExpectedSize*/
//                        RW);  /* UNS8 checkAccess */

//}





//void StoberServo::InitNode(CO_Data *d, u_int32_t id)
//{
//    qDebug("Master InitNode");
//    StoberServo *controller = static_cast<StoberServo*>(_instance);
//    RegisterSetODentryCallBack(controller->_od, STATUS_WORD, 0, OnStatusWordUpdate);
//    RegisterSetODentryCallBack(controller->_od, MOTION_STATUS_BYTE, 0, OnI201MotionStatusByteUpdate);
//    setState(controller->_od, Initialisation);
//}

//void StoberServo::BeginCheckChargeRelay()
//{
//    readNetworkDictCallback(_od, _slaveId, CHARGE_RELAY, 0, 0, ChargeRelayCallback, 0);
//}

//void StoberServo::ChargeRelayCallback(CO_Data *d, u_int8_t nodeId)
//{
//    UNS32 abortCode;
//    UNS32 size;
//    UNS8 data;

//    StoberServo *controller = static_cast<StoberServo*>(_instance);
//    if(getReadResultNetworkDict(controller->_od, nodeId, &data, &size, &abortCode) != SDO_FINISHED)
//    {
//        qDebug("Master : Failed charge relay callback slave %2.2x, data:%2.2x \n",
//               nodeId, data);
//        qDebug()<<abortCode;
//    }
//    /* Finalise last SDO transfer with this node */
//    closeSDOtransfer(controller->_od, nodeId, SDO_CLIENT);

//    if(data != 1)
//        controller->BeginCheckChargeRelay();
//    else
//        controller->_chargeRelay = true;
//}

//void StoberServo::WriteControlWord(u_int16_t statusword)
//{
//    writeNetworkDictCallBack(_od, _slaveId, CONTROL_WORD, 0, 2, 0, &statusword, WriteControlWordCallback, 0);
//}

//void StoberServo::WriteControlWordCallback(CO_Data *d, u_int8_t nodeId)
//{
//    UNS32 abortCode;
//    StoberServo *controller = static_cast<StoberServo*>(_instance);
//    if(getWriteResultNetworkDict(controller->_od, nodeId, &abortCode)!=SDO_FINISHED)
//    {
//        qDebug("Master : Failed write control word slave %2.2x, AbortCode :%4.4x \n", nodeId, abortCode);
//    }

//    closeSDOtransfer(controller->_od, nodeId, SDO_CLIENT);
//}

//u_int32_t StoberServo::OnStatusWordUpdate(CO_Data *d, const indextable *unsused_indextable, u_int8_t unsused_bSubindex)
//{
//    qDebug()<<"OnStatusWordUpdate:" << Statusword;
//    StoberServo *controller = static_cast<StoberServo*>(_instance);
//    controller->ParseStatusWord(Statusword);
//    return 0;
//}

//u_int32_t StoberServo::OnI201MotionStatusByteUpdate(CO_Data *d, const indextable *unsused_indextable, u_int8_t unsused_bSubindex)
//{
//    //qDebug()<<"OnI201MotionStatusByteUpdate:" << I201MotionStatusByte;
//    return 0;
//}

//void StoberServo::ParseStatusWord(u_int16_t data)
//{
//    QBitArray bits;
//    bits = bitsFromByte(&data,16);


//    _statusWord.ReadyToSwitchOn     = bits[0];
//    _statusWord.SwitchedOn          = bits[1];
//    _statusWord.OpEnabled           = bits[2];

//    if(!_statusWord.Fault && bits[3])
//    {
//        qDebug()<<"Inverter FAULT";
//        emit Fault();
//    }

//    _statusWord.Fault               = bits[3];

//    _statusWord.Fault               = bits[3];
//    _statusWord.VoltageEnabed       = bits[4];
//    _statusWord.QuickStop           = bits[5];
//    _statusWord.SwitchOnDisable     = bits[6];
//    _statusWord.Warning             = bits[7];
//    _statusWord.Message             = bits[8];
//    _statusWord.TargetReached       = bits[10];

//}

//void StoberServo::ConfigureSlave(CO_Data *d, u_int8_t nodeId)
//{
//    UNS8 res;
//    UNS8 data8;
//    UNS16 data16;
//    switch(++static_cast<StoberServo*>(_instance)->_configureSlaveStep)
//    {
//    case 1:         //Set cycle period
//        data8 = CYCLE_TIME_4MS;
//        res = writeNetworkDictCallBack(d,
//                                       nodeId,
//                                       CYCLE_TIME,
//                                       0,
//                                       1,
//                                       0,
//                                       &data8,
//                                       &ConfigureSlaveCallback,
//                                       0);
//        qDebug("Set Cycle time - Result %d",res);
//        break;
//    case 2:         //Set ready to switch-on
//        data16 = 0x0006;
//        res = writeNetworkDictCallBack(d,
//                                       nodeId,
//                                       CONTROL_WORD,
//                                       0,
//                                       2,
//                                       0,
//                                       &data16,
//                                       &ConfigureSlaveCallback,
//                                       0);
//        qDebug("Set Ready to switch-on - Result %d",res);
//        break;
//    case 3:
//        I210PosiControlWord = 0x00;

//        qDebug()<<"Start NODE";
//        masterSendNMTstateChange(d, static_cast<StoberServo*>(_instance)->_slaveId, NMT_Start_Node);
//        //startSYNC(_co_data);
//        setState(d,Operational);
//        break;
//    }
//}

//void StoberServo::ConfigureSlaveCallback(CO_Data *d, u_int8_t nodeId)
//{
//    UNS32 abortCode;
//    StoberServo *controller = static_cast<StoberServo*>(_instance);
//    if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
//    {
//        qDebug("Master : Failed in initializing slave %2.2x, step %d, AbortCode :%4.4x \n",
//               nodeId, controller->_configureSlaveStep, abortCode);
//    }
//    /* Finalise last SDO transfer with this node */
//    closeSDOtransfer(controller->_od, nodeId, SDO_CLIENT);
//    //Continue configureation slave node
//    controller->ConfigureSlave(d, nodeId);
//}

//QBitArray StoberServo::bitsFromByte(u_int16_t *data, int size)
//{
//    QBitArray bits(size);
//    qint16 mask = 0x0001;
//    for(int i = 0;i<size;i++)
//    {
//        bits[i]=(*data & mask ? true : false);
//        mask = mask << 1;
//    }
//    return bits;
//}
