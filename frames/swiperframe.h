#ifndef SWIPERFRAME_H
#define SWIPERFRAME_H

#include "framebase.h"

#include <QWidget>

namespace Ui {
class SwiperFrame;
}

class SwiperFrame : public FrameBase
{
    Q_OBJECT

public:
    explicit SwiperFrame(QWidget *parent = nullptr);
    ~SwiperFrame();
    void setEnabled(bool enabled) override;
private:
    Ui::SwiperFrame *ui;
    PressetConfig *getConfig();
    // FrameBase interface
protected slots:
    void onPressedConfigChanged() override;

    void onTxtClicked();
private slots:
    void on_btnTempPlus_released();
    void on_btnTempMinus_released();
    void on_btnMassPlus_released();
    void on_btnMassMinus_released();
    void on_btnSwiperPlus_released();
    void on_btnSwiperMinus_released();
    void on_btnFirstPortionPlus_released();
    void on_btnFirstPortionMinus_released();
    void on_btnReversePlus_clicked();
    void on_btnReverseMinus_clicked();
};

#endif // SWIPERFRAME_H
