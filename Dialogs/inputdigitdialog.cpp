#include "inputdigitdialog.h"
#include "ui_inputdigitdialog.h"

#include <QTimer>

InputDigitDialog::InputDigitDialog(QLineEdit *edit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDigitDialog)
{
    _editText = edit;
    ui->setupUi(this);
    connect(ui->btn0, &QPushButton::clicked, this, &InputDigitDialog::onDigitButtonPressed);
    connect(ui->btn1, &QPushButton::clicked, this, &InputDigitDialog::onDigitButtonPressed);
    connect(ui->btn2, &QPushButton::clicked, this, &InputDigitDialog::onDigitButtonPressed);
    connect(ui->btn3, &QPushButton::clicked, this, &InputDigitDialog::onDigitButtonPressed);
    connect(ui->btn4, &QPushButton::clicked, this, &InputDigitDialog::onDigitButtonPressed);
    connect(ui->btn5, &QPushButton::clicked, this, &InputDigitDialog::onDigitButtonPressed);
    connect(ui->btn6, &QPushButton::clicked, this, &InputDigitDialog::onDigitButtonPressed);
    connect(ui->btn7, &QPushButton::clicked, this, &InputDigitDialog::onDigitButtonPressed);
    connect(ui->btn8, &QPushButton::clicked, this, &InputDigitDialog::onDigitButtonPressed);
    connect(ui->btn9, &QPushButton::clicked, this, &InputDigitDialog::onDigitButtonPressed);

    connect(ui->btn_dot, &QPushButton::clicked, this, &InputDigitDialog::onDotButtonPressed);

    connect(ui->btn_del, &QPushButton::clicked, this, &InputDigitDialog::onDeleteButtonPressed);
    connect(ui->btnOk, &QPushButton::clicked, this, &InputDigitDialog::onOkButtonPressed);
    connect(ui->btnCancel, &QPushButton::clicked, this, &InputDigitDialog::onCancelButtonPressed);
    ui->txtEdit->setText(_editText->text());


    QTimer::singleShot(0,ui->txtEdit,SLOT(selectAll()));
    ui->txtEdit->setFocus(Qt::FocusReason::MouseFocusReason);
    _first_key = true;
}

InputDigitDialog::~InputDigitDialog()
{
    delete ui;
}

void InputDigitDialog::onDigitButtonPressed()
{
    QPushButton *button = static_cast<QPushButton*>(sender());
    if(_first_key)
    {
        ui->txtEdit->clear();
        _first_key = false;
    }
    //ui->txtEdit->setText(ui->txtEdit->text()+button->text());
    double new_value = (ui->txtEdit->text()+button->text()).toDouble();
    QString new_label = QString::number(new_value,'g',15);
    ui->txtEdit->setText(new_label);

}

void InputDigitDialog::onDotButtonPressed()
{
    if(ui->txtEdit->text().contains('.'))
        return;

    ui->txtEdit->setText(ui->txtEdit->text()+'.');
}

void InputDigitDialog::onDeleteButtonPressed()
{
    QString new_value = ui->txtEdit->text();
    new_value.chop(1);
    ui->txtEdit->setText(new_value);
    _first_key = false;
}

void InputDigitDialog::onOkButtonPressed()
{
    _editText->setText(ui->txtEdit->text());
    accept();
}

void InputDigitDialog::onCancelButtonPressed()
{
    reject();
}
