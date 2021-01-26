#include "mainframe.h"
#include "portionframe.h"
#include "stuffingframe.h"
#include "swiperframe.h"
#include "ui_mainframe.h"

#include <Dialogs/inputdigitdialog.h>
#include <Dialogs/selectpressetdialog.h>

#include <QDialog>

#include <IO/iomodbus.h>

#include <setting/settingstorage.h>

#include <can/fakeservo.h>

MainFrame::MainFrame(Machine *machine, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainFrame),
    _currentFrame(nullptr),
    //_currentLogic(nullptr),
    //_currentLogicPresset(nullptr),
    _frameSignals()
{
    ui->setupUi(this);
    connect(ui->btnStuffing, &QPushButton::toggled, this, &MainFrame::on_btnStuffing_toggled);
    connect(ui->btnPortion, &QPushButton::toggled, this, &MainFrame::on_btnPortion_toggled);
    connect(ui->btnSwiper, &QPushButton::toggled, this, &MainFrame::on_btnSwiper_toggled);

    connect(ui->btnPedal0, &QPushButton::toggled, this, &MainFrame::on_PedalMode_toggled);
    connect(ui->btnPedal1, &QPushButton::toggled, this, &MainFrame::on_PedalMode_toggled);
    connect(ui->btnPedal2, &QPushButton::toggled, this, &MainFrame::on_PedalMode_toggled);
    connect(ui->btnPedal3, &QPushButton::toggled, this, &MainFrame::on_PedalMode_toggled);

    _machine = machine;


    PressetConfig *lastPresset = SettingStorage::instance()->getLastPresset();

    if(lastPresset==nullptr)
    {
        lastPresset = SettingStorage::instance()->getFirstPresset();
        SettingStorage::instance()->setLastPresset(lastPresset);
    }
    _machine->LoadLogicPresset(lastPresset);
    _machine->setLogicMode(LogicMode::Stop);
    //setPressetFrame(lastPresset);
    toggleLogicButton(_machine->CurrentLogic()->Logic());

    ui->btnPedal0->setChecked(true);


    _machine->LoadLogicPresset(lastPresset);

    ui->txtPressetName->setText(lastPresset->PressetName());
    //ui->btnPortion->setEnabled(false);
    //ui->btnStuffing->setEnabled(false);

    ui->txtPortionCount->setText("0");
    connect(_machine, &Machine::CycleDone,this, &MainFrame::onCycleDone);
    connect(_machine, &Machine::AnalogDataReady, this, &MainFrame::onAnalogDataReady);
    //connect(_machine, &Machine)

    ui->indVacuum->setRange(0, 100);
    ui->indSetpoint->setRange(0, 100);
    _vac_setpoint = 10;
     _machine->setVacuum(_vac_setpoint);
    ui->lblSetpoint->setText(QString::number(_vac_setpoint));
}

MainFrame::~MainFrame()
{
    delete ui;
}


void MainFrame::on_btnStuffing_toggled(bool toggled)
{
    if(toggled)
    {
        PressetConfig *presset = SettingStorage::instance()->getFirsLogic(Logic::Stuffing);
        setPressetFrame(presset);
        _machine->LoadLogicPresset(presset);
    }
}

void MainFrame::on_btnPortion_toggled(bool toggled)
{
    if(toggled)
    {
        PressetConfig *presset = SettingStorage::instance()->getFirsLogic(Logic::Portion);
        setPressetFrame(presset);
        _machine->LoadLogicPresset(presset);
    }
}

void MainFrame::on_btnSwiper_toggled(bool toggled)
{
    if(toggled)
    {
        PressetConfig *presset = SettingStorage::instance()->getFirsLogic(Logic::Swiper);
        setPressetFrame(presset);
        _machine->LoadLogicPresset(presset);
    }
}

void MainFrame::on_PedalMode_toggled(bool toggled)
{
    if(toggled)
    {
        QPushButton *btn = static_cast<QPushButton*>(sender());
        int btnNumber = btn->text().toInt();
        switch (btnNumber)
        {
        case 0:
            ui->lblPedalMode->setText(QStringLiteral(u"Блок."));
            _machine->setLogicMode(LogicMode::Stop);
            break;
        case 1:
            ui->lblPedalMode->setText(QStringLiteral(u"Разовая порция"));
            _machine->setLogicMode(LogicMode::Single);
            break;
        case 2:
            ui->lblPedalMode->setText(QStringLiteral(u"Непрерывное порционирование"));
            _machine->setLogicMode(LogicMode::Cyclic);
            break;
        case 3:
            ui->lblPedalMode->setText(QStringLiteral(u"Кол-во штук"));
            _machine->setLogicMode(LogicMode::CyclicEndless);
            break;
        }

    }
}

void MainFrame::onTxtRecipeIDPressed()
{

}

