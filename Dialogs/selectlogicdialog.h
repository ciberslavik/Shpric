#ifndef SELECTLOGICDIALOG_H
#define SELECTLOGICDIALOG_H

#include <QDialog>
#include "setting/pressetconfig.h"
namespace Ui {
class SelectLogicDialog;
}

class SelectLogicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectLogicDialog(QWidget *parent = nullptr);
    ~SelectLogicDialog();

    Logic getSelectedLogic();
private slots:
    void on_btnPrev_released();

    void on_btnNext_released();

private:
    Ui::SelectLogicDialog *ui;
    Logic _selectedLogic;
};

#endif // SELECTLOGICDIALOG_H
