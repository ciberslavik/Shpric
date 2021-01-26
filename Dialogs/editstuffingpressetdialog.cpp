#include "editstuffingpressetdialog.h"
#include "inputdigitdialog.h"
#include "inputtextdialog.h"
#include "ui_editstuffingpressetdialog.h"

#include <setting/settingstorage.h>

EditStuffingPressetDialog::EditStuffingPressetDialog(QWidget *parent) :
    EditPressetDialog(parent),
    ui(new Ui::EditStuffingPressetDialog)
{
    ui->setupUi(this);
    setFixedSize(this->size());
    connect(ui->txtName, &QClickableLineEdit::clicked, this, &EditStuffingPressetDialog::onTxtNameClicked);
    connect(ui->txtTemp,&QClickableLineEdit::clicked,this, &EditStuffingPressetDialog::onTxtClicked);
    connect(ui->txtReverse,&QClickableLineEdit::clicked,this, &EditStuffingPressetDialog::onTxtClicked);
}

EditStuffingPressetDialog::~EditStuffingPressetDialog()
{
    delete ui;
}


PressetConfig *EditStuffingPressetDialog::getConfig()
{
    PressetConfig *cfg = new PressetConfig(this);
    cfg->setPressetName(ui->txtName->text());
    cfg->setTemp(ui->txtTemp->text().toInt());
    cfg->setReverse(ui->txtReverse->text().toInt());
    cfg->setLogic((int)Logic::Stuffing);
    return cfg;
}

void EditStuffingPressetDialog::setConfig(PressetConfig *config)
{
    ui->txtName->setText(config->PressetName());
    ui->txtTemp->setText(QString::number(config->Temp()));
    ui->txtReverse->setText(QString::number(config->Reverse()));
}

void EditStuffingPressetDialog::onTxtNameClicked()
{
    InputTextDialog *dlg = new InputTextDialog(this);

    dlg->setText(ui->txtName->text());

    if(dlg->exec()==QDialog::Accepted)
    {
        ui->txtName->setText(dlg->getText());
    }

    delete dlg;
}

void EditStuffingPressetDialog::onTxtClicked()
{
    QLineEdit *edit = static_cast<QLineEdit*>(sender());

    InputDigitDialog *dlg = new InputDigitDialog(edit,this);
    if(dlg->exec()==QDialog::Accepted)
    {

    }
    delete dlg;
}

void EditStuffingPressetDialog::on_btnOK_clicked()
{
    PressetConfig *config = SettingStorage::instance()->createPressetConfig(ui->txtName->text());
    config->setPressetName(ui->txtName->text());
    config->setTemp(ui->txtTemp->text().toInt());
    config->setReverse(ui->txtReverse->text().toDouble());
    config->setLogic((int)Logic::Stuffing);

    accept();
}

void EditStuffingPressetDialog::on_btnCancel_clicked()
{
    reject();
}
