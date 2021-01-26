#ifndef MODBUSCONFIGFRAME_H
#define MODBUSCONFIGFRAME_H

#include "configframebase.h"

#include <QTimer>
#include <QWidget>

#include <setting/ioserialconfig.h>

namespace Ui {
class ModbusConfigFrame;
}

class ModbusConfigFrame : public ConfigFrameBase
{
    Q_OBJECT

public:
    explicit ModbusConfigFrame(AbstractConfig *config, QWidget *parent = nullptr);
    ~ModbusConfigFrame();
private slots:
    void on_btnSave_released();
    void on_portUpdate_timeout();
private:
    Ui::ModbusConfigFrame *ui;

    SerialConfig *getSerialConfig();
    void LoadSerialConfig();
    QTimer _portUpdateTimer;
    int _portCount;
};

#endif // MODBUSCONFIGFRAME_H
