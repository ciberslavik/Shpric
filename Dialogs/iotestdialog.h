#ifndef IOTESTDIALOG_H
#define IOTESTDIALOG_H

#include <QDialog>
#include <machineio_.h>
#include <QDebug>

#include <IO/iobase.h>

namespace Ui {
class IOTestDialog;
}

class IOTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IOTestDialog(QWidget *parent = nullptr);
    ~IOTestDialog();

private slots:
    void on_pushButton_released();

    void on_IOTestDialog_accepted();

    void on_IOTestDialog_rejected();

    void rawDataRecived(QString data);
    void on_swiperMoveDone();
    void on_pedalToggle(bool state);
    void on_btnSwipe_released();

    void on_btnSwipe_toggled(bool checked);
    void statusPrint(QString msg);
private:
    Ui::IOTestDialog *ui;

    IOBase *_mIO;

    bool _isConnected;
};

#endif // IOTESTDIALOG_H
