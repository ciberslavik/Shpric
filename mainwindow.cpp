#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <frames/mainframe.h>
#include <frames/selectsettingframe.h>

#include <Dialogs/inputtextdialog.h>

#include <setting/settingstorage.h>

#include <can/StoberServo.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _currentFrame(nullptr),
    _prevFrame(nullptr),
    _currentGrid(nullptr),
    _mainFrame(nullptr),
    _selectSettingFrame(nullptr),
    _stbyFrame(nullptr)
{
    ui->setupUi((QMainWindow*)this);
    _currentGrid = new QGridLayout(ui->mainFrame);





    showStbyFrame();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onEnableOperation()
{
    _machine->EnableOperation();
    showMainFrame();
}

void MainWindow::onDisableOperation()
{
    _machine->DisableOperation();
    showStbyFrame();
}

void MainWindow::onSettingCalled()
{
    showSelectSettingFrame();
}

void MainWindow::onReturn()
{
    showMainFrame();
}

void MainWindow::onCycleDone()
{

}

void MainWindow::createMainFrame()
{
    if(_mainFrame!=nullptr)
        return;
    MainFrame *frame = new MainFrame(_machine, ui->mainFrame);


    connect(frame, &MainFrame::DisableOperation, this, &MainWindow::onDisableOperation);
    connect(frame, &MainFrame::SettingCalled, this, &MainWindow::onSettingCalled);

    _mainFrame = (QWidget*)frame;
}

void MainWindow::createStbyFrame()
{
    if(_stbyFrame!=nullptr)
        return;

    StandbyFrame *stbyFrame = new StandbyFrame(ui->mainFrame);

    connect(stbyFrame, &StandbyFrame::EnableOperation, this, &MainWindow::onEnableOperation);
    _stbyFrame = stbyFrame;
}

void MainWindow::createSelectSettingFrame()
{
    if(_selectSettingFrame!=nullptr)
        return;
    SelectSettingFrame *settingFrame = new SelectSettingFrame(ui->mainFrame);

    connect(settingFrame, &SelectSettingFrame::Return, this, &MainWindow::onReturn);

    _selectSettingFrame = settingFrame;
}

void MainWindow::showMainFrame()
{
    createMainFrame();
    setCurrentFrame(_mainFrame);
}


void MainWindow::showStbyFrame()
{
    createStbyFrame();
    setCurrentFrame(_stbyFrame);
}

void MainWindow::showSelectSettingFrame()
{
    createSelectSettingFrame();
    setCurrentFrame(_selectSettingFrame);
}

void MainWindow::setCurrentFrame(QWidget *frame)
{
    if(_currentFrame != nullptr)
    {
        ((QLayout*)_currentGrid)->removeWidget(_currentFrame);
        _currentFrame->close();
        _prevFrame = _currentFrame;
    }
    _currentGrid->addWidget(frame, 0, 0, 1, 1);
    frame->showMaximized();

    _currentFrame = frame;
}

void MainWindow::showEvent(QShowEvent *e)
{
    Q_UNUSED(e)
    if(_firstShow)
    {
        _mConfig = static_cast<MachineConfig*>
                (SettingStorage::instance()->getConfig("MachineConfig"));

        _device = new Device();
        _device->setSlaveID(1);
        _device->Init(DriverType::Socket, "can0");
        _device->setSyncCyclePeriod(10000);
        _device->Start();

        _servo = new StoberServo(IOBase::instance(), _device);


        MachineController *controller = new MachineController(_mConfig, IOBase::instance(), ControllerBase::Instance());

        _machine = new Machine();
        _machine->InitMachine(controller);

        _firstShow = false;
    }
}
