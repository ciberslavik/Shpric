#ifndef STANDBYFRAME_H
#define STANDBYFRAME_H

#include <QTimer>
#include <QWidget>

namespace Ui {
class StandbyFrame;
}

class StandbyFrame : public QWidget
{
    Q_OBJECT

public:
    explicit StandbyFrame(QWidget *parent = nullptr);
    ~StandbyFrame();
private slots:
    void timerTimeout();
    void on_btnEnable_released();

signals:
    void EnableOperation();
private:
    Ui::StandbyFrame *ui;
    QTimer _timeTimer;
};

#endif // STANDBYFRAME_H