void MainFrame::setPressetFrame(PressetConfig *presset)
{
    bool isCreate = true;
    if(_currentFrame!=nullptr)
    {
        if(_currentFrame->getPresset()->Logic()==((int)presset->Logic()))
        {
            isCreate = false;
        }
    }

    if(isCreate)
    {
        deleteCurrentFrame();
        switch (static_cast<Logic_t>(presset->Logic()))
        {
        case Logic::Stuffing:
            _currentFrame = new StuffingFrame(ui->PressetFrame);
            break;

        case Logic::Swiper:
            _currentFrame = new SwiperFrame(ui->PressetFrame);
            break;

        case Logic::Portion:
            _currentFrame = new PortionFrame(ui->PressetFrame);
            break;
        }
    }
    _currentFrame->setPressetConfig(presset);
    ui->txtPressetName->setText(presset->PressetName());

    _frameSignals.push(connect(_currentFrame,&FrameBase::PressetChanged, this, &MainFrame::onPressetChanged));
    _currentFrame->showMaximized();
}

void MainFrame::deleteCurrentFrame()
{
    if(_currentFrame != nullptr)
    {
        _currentFrame->close();
        while(!_frameSignals.isEmpty())
        {
            disconnect(_frameSignals.pop());
        }

        delete _currentFrame;
        _currentFrame = nullptr;
    }
}

void MainFrame::toggleLogicButton(Logic_t logic)
{
    switch (logic)
    {
    case Logic::Stuffing:
        ui->btnStuffing->toggle();
        break;

    case Logic::Swiper:
        ui->btnSwiper->toggle();
        break;

    case Logic::Portion:
        ui->btnPortion->toggle();
        break;
    }
}

//void MainFrame::setLogicMode(enum Machine::LogicMode logicMode)
//{
//    _currentLogicMode = logicMode;
//    if(_machine!=nullptr)
//        _machine->setLogicMode(_currentLogicMode);
//}

void MainFrame::on_btnManagePresset_released()
{
    SelectPressetDialog *dialog =new SelectPressetDialog(this);
    if(dialog->exec()==QDialog::Accepted)
    {
        PressetConfig *cfg = dialog->getPresset();
        ui->txtPressetName->setText(cfg->PressetName());
        setPressetFrame(cfg);
        _machine->LoadLogicPresset(cfg);
        //_currentFrame->setPressetConfig(cfg);

        SettingStorage::instance()->setLastPresset(cfg);
    }
}

void MainFrame::on_btnEnableOperation_released()
{
    emit DisableOperation();
}

void MainFrame::on_buttonConfig_released()
{
    if(_machine->LogicMode() == LogicMode::Stop)
    {
        emit SettingCalled();
    }
}

void MainFrame::on_btnVacuumOnOff_toggled(bool checked)
{
    _machine->Vacuum(checked);
}

void MainFrame::on_txtPortionCount_clicked()
{
    if(_machine->LogicMode() == LogicMode::CyclicEndless)
    {
        InputDigitDialog *dlg = new InputDigitDialog(ui->txtPortionCount, this);
        if(dlg->exec()==QDialog::Accepted)
        {
            _machine->setTargetPortions(ui->txtPortionCount->text().toInt());
            //ui->txtPortionCount->setText(QString::number(dlg->result()));

        }
        delete dlg;
    }
}

void MainFrame::onPressetChanged()
{
    PressetConfig *conf = _currentFrame->getPresset();
    conf->setPressetName(ui->txtPressetName->text());
    _machine->LoadLogicPresset(conf);
    SettingStorage::instance()->setLastPresset(conf);
}

void MainFrame::onCycleDone()
{
    if(_machine->LogicMode()==LogicMode::Cyclic)
    {
        ui->txtPortionCount->setText(QString::number(_machine->Portions()));
    }
    if(_machine->LogicMode()==LogicMode::CyclicEndless)
    {
        int portions = _machine->TargetPortions()-_machine->Portions();
        ui->txtPortionCount->setText(QString::number(portions));
    }
}

void MainFrame::on_pushButton_29_toggled(bool checked)
{
    _machine->onPedalToggled(checked);
}

void MainFrame::on_btnVacuumMinus_clicked()
{
    if(_vac_setpoint>0)
        _vac_setpoint-=10;
    ui->lblSetpoint->setText(QString::number(_vac_setpoint));
    ui->indSetpoint->setValue(_vac_setpoint);
    _machine->setVacuum(_vac_setpoint);
}

void MainFrame::on_btnVacuumPlus_clicked()
{
    if(_vac_setpoint<=100)
        _vac_setpoint += 10;
    ui->lblSetpoint->setText(QString::number(_vac_setpoint));
    ui->indSetpoint->setValue(_vac_setpoint);
    _machine->setVacuum(_vac_setpoint);
}

void MainFrame::onAnalogDataReady(double value)
{
    ui->indVacuum->setValue(static_cast<int>(value));
    ui->lblVacuum->setText(QString::number(value));
}
