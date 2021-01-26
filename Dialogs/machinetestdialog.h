#ifndef MACHINETESTDIALOG_H
#define MACHINETESTDIALOG_H

#include <QDialog>

namespace Ui {
class MachineTestDialog;
}

class MachineTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MachineTestDialog(QWidget *parent = nullptr);
    ~MachineTestDialog();

private:
    Ui::MachineTestDialog *ui;
};

#endif // MACHINETESTDIALOG_H
