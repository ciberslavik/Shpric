#ifndef STUFFINGFRAME_H
#define STUFFINGFRAME_H

#include "framebase.h"

#include <QWidget>

namespace Ui {
class StuffingFrame;
}

class StuffingFrame : public FrameBase
{
    Q_OBJECT

public:
    explicit StuffingFrame(QWidget *parent = nullptr);
    ~StuffingFrame();

private:
    Ui::StuffingFrame *ui;
    PressetConfig *getConfig();
    // FrameBase interface
protected slots:
    void onPressedConfigChanged() override;

    // FrameBase interface
public:
    void setEnabled(bool enabled) override;
private slots:
    void on_btnTempMinus_released();
    void on_btnTempPlus_released();
    void on_btnReverseMinus_released();
    void on_btnReversePlus_released();
    void onTxtClicked();
};

#endif // STUFFINGFRAME_H
