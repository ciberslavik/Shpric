#include "serviceframe.h"
#include "ui_serviceframe.h"

#include <IO/iobase.h>

ServiceFrame::ServiceFrame(QWidget *parent) :
    ConfigFrameBase(nullptr,parent),
    ui(new Ui::ServiceFrame)
{
    ui->setupUi(this);
}

ServiceFrame::~ServiceFrame()
{
    delete ui;
    if(IOBase::instance()!=nullptr)
    IOBase::instance()->SwiperOn(false);
}

void ServiceFrame::on_pushButton_clicked()
{


}

void ServiceFrame::on_pushButton_toggled(bool checked)
{
    if(checked)
        IOBase::instance()->SwiperOn(true);
    else
        IOBase::instance()->SwiperOn(false);
}
