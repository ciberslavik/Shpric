#include "modbusconfigframe.h"
#include "ui_modbusconfigframe.h"

ModbusConfigFrame::ModbusConfigFrame(AbstractConfig *config, QWidget *parent) :
    ConfigFrameBase(config, parent),
    ui(new Ui::ModbusConfigFrame)
{
    ui->setupUi(this);
    ui->cbBaudRate->addItem("9600");
    ui->cbBaudRate->addItem("14400");
    ui->cbBaudRate->addItem("19200");
    ui->cbBaudRate->addItem("38400");
    ui->cbBaudRate->addItem("57600");
    ui->cbBaudRate->addItem("115200");

    ui->cbParity->addItem("Even");
    ui->cbParity->addItem("Odd");
    ui->cbParity->addItem("None");

    ui->cbDataBits->addItem("8");
    ui->cbDataBits->addItem("9");

    ui->cbStopBits->addItem("1");
    ui->cbStopBits->addItem("2");

    LoadSerialConfig();
    _portUpdateTimer.setInterval(200);
    connect(&_portUpdateTimer, &QTimer::timeout, this, &ModbusConfigFrame::on_portUpdate_timeout);
    _portUpdateTimer.start();
}

ModbusConfigFrame::~ModbusConfigFrame()
{
    delete ui;
}

SerialConfig *ModbusConfigFrame::getSerialConfig()
{
    SerialConfig *currConfig = static_cast<SerialConfig *>(_curentConfig);
    currConfig->setPortName(ui->cbPortName->currentText());
    currConfig->setBaudrate(ui->cbBaudRate->currentText().toInt());
    currConfig->setParity(ui->cbParity->currentText());
    currConfig->setDataBits(ui->cbDataBits->currentText().toInt());
    currConfig->setStopBits(ui->cbStopBits->currentText().toInt());
    currConfig->setRetries(ui->spinNumberRetries->value());
    currConfig->setTimeout(ui->txtModbusTimeout->text().toInt());
    currConfig->setCycleTime(ui->txtCycleTime->text().toInt());
    currConfig->setSlaveAddress(ui->txtSlaveID->text().toInt());

    return currConfig;
}

void ModbusConfigFrame::LoadSerialConfig()
{
    SerialConfig *currConfig = static_cast<SerialConfig *>(_curentConfig);
    ui->cbPortName->setCurrentText(currConfig->PortName());
    ui->cbBaudRate->setCurrentText(QString::number(currConfig->Baudrate()));
    ui->cbParity->setCurrentText(currConfig->Parity());
    ui->cbDataBits->setCurrentText(QString::number(currConfig->DataBits()));
    ui->cbStopBits->setCurrentText(QString::number(currConfig->StopBits()));
    ui->txtSlaveID->setText(QString::number(currConfig->SlaveAddress()));
    ui->txtCycleTime->setText(QString::number(currConfig->CycleTime()));
    ui->txtModbusTimeout->setText(QString::number(currConfig->Timeout()));
    ui->spinNumberRetries->setValue(currConfig->Retries());
}

void ModbusConfigFrame::on_btnSave_released()
{
    SerialConfig *config = getSerialConfig();
    config->Save();
}

void ModbusConfigFrame::on_portUpdate_timeout()
{
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    if(_portCount!=infos.count())
    {
        ui->cbPortName->clear();

        for(int i = 0;i<infos.count();i++)
        {
            ui->cbPortName->addItem(infos.at(i).systemLocation());
        }
        SerialConfig *currConfig = static_cast<SerialConfig*>(_curentConfig);
        if(ui->cbPortName->findText(currConfig->PortName())==-1)
        {
            ui->cbPortName->addItem(currConfig->PortName());
        }
        ui->cbPortName->setCurrentText(currConfig->PortName());

        _portCount = infos.count();
    }
}
