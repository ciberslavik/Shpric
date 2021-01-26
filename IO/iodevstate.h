#ifndef IODEVSTATE_H
#define IODEVSTATE_H
#include <QObject>

struct DevState
{
    bool ExtDisable;
    bool TimeoutDisable;
    bool WdReset;
    bool BrownotReset;
    bool IllegalOPCodeReset;
    DevState()
    {
        ExtDisable = false;
        TimeoutDisable = false;
        WdReset = false;
        BrownotReset = false;
        IllegalOPCodeReset = false;
    }
    static DevState fromByte(u_int8_t data)
    {
        DevState result;
        result.ExtDisable           = (data & 0x01) ? true : false;
        result.TimeoutDisable       = (data & 0x02) ? true : false;
        result.WdReset              = (data & 0x04) ? true : false;
        result.BrownotReset         = (data & 0x08) ? true : false;
        result.IllegalOPCodeReset   = (data & 0x10) ? true : false;
        return result;
    }

    u_int8_t toByte()
    {
        u_int8_t result = 0x00;

        if(ExtDisable)
            result |= 0x01;
        if(TimeoutDisable)
            result |= 0x02;
        if(WdReset)
            result |= 0x04;
        if(BrownotReset)
            result |= 0x08;
        if(IllegalOPCodeReset)
            result |= 0x10;

        return result;
    }
};

#endif // IODEVSTATE_H
