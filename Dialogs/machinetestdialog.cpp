#include "machinetestdialog.h"
#include "ui_machinetestdialog.h"

MachineTestDialog::MachineTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MachineTestDialog)
{
    ui->setupUi(this);
}

MachineTestDialog::~MachineTestDialog()
{
    delete ui;
}
