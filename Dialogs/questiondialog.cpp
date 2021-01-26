#include "questiondialog.h"
#include "ui_questiondialog.h"

QuestionDialog::QuestionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionDialog)
{
    ui->setupUi(this);
    ui->btnOK->setFixedWidth(120);
    ui->btnCancel->setFixedWidth(120);
}

QuestionDialog::~QuestionDialog()
{
    delete ui;
}

void QuestionDialog::setText(QString text)
{
    ui->lblText->setText(text);
}

void QuestionDialog::on_btnOK_released()
{
    accept();
}

void QuestionDialog::on_btnCancel_released()
{
    reject();
}
