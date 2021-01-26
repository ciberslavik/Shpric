#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <frames/mainframe.h>
#include <frames/selectsettingframe.h>

#include <Dialogs/inputtextdialog.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _currentFrame(nullptr),
    _prevFrame(nullptr),
    _currentGrid(nullptr),
    _machine(nullptr),
    _mainFrame(nullptr),
    _selectSettingFrame(nullptr),
    _stbyFrame(nullptr)
{
    ui->setupUi(this);
    _currentGrid = new QGridLayout(ui->mainFrame);

    showStbyFrame();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMachine(Machine *machine)
{
    _machine = machine;
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
    MainFrame *frame = new MainFrame(_machine,ui->mainFrame);


    connect(frame, &MainFrame::DisableOperation, this, &MainWindow::onDisableOperation);
    connect(frame, &MainFrame::SettingCalled, this, &MainWindow::onSettingCalled);

    _mainFrame = frame;
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
        _currentGrid->removeWidget(_currentFrame);
        _currentFrame->close();
        _prevFrame = _currentFrame;
    }
    _currentGrid->addWidget(frame, 0, 0, 1, 1);
    frame->showMaximized();

    _currentFrame = frame;
}
