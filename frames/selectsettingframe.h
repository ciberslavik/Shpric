#ifndef SELECTSETTINGFRAME_H
#define SELECTSETTINGFRAME_H

#include "configframebase.h"

#include <QGridLayout>
#include <QWidget>

namespace Ui {
class SelectSettingFrame;
}

class SelectSettingFrame : public QWidget
{
    Q_OBJECT

public:
    explicit SelectSettingFrame(QWidget *parent = nullptr);
    ~SelectSettingFrame();
signals:
    void Return();
private slots:
    void on_btnReturn_released();

    void on_btnModbus_toggled(bool checked);

    void on_btnCANBus_toggled(bool checked);

    void on_btnMachine_toggled(bool checked);

    void on_btnRecipe_toggled(bool checked);


    void on_btnService_toggled(bool checked);

private:
    Ui::SelectSettingFrame *ui;
    ConfigFrameBase *_currentFrame;
    QGridLayout *_currentGrid;


    void removeCurrentFrame();
    void showCurrentFrame();
};

#endif // SELECTSETTINGFRAME_H
