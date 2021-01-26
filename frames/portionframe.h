#ifndef PORTIONFRAME_H
#define PORTIONFRAME_H

#include "framebase.h"

#include <QWidget>

namespace Ui {
class PortionFrame;
}

class PortionFrame : public FrameBase
{
    Q_OBJECT

public:
    explicit PortionFrame(QWidget *parent = nullptr);
    ~PortionFrame();

private slots:
    void on_btnReversePlus_clicked();

    void on_btnReverseMinus_clicked();

    void on_btnTempMinus_released();

    void on_btnTempPlus_released();

    void on_btnMassPlus_released();

    void on_btnMassMinus_released();

    void on_btnPausePlus_released();

    void on_btnPauseMinus_released();

    void on_btnFirstPortionPlus_released();

    void on_btnFirstPortionMinus_released();

private:
    Ui::PortionFrame *ui;
    PressetConfig *getConfig();
    // FrameBase interface
protected slots:
    void onPressedConfigChanged() override;

    void onTxtClicked();
    // FrameBase interface
public:
    void setEnabled(bool enabled) override;
};

#endif // PORTIONFRAME_H
