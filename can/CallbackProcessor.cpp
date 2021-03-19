#include "CallbackProcessor.h"
//CMaster* CllbackProcessor::_master = nullptr;

Device *CallbackProcessor::_device = nullptr;
void CallbackProcessor::setPointer(Device *device)
{
    _device = device;

    _device->_od->initialisation        = Initialisation;
    _device->_od->preOperational        = PreOperational;
    _device->_od->operational           = Operational;
    _device->_od->stopped               = Stopped;
    _device->_od->post_emcy             = PostEMCY;
    _device->_od->post_TPDO             = PostTPDO;
    _device->_od->post_sync             = PostSync;
    _device->_od->post_SlaveBootup      = PostSlaveBootup;
    _device->_od->post_SlaveStateChange = PostSlaveStateChanged;
    _device->_od->heartbeatError        = HeartbeatError;

}

void CallbackProcessor::HeartbeatError(CO_Data *d, u_int8_t heartbeatID)
{
    if(_device!=nullptr)
        _device->OnHeartbeatError(d, heartbeatID);
}

void CallbackProcessor::Initialisation(CO_Data *d)
{
    if(_device!=nullptr)
        _device->OnInitialisation(d);
}

void CallbackProcessor::PreOperational(CO_Data *d)
{
    if(_device!=nullptr)
        _device->OnPreOperational(d);
}

void CallbackProcessor::Operational(CO_Data *d)
{
    if(_device!=nullptr)
        _device->OnOperational(d);
}

void CallbackProcessor::Stopped(CO_Data *d)
{
    if(_device!=nullptr)
        _device->OnStopped(d);
}

void CallbackProcessor::PostSync(CO_Data *d)
{
    if(_device!=nullptr)
        _device->OnPostSync(d);
}

void CallbackProcessor::PostTPDO(CO_Data *d)
{
    if(_device!=nullptr)
        _device->OnPostTPDO(d);
}

void CallbackProcessor::PostEMCY(CO_Data *d, u_int8_t nodeID, u_int16_t errCode, u_int8_t errReg)
{
    qDebug("Master received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\n", nodeID, errCode, errReg);
    if(_device!=nullptr)
        _device->OnPostEMCY(d, nodeID, errCode, errReg);
}

void CallbackProcessor::PostSlaveBootup(CO_Data *d, u_int8_t nodeid)
{
    if(_device!=nullptr)
        _device->OnPostSlaveBootup(d,nodeid);
}

void CallbackProcessor::PostSlaveStateChanged(CO_Data *d, u_int8_t nodeId, e_nodeState newNodeState)
{
    if(_device!=nullptr)
        _device->OnPostSlaveStateChanged(d, nodeId, newNodeState);
}

void CallbackProcessor::InitNode(CO_Data *d, u_int32_t id)
{
    if(_device!=nullptr)
        _device->OnInitNode(d, id);
}

void CallbackProcessor::StopTimer(CO_Data* d, UNS32 id)
{
    if(_device!=nullptr)
        _device->OnStopTimer(d, id);
}

//void CallbackProcessor::CheckReadSDO(CO_Data *d, u_int8_t slaveId)
//{

//   // if(_device!=nullptr)
//   //     _device->OnCheckReadSDO(d, slaveId);
//}

//void CallbackProcessor::CheckWriteSDO(CO_Data *d, u_int8_t slaveId)
//{
//   // if(_device!=nullptr)
//    //    _device->OnCheckWriteSDO(d, slaveId);
//}

UNS32 CallbackProcessor::StatusWordUpdate(CO_Data *d, const indextable *unsused_indextable, u_int8_t unsused_bSubindex)
{
    if(_device!=nullptr)
       return _device->OnStatusWordUpdate(d, unsused_indextable, unsused_bSubindex);
    else
        return 0xffffffff;
}

UNS32 CallbackProcessor::MotionStatusByteUpdate(CO_Data *d, const indextable *unsused_indextable, u_int8_t unsused_bSubindex)
{
    if(_device!=nullptr)
       return _device->OnMotionStatusByteUpdate(d, unsused_indextable, unsused_bSubindex);
    else
        return 0xffffffff;
}
