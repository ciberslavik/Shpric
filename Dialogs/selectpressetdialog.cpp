#include "selectpressetdialog.h"
#include "ui_selectpressetdialog.h"

#include <QStringListModel>

#include <setting/settingstorage.h>

SelectPressetDialog::SelectPressetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectPressetDialog)
{
    ui->setupUi(this);
    ui->listWidget->addItems(SettingStorage::instance()->getPressetsList());
}

SelectPressetDialog::~SelectPressetDialog()
{
    delete ui;
}

PressetConfig *SelectPressetDialog::getPresset()
{
    return _currentPresset;
}

void SelectPressetDialog::on_pushButton_clicked()
{
    reject();
}

void SelectPressetDialog::on_pushButton_2_clicked()
{
    accept();
}

void SelectPressetDialog::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)
    _currentPresset = SettingStorage::instance()->getPressetConfig(current->text());
}
