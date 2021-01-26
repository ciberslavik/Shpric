#ifndef PRESSETMANAGERFRAME_H
#define PRESSETMANAGERFRAME_H

#include "configframebase.h"
#include "pressetsmodel.h"

#include <QWidget>
#include <QStringListModel>

namespace Ui {
class PressetManagerFrame;
}

class PressetManagerFrame : public ConfigFrameBase
{
    Q_OBJECT

public:
    explicit PressetManagerFrame(QWidget *parent = nullptr);
    ~PressetManagerFrame();

private slots:
    void on_btnAddPresset_released();

    void on_btnEditPresset_released();

    void on_btnRemovePresset_released();

private:
    Ui::PressetManagerFrame *ui;
    //QStringListModel *model;

    PressetsModel *model;
};

#endif // PRESSETMANAGERFRAME_H
