#include "iotestdialog.h"
#include "ui_iotestdialog.h"

IOTestDialog::IOTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IOTestDialog)
{
    ui->setupUi(this);
    _isConnected = false;
}

IOTestDialog::~IOTestDialog()
{
    delete ui;
}

void IOTestDialog::on_pushButton_released()
{
    qDebug()<<"Connect button pressed";
    if(!_isConnected)
    {

        ui->pushButton->setText("Disconnect");
        _isConnected = true;
    }
    else
    {
        ui->pushButton->setText("Connect");
        _isConnected = false;
    }
}

void IOTestDialog::on_IOTestDialog_accepted()
{

}

void IOTestDialog::on_IOTestDialog_rejected()
{

}

void IOTestDialog::rawDataRecived(QString data)
{
    ui->txtRawData->append(data+"\r\n");
}

void IOTestDialog::on_swiperMoveDone()
{
    ui->btnSwipe->setChecked(false);
}

void IOTestDialog::on_pedalToggle(bool state)
{
    Q_UNUSED(state)
}

void IOTestDialog::on_btnSwipe_released()
{

}

void IOTestDialog::on_btnSwipe_toggled(bool checked)
{
    if(checked)
    {

    }
}

void IOTestDialog::statusPrint(QString msg)
{
    ui->txtStatusPrint->append(msg);
}
