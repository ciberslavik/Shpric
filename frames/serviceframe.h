#ifndef SERVICEFRAME_H
#define SERVICEFRAME_H

#include "configframebase.h"

#include <QWidget>

namespace Ui {
class ServiceFrame;
}

class ServiceFrame : public ConfigFrameBase
{
    Q_OBJECT

public:
    explicit ServiceFrame(QWidget *parent = nullptr);
    ~ServiceFrame();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_toggled(bool checked);

private:
    Ui::ServiceFrame *ui;
};

#endif // SERVICEFRAME_H
