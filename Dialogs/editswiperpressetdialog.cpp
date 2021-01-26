#include "editswiperpressetdialog.h"
#include "inputdigitdialog.h"
#include "inputtextdialog.h"
#include "ui_editswiperpressetdialog.h"

#include <setting/settingstorage.h>
#include <setting/settingstorage.h>

EditSwiperPressetDialog::EditSwiperPressetDialog(QWidget *parent) :
    EditPressetDialog(parent),
    ui(new Ui::EditSwiperPressetDialog)
{
    ui->setupUi(this);
    setFixedSize(this->size());
    connect(ui->txtName, &QClickableLineEdit::clicked, this, &EditSwiperPressetDialog::onTxtNameClicked);
    connect(ui->txtMass,&QClickableLineEdit::clicked,this, &EditSwiperPressetDialog::onTxtClicked);
    connect(ui->txtTemp,&QClickableLineEdit::clicked,this, &EditSwiperPressetDialog::onTxtClicked);
    connect(ui->txtPause,&QClickableLineEdit::clicked,this, &EditSwiperPressetDialog::onTxtClicked);
    connect(ui->txtSwiper,&QClickableLineEdit::clicked,this, &EditSwiperPressetDialog::onTxtClicked);
    connect(ui->txtFirstPortion,&QClickableLineEdit::clicked,this, &EditSwiperPressetDialog::onTxtClicked);
    connect(ui->txtReverse,&QClickableLineEdit::clicked,this, &EditSwiperPressetDialog::onTxtClicked);
}

EditSwiperPressetDialog::~EditSwiperPressetDialog()
{
    delete ui;
}

void EditSwiperPressetDialog::onTxtNameClicked()
{
    InputTextDialog *dlg = new InputTextDialog(this);

    dlg->setText(ui->txtName->text());

    if(dlg->exec()==QDialog::Accepted)
    {
        ui->txtName->setText(dlg->getText());
    }

    delete dlg;
}

void EditSwiperPressetDialog::onTxtClicked()
{
    QLineEdit *edit = static_cast<QLineEdit*>(sender());

    InputDigitDialog *dlg = new InputDigitDialog(edit,this);
    if(dlg->exec()==QDialog::Accepted)
    {

    }
    delete dlg;
}


PressetConfig *EditSwiperPressetDialog::getConfig()
{
    PressetConfig *cfg = new PressetConfig(this);
    cfg->setPressetName(ui->txtName->text());
    cfg->setMass(ui->txtMass->text().toInt());
    cfg->setTemp(ui->txtTemp->text().toInt());
    cfg->setSwiperPulse(ui->txtSwiper->text().toInt());
    cfg->setReverse(ui->txtReverse->text().toInt());
    cfg->setFirstPortion(ui->txtFirstPortion->text().toInt());
    cfg->setLogic((int)Logic::Swiper);
    return cfg;
}

void EditSwiperPressetDialog::setConfig(PressetConfig *config)
{
    ui->txtName->setText(config->PressetName());
    ui->txtMass->setText(QString::number(config->Mass()));
    ui->txtTemp->setText(QString::number(config->Temp()));
    ui->txtPause->setText(QString::number(config->Pause()));
    ui->txtSwiper->setText(QString::number(config->SwiperPulse()));
    ui->txtReverse->setText(QString::number(config->Reverse()));
    ui->txtFirstPortion->setText(QString::number(config->FirstPortion()));
}

void EditSwiperPressetDialog::on_pushButton_released()
{
    PressetConfig *config = SettingStorage::instance()->createPressetConfig(ui->txtName->text());
    config->setPressetName(ui->txtName->text());
    config->setTemp(ui->txtTemp->text().toInt());
    config->setMass(ui->txtMass->text().toDouble());
    config->setPause(ui->txtPause->text().toInt());
    config->setSwiperPulse(ui->txtSwiper->text().toInt());
    config->setReverse(ui->txtReverse->text().toDouble());
    config->setFirstPortion(ui->txtFirstPortion->text().toDouble());
    config->setLogic((int)Logic::Swiper);

    accept();
}

void EditSwiperPressetDialog::on_pushButton_2_released()
{
    reject();
}
