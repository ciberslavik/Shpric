#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "framebase.h"

//#include <AbstractMachineControl.h>
#include <QWidget>
#include <machine.h>

#include <logic/AbstractLogic.h>

namespace Ui {
class MainFrame;
}

class MainFrame : public QWidget
{
    Q_OBJECT

public:
    explicit MainFrame(Machine *machine, QWidget *parent = nullptr);
    ~MainFrame();
public slots:
    void on_btnStuffing_toggled(bool toggled);
    void on_btnPortion_toggled(bool toggled);
    void on_btnSwiper_toggled(bool toggled);

    void on_PedalMode_toggled(bool toggled);
    void onTxtRecipeIDPressed();
signals:
    void DisableOperation();
    void SettingCalled();
private slots:
    void on_btnManagePresset_released();
    void on_btnEnableOperation_released();
    void on_buttonConfig_released();
    void on_btnVacuumOnOff_toggled(bool checked);
    void on_txtPortionCount_clicked();
    void onPressetChanged();
    void onCycleDone();
    void on_pushButton_29_toggled(bool checked);

    void on_btnVacuumMinus_clicked();

    void on_btnVacuumPlus_clicked();
    void onAnalogDataReady(double value);
private:
    Ui::MainFrame *ui;


    void setPressetFrame(PressetConfig *presset);
    void deleteCurrentFrame();
//    void setLogicMode(enum Machine::LogicMode logicMode);
    void toggleLogicButton(Logic_t logic);

    Machine *_machine;
    FrameBase *_currentFrame;

    QStack<QMetaObject::Connection> _frameSignals;
    double _vac_setpoint;
};

#endif // MAINFRAME_H
