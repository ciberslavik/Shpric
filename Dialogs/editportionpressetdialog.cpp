#include "editportionpressetdialog.h"
#include "inputdigitdialog.h"
#include "inputtextdialog.h"
#include "ui_editportionpressetdialog.h"

#include <setting/settingstorage.h>

EditPortionPressetDialog::EditPortionPressetDialog(QWidget *parent) :
    EditPressetDialog(parent),
    ui(new Ui::EditPortionPressetDialog)
{
    ui->setupUi(this);

    connect(ui->txtName, &QClickableLineEdit::clicked, this, &EditPortionPressetDialog::onTxtNameClicked);
    connect(ui->txtMass,&QClickableLineEdit::clicked,this, &EditPortionPressetDialog::onTxtClicked);
    connect(ui->txtTemp,&QClickableLineEdit::clicked,this, &EditPortionPressetDialog::onTxtClicked);
    connect(ui->txtPause,&QClickableLineEdit::clicked,this, &EditPortionPressetDialog::onTxtClicked);

    connect(ui->txtFirstPortion,&QClickableLineEdit::clicked,this, &EditPortionPressetDialog::onTxtClicked);
    connect(ui->txtReverse,&QClickableLineEdit::clicked,this, &EditPortionPressetDialog::onTxtClicked);
}

EditPortionPressetDialog::~EditPortionPressetDialog()
{
    delete ui;
}

void EditPortionPressetDialog::on_btnSave_released()
{

}

void EditPortionPressetDialog::onTxtNameClicked()
{
    InputTextDialog *dlg = new InputTextDialog(this);

    dlg->setText(ui->txtName->text());

    if(dlg->exec()==QDialog::Accepted)
    {
        ui->txtName->setText(dlg->getText());
    }

    delete dlg;
}

void EditPortionPressetDialog::onTxtClicked()
{
    QLineEdit *edit = static_cast<QLineEdit*>(sender());

    InputDigitDialog *dlg = new InputDigitDialog(edit,this);
    if(dlg->exec()==QDialog::Accepted)
    {

    }
    delete dlg;
}


PressetConfig *EditPortionPressetDialog::getConfig()
{
    PressetConfig *cfg = new PressetConfig(this);
    cfg->setPressetName(ui->txtName->text());
    cfg->setMass(ui->txtMass->text().toInt());
    cfg->setTemp(ui->txtTemp->text().toInt());
    cfg->setReverse(ui->txtReverse->text().toInt());
    cfg->setFirstPortion(ui->txtFirstPortion->text().toInt());
    cfg->setPause(ui->txtPause->text().toInt());
    cfg->setLogic((int)Logic::Portion);
    return cfg;
}

void EditPortionPressetDialog::setConfig(PressetConfig *config)
{
    ui->txtName->setText(config->PressetName());
    ui->txtMass->setText(QString::number(config->Mass()));
    ui->txtTemp->setText(QString::number(config->Temp()));
    ui->txtPause->setText(QString::number(config->Pause()));
    //ui->txtSwiper->setText(QString::number(config->SwiperPulse()));
    ui->txtReverse->setText(QString::number(config->Reverse()));
    ui->txtFirstPortion->setText(QString::number(config->FirstPortion()));
}

void EditPortionPressetDialog::on_btnSave_clicked()
{
    PressetConfig *config = SettingStorage::instance()->createPressetConfig(ui->txtName->text());
    config->setPressetName(ui->txtName->text());
    config->setTemp(ui->txtTemp->text().toInt());
    config->setMass(ui->txtMass->text().toDouble());
    config->setPause(ui->txtPause->text().toInt());
    //config->setSwiperPulse(ui->txtSwiper->text().toInt());
    config->setReverse(ui->txtReverse->text().toDouble());
    config->setFirstPortion(ui->txtFirstPortion->text().toDouble());
    config->setLogic((int)Logic::Portion);

    accept();
}

void EditPortionPressetDialog::on_btnCancel_clicked()
{
    reject();
}
