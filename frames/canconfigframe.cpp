#include "canconfigframe.h"
#include "ui_canconfigframe.h"

CANConfigFrame::CANConfigFrame(AbstractConfig *config, QWidget *parent) :
    ConfigFrameBase(config, parent),
    ui(new Ui::CANConfigFrame)
{
    ui->setupUi(this);
}

CANConfigFrame::~CANConfigFrame()
{
    delete ui;
}
