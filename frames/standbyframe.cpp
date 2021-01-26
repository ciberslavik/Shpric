#include "standbyframe.h"
#include "ui_standbyframe.h"

#include <QDateTime>

StandbyFrame::StandbyFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StandbyFrame)
{
    ui->setupUi(this);
    _timeTimer.setInterval(1000);
    connect(&_timeTimer, &QTimer::timeout, this, &StandbyFrame::timerTimeout);

    setWindowState(Qt::WindowState::WindowFullScreen);
    _timeTimer.start();
}

StandbyFrame::~StandbyFrame()
{
    delete ui;
}

void StandbyFrame::timerTimeout()
{
    ui->lblTime->setText(QDateTime::currentDateTime().toString(Qt::DateFormat::DefaultLocaleLongDate));
    ui->lblTime->repaint();
}

void StandbyFrame::on_btnEnable_released()
{
    emit EnableOperation();
}
