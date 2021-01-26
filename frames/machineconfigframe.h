#ifndef MACHINECONFIGFRAME_H
#define MACHINECONFIGFRAME_H

#include "configframebase.h"
#include "setting/machineconfig.h"
#include "widgets/qclickablelineedit.h"

#include <QWidget>

namespace Ui {
class MachineConfigFrame;
}

class MachineConfigFrame : public ConfigFrameBase
{
    Q_OBJECT

public:
    explicit MachineConfigFrame(AbstractConfig *config, QWidget *parent = nullptr);
    ~MachineConfigFrame();
private slots:
    void onLineEditClicked();
    void on_pushButton_released();

private:
    Ui::MachineConfigFrame *ui;

    MachineConfig *getMachineConfig();
    void loadMachineConfig();
};

#endif // MACHINECONFIGFRAME_H
