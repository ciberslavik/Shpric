#include "machineconfigframe.h"
#include "ui_machineconfigframe.h"

#include <Dialogs/inputdigitdialog.h>

MachineConfigFrame::MachineConfigFrame(AbstractConfig *config, QWidget *parent) :
    ConfigFrameBase(config, parent),
    ui(new Ui::MachineConfigFrame)
{
    ui->setupUi(this);

    connect(ui->txtEncoderTics, &QClickableLineEdit::clicked, this, &MachineConfigFrame::onLineEditClicked);
    connect(ui->txtGearboxRatio, &QClickableLineEdit::clicked, this, &MachineConfigFrame::onLineEditClicked);
    connect(ui->txtPumpPerformance, &QClickableLineEdit::clicked, this, &MachineConfigFrame::onLineEditClicked);
    connect(ui->txtSwiperSpeed, &QClickableLineEdit::clicked, this, &MachineConfigFrame::onLineEditClicked);
    loadMachineConfig();
}

MachineConfigFrame::~MachineConfigFrame()
{
    delete ui;
}

void MachineConfigFrame::onLineEditClicked()
{
    QLineEdit *edit = static_cast<QLineEdit*>(sender());
    InputDigitDialog *dlg = new InputDigitDialog(edit,this);
    dlg->exec();
}

void MachineConfigFrame::on_pushButton_released()
{
    getMachineConfig()->Save();
}

MachineConfig *MachineConfigFrame::getMachineConfig()
{
    MachineConfig *config = static_cast<MachineConfig*>(_curentConfig);

    config->setEncoderTicks(ui->txtEncoderTics->text().toInt());
    config->setGearboxRatio(ui->txtGearboxRatio->text().toDouble());
    config->setMassPerPumpRev(ui->txtPumpPerformance->text().toDouble());

    config->setSwiperSpeed(ui->txtSwiperSpeed->text().toInt());

    return config;
}

void MachineConfigFrame::loadMachineConfig()
{
    MachineConfig *cfg = static_cast<MachineConfig*>(_curentConfig);
    ui->txtEncoderTics->setText(QString::number(cfg->EncoderTicks()));
    ui->txtGearboxRatio->setText(QString::number(cfg->GearboxRatio()));
    ui->txtPumpPerformance->setText(QString::number(cfg->MassPerPumpRev()));
    ui->txtSwiperSpeed->setText(QString::number(cfg->SwiperSpeed()));
}
