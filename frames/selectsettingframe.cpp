#include "machineconfigframe.h"
#include "modbusconfigframe.h"
#include "pressetmanagerframe.h"
#include "selectsettingframe.h"
#include "serviceframe.h"
#include "ui_selectsettingframe.h"

#include <setting/settingstorage.h>

SelectSettingFrame::SelectSettingFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectSettingFrame)
{
    ui->setupUi(this);

    _currentFrame = new ModbusConfigFrame(SettingStorage::instance()->getConfig("IOSerialConfig"),ui->frame);

    _currentGrid = new QGridLayout(ui->frame);

    _currentGrid->addWidget(_currentFrame);
    _currentFrame->showMaximized();

    ui->btnModbus->setChecked(true);
}

SelectSettingFrame::~SelectSettingFrame()
{
    delete ui;
}

void SelectSettingFrame::on_btnReturn_released()
{
    emit Return();
}

void SelectSettingFrame::on_btnModbus_toggled(bool checked)
{
    Q_UNUSED(checked)
    removeCurrentFrame();
    _currentFrame = new ModbusConfigFrame(SettingStorage::instance()->getConfig("IOSerialConfig"),ui->frame);
    showCurrentFrame();
}

void SelectSettingFrame::on_btnCANBus_toggled(bool checked)
{
    Q_UNUSED(checked)
}

void SelectSettingFrame::on_btnMachine_toggled(bool checked)
{
    Q_UNUSED(checked)
    removeCurrentFrame();
    _currentFrame = new MachineConfigFrame(SettingStorage::instance()->getConfig("MachineConfig"),ui->frame);
    showCurrentFrame();
}

void SelectSettingFrame::removeCurrentFrame()
{
    if(_currentFrame!=nullptr)
    {
        _currentFrame->close();
        _currentGrid->removeWidget(_currentFrame);
        delete _currentFrame;
        _currentFrame = nullptr;
    }
}

void SelectSettingFrame::showCurrentFrame()
{
    if((_currentFrame!=nullptr) && (_currentGrid!=nullptr))
    {
        _currentGrid->addWidget(_currentFrame,0,0,1,1);
        _currentFrame->showMaximized();
    }
}

void SelectSettingFrame::on_btnRecipe_toggled(bool checked)
{
    if(checked)
    {
        removeCurrentFrame();
        _currentFrame = new PressetManagerFrame(ui->frame);
        showCurrentFrame();
    }
}



void SelectSettingFrame::on_btnService_toggled(bool checked)
{
    if(checked)
    {
        removeCurrentFrame();
        _currentFrame = new ServiceFrame(ui->frame);
        showCurrentFrame();
    }
}
