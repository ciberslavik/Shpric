#ifndef EDITPORTIONPRESSETDIALOG_H
#define EDITPORTIONPRESSETDIALOG_H

#include "editpressetdialog.h"

#include <QDialog>

namespace Ui {
class EditPortionPressetDialog;
}

class EditPortionPressetDialog : public EditPressetDialog
{
    Q_OBJECT

public:
    explicit EditPortionPressetDialog(QWidget *parent = nullptr);
    ~EditPortionPressetDialog();

private slots:
    void on_btnSave_released();
    void onTxtNameClicked();
    void onTxtClicked();
    void on_btnSave_clicked();

    void on_btnCancel_clicked();

private:
    Ui::EditPortionPressetDialog *ui;

    // EditPressetDialog interface
public:
    PressetConfig *getConfig() override;
    void setConfig(PressetConfig *config) override;
};

#endif // EDITPORTIONPRESSETDIALOG_H
