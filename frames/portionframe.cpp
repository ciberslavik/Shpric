#include "portionframe.h"
#include "ui_portionframe.h"

#include <Dialogs/inputdigitdialog.h>

PortionFrame::PortionFrame(QWidget *parent) :
    FrameBase(parent),
    ui(new Ui::PortionFrame)
{
    ui->setupUi(this);

    connect(ui->txtMass, &QClickableLineEdit::clicked, this, &PortionFrame::onTxtClicked);
    connect(ui->txtTemp, &QClickableLineEdit::clicked, this, &PortionFrame::onTxtClicked);
    connect(ui->txtPause, &QClickableLineEdit::clicked, this, &PortionFrame::onTxtClicked);
    connect(ui->txtReverse, &QClickableLineEdit::clicked, this, &PortionFrame::onTxtClicked);
    connect(ui->txtFirstPortion, &QClickableLineEdit::clicked, this, &PortionFrame::onTxtClicked);

}

PortionFrame::~PortionFrame()
{
    delete ui;
}




void PortionFrame::onPressedConfigChanged()
{
    ui->txtMass->setText(QString::number(_pressetConfig->Mass()));
    ui->txtTemp->setText(QString::number(_pressetConfig->Temp()));
    ui->txtPause->setText(QString::number(_pressetConfig->Pause()));
    ui->txtReverse->setText(QString::number(_pressetConfig->Reverse()));
    ui->txtFirstPortion->setText(QString::number(_pressetConfig->FirstPortion()));
}

void PortionFrame::onTxtClicked()
{
    QClickableLineEdit *line = static_cast<QClickableLineEdit*>(sender());
    InputDigitDialog *dlg = new InputDigitDialog(line);
    if(dlg->exec() == QDialog::Accepted)
    {
        _pressetConfig = getConfig();
        emit PressetChanged();
    }
}

void PortionFrame::setEnabled(bool enabled)
{
    ui->txtMass->setEnabled(enabled);
    ui->txtTemp->setEnabled(enabled);
    ui->txtPause->setEnabled(enabled);
    ui->txtReverse->setEnabled(enabled);
    ui->txtFirstPortion->setEnabled(enabled);

    ui->btnMassPlus->setEnabled(enabled);
    ui->btnMassMinus->setEnabled(enabled);
    ui->btnTempPlus->setEnabled(enabled);
    ui->btnTempMinus->setEnabled(enabled);
    ui->btnReversePlus->setEnabled(enabled);
    ui->btnReverseMinus->setEnabled(enabled);
    ui->btnFirstPortionPlus->setEnabled(enabled);
    ui->btnFirstPortionMinus->setEnabled(enabled);
    ui->btnPausePlus->setEnabled(enabled);
    ui->btnPauseMinus->setEnabled(enabled);
}
void PortionFrame::on_btnTempMinus_released()
{
    int temp = ui->txtTemp->text().toInt();
    temp--;
    _pressetConfig->setTemp(temp);
    ui->txtTemp->setText(QString::number(temp));
}

void PortionFrame::on_btnTempPlus_released()
{
    double temp = ui->txtMass->text().toDouble();
    temp++;
    _pressetConfig->setMass(temp);
    ui->txtMass->setText(QString::number(temp));
    emit PressetChanged();
}

void PortionFrame::on_btnMassPlus_released()
{
    double temp = ui->txtMass->text().toDouble();
    temp++;
    _pressetConfig->setMass(temp);
    ui->txtMass->setText(QString::number(temp));
    emit PressetChanged();
}
void PortionFrame::on_btnMassMinus_released()
{
    double temp = ui->txtMass->text().toDouble();
    temp--;
    _pressetConfig->setMass(temp);
    ui->txtMass->setText(QString::number(temp));
    emit PressetChanged();
}
void PortionFrame::on_btnReversePlus_clicked()
{
    int temp = ui->txtReverse->text().toInt();
    temp++;
    _pressetConfig->setReverse(temp);
    ui->txtReverse->setText(QString::number(temp));
    emit PressetChanged();
}

void PortionFrame::on_btnReverseMinus_clicked()
{
    int temp = ui->txtReverse->text().toInt();
    temp--;
    _pressetConfig->setReverse(temp);
    ui->txtReverse->setText(QString::number(temp));
    emit PressetChanged();
}



void PortionFrame::on_btnPausePlus_released()
{
    double temp = ui->txtPause->text().toDouble();
    temp++;
    _pressetConfig->setPause(temp);
    ui->txtPause->setText(QString::number(temp));
    emit PressetChanged();
}

void PortionFrame::on_btnPauseMinus_released()
{
    double temp = ui->txtPause->text().toDouble();
    temp--;
    _pressetConfig->setPause(temp);
    ui->txtPause->setText(QString::number(temp));
    emit PressetChanged();
}

void PortionFrame::on_btnFirstPortionPlus_released()
{
    double temp = ui->txtFirstPortion->text().toDouble();
    temp++;
    _pressetConfig->setFirstPortion(temp);
    ui->txtFirstPortion->setText(QString::number(temp));
    emit PressetChanged();
}

void PortionFrame::on_btnFirstPortionMinus_released()
{
    double temp = ui->txtFirstPortion->text().toDouble();
    temp--;
    _pressetConfig->setFirstPortion(temp);
    ui->txtFirstPortion->setText(QString::number(temp));
    emit PressetChanged();
}

PressetConfig *PortionFrame::getConfig()
{
    PressetConfig *cfg = new PressetConfig(this);
    cfg->setMass(ui->txtMass->text().toInt());
    cfg->setTemp(ui->txtTemp->text().toInt());
    cfg->setPause(ui->txtPause->text().toInt());
    cfg->setReverse(ui->txtReverse->text().toInt());
    cfg->setFirstPortion(ui->txtFirstPortion->text().toInt());
    cfg->setLogic((int)Logic::Portion);
    return cfg;
}
