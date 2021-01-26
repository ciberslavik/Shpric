#ifndef CANCONFIGFRAME_H
#define CANCONFIGFRAME_H

#include "configframebase.h"

#include <QWidget>

namespace Ui {
class CANConfigFrame;
}

class CANConfigFrame : public ConfigFrameBase
{
    Q_OBJECT

public:
    explicit CANConfigFrame(AbstractConfig *config, QWidget *parent = nullptr);
    ~CANConfigFrame();

private:
    Ui::CANConfigFrame *ui;
};

#endif // CANCONFIGFRAME_H
