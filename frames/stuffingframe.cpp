#include "stuffingframe.h"
#include "ui_stuffingframe.h"

#include <Dialogs/inputdigitdialog.h>

StuffingFrame::StuffingFrame(QWidget *parent) :
    FrameBase(parent),
    ui(new Ui::StuffingFrame)
{
    ui->setupUi(this);
    connect(ui->txtTemp, &QClickableLineEdit::clicked, this, &StuffingFrame::onTxtClicked);
    connect(ui->txtReverse, &QClickableLineEdit::clicked, this, &StuffingFrame::onTxtClicked);
}

StuffingFrame::~StuffingFrame()
{
    delete ui;
}

PressetConfig *StuffingFrame::getConfig()
{
    PressetConfig *cfg = new PressetConfig(this);
    cfg->setMass(ui->txtReverse->text().toInt());
    cfg->setTemp(ui->txtTemp->text().toInt());
    cfg->setLogic((int)Logic::Stuffing);
    return cfg;
}


void StuffingFrame::onPressedConfigChanged()
{
    ui->txtTemp->setText(QString::number(_pressetConfig->Temp()));
    ui->txtReverse->setText(QString::number(_pressetConfig->Reverse()));
}

void StuffingFrame::setEnabled(bool enabled)
{
    ui->txtTemp->setEnabled(enabled);
    ui->txtReverse->setEnabled(enabled);

    ui->btnTempPlus->setEnabled(enabled);
    ui->btnTempMinus->setEnabled(enabled);
    ui->btnReversePlus->setEnabled(enabled);
    ui->btnReverseMinus->setEnabled(enabled);
}
void StuffingFrame::on_btnTempMinus_released()
{
    int temp = ui->txtTemp->text().toInt();
    temp--;
    _pressetConfig->setTemp(temp);
    ui->txtTemp->setText(QString::number(temp));
    emit PressetChanged();
}

void StuffingFrame::on_btnTempPlus_released()
{
    int temp = ui->txtTemp->text().toInt();
    temp++;
    _pressetConfig->setTemp(temp);
    ui->txtTemp->setText(QString::number(temp));
    emit PressetChanged();
}

void StuffingFrame::on_btnReverseMinus_released()
{
    int temp = ui->txtReverse->text().toDouble();
    temp--;
    _pressetConfig->setReverse(temp);
    ui->txtReverse->setText(QString::number(temp));
    emit PressetChanged();
}

void StuffingFrame::on_btnReversePlus_released()
{
    double temp = ui->txtReverse->text().toDouble();
    temp++;
    _pressetConfig->setReverse(temp);
    ui->txtReverse->setText(QString::number(temp));
    emit PressetChanged();
}

void StuffingFrame::onTxtClicked()
{
    QClickableLineEdit *line = static_cast<QClickableLineEdit*>(sender());
    InputDigitDialog *dlg = new InputDigitDialog(line);
    if(dlg->exec() == QDialog::Accepted)
    {
        _pressetConfig = getConfig();
        emit PressetChanged();
    }
}
