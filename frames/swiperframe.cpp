#include "swiperframe.h"
#include "ui_swiperframe.h"

#include <Dialogs/inputdigitdialog.h>

#include <setting/settingstorage.h>

SwiperFrame::SwiperFrame(QWidget *parent) :
    FrameBase(parent),
    ui(new Ui::SwiperFrame)
{
    ui->setupUi(this);
    connect(ui->txtMass, &QClickableLineEdit::clicked, this, &SwiperFrame::onTxtClicked);
    connect(ui->txtTemp, &QClickableLineEdit::clicked, this, &SwiperFrame::onTxtClicked);
    connect(ui->txtSwiper, &QClickableLineEdit::clicked, this, &SwiperFrame::onTxtClicked);
    connect(ui->txtReverse, &QClickableLineEdit::clicked, this, &SwiperFrame::onTxtClicked);
    connect(ui->txtFirstPortion, &QClickableLineEdit::clicked, this, &SwiperFrame::onTxtClicked);
}

SwiperFrame::~SwiperFrame()
{
    delete ui;
}

void SwiperFrame::setEnabled(bool enabled)
{
    ui->txtMass->setEnabled(enabled);
    ui->txtTemp->setEnabled(enabled);
    ui->txtSwiper->setEnabled(enabled);
    ui->txtReverse->setEnabled(enabled);
    ui->txtFirstPortion->setEnabled(enabled);

    ui->btnMassPlus->setEnabled(enabled);
    ui->btnMassMinus->setEnabled(enabled);
    ui->btnTempPlus->setEnabled(enabled);
    ui->btnTempMinus->setEnabled(enabled);
    ui->btnSwiperPlus->setEnabled(enabled);
    ui->btnSwiperMinus->setEnabled(enabled);
    ui->btnReversePlus->setEnabled(enabled);
    ui->btnReverseMinus->setEnabled(enabled);
    ui->btnFirstPortionPlus->setEnabled(enabled);
    ui->btnFirstPortionMinus->setEnabled(enabled);
}

PressetConfig *SwiperFrame::getConfig()
{
    PressetConfig *cfg = new PressetConfig(this);
    cfg->setMass(ui->txtMass->text().toInt());
    cfg->setTemp(ui->txtTemp->text().toInt());
    cfg->setSwiperPulse(ui->txtSwiper->text().toInt());
    cfg->setReverse(ui->txtReverse->text().toInt());
    cfg->setFirstPortion(ui->txtFirstPortion->text().toInt());
    cfg->setLogic((int)Logic::Swiper);
    cfg->setPause(5);
    return cfg;
}


void SwiperFrame::onPressedConfigChanged()
{
    ui->txtMass->setText(QString::number(_pressetConfig->Mass()));
    ui->txtTemp->setText(QString::number(_pressetConfig->Temp()));
    ui->txtSwiper->setText(QString::number((_pressetConfig->SwiperPulse())));
    ui->txtReverse->setText(QString::number(_pressetConfig->Reverse()));
    ui->txtFirstPortion->setText(QString::number(_pressetConfig->FirstPortion()));
}

void SwiperFrame::onTxtClicked()
{
    QClickableLineEdit *line = static_cast<QClickableLineEdit*>(sender());
    InputDigitDialog *dlg = new InputDigitDialog(line);
    if(dlg->exec() == QDialog::Accepted)
    {
        _pressetConfig = getConfig();
        emit PressetChanged();
    }
}

void SwiperFrame::on_btnTempPlus_released()
{
    int temp = ui->txtTemp->text().toInt();
    temp++;
    _pressetConfig->setTemp(temp);
    ui->txtTemp->setText(QString::number(temp));
    emit PressetChanged();
}

void SwiperFrame::on_btnTempMinus_released()
{
    int temp = ui->txtTemp->text().toInt();
    temp--;
    _pressetConfig->setTemp(temp);
    ui->txtTemp->setText(QString::number(temp));
    emit PressetChanged();
}
void SwiperFrame::on_btnMassPlus_released()
{
    double temp = ui->txtMass->text().toDouble();
    temp++;
    _pressetConfig->setMass(temp);
    ui->txtMass->setText(QString::number(temp));
    emit PressetChanged();
}

void SwiperFrame::on_btnMassMinus_released()
{
    double temp = ui->txtMass->text().toDouble();
    temp--;
    _pressetConfig->setMass(temp);
    ui->txtMass->setText(QString::number(temp));
    emit PressetChanged();
}

void SwiperFrame::on_btnSwiperPlus_released()
{
    int temp = ui->txtSwiper->text().toInt();
    temp++;
    _pressetConfig->setSwiperPulse(temp);
    ui->txtSwiper->setText(QString::number(temp));
    emit PressetChanged();
}

void SwiperFrame::on_btnSwiperMinus_released()
{
    int temp = ui->txtSwiper->text().toInt();
    temp--;
    _pressetConfig->setSwiperPulse(temp);
    ui->txtSwiper->setText(QString::number(temp));
    emit PressetChanged();
}

void SwiperFrame::on_btnFirstPortionPlus_released()
{
    double temp = ui->txtFirstPortion->text().toDouble();
    temp++;
    _pressetConfig->setFirstPortion(temp);
    ui->txtFirstPortion->setText(QString::number(temp));
    emit PressetChanged();
}

void SwiperFrame::on_btnFirstPortionMinus_released()
{
    double temp = ui->txtFirstPortion->text().toDouble();
    temp--;
    _pressetConfig->setFirstPortion(temp);
    ui->txtFirstPortion->setText(QString::number(temp));
    emit PressetChanged();
}

void SwiperFrame::on_btnReversePlus_clicked()
{
    int temp = ui->txtReverse->text().toInt();
    temp++;
    _pressetConfig->setReverse(temp);
    ui->txtReverse->setText(QString::number(temp));
    emit PressetChanged();
}

void SwiperFrame::on_btnReverseMinus_clicked()
{
    int temp = ui->txtReverse->text().toInt();
    temp--;
    _pressetConfig->setReverse(temp);
    ui->txtReverse->setText(QString::number(temp));
    emit PressetChanged();
}
