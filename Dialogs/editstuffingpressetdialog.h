#ifndef EDITSTUFFINGPRESSETDIALOG_H
#define EDITSTUFFINGPRESSETDIALOG_H

#include "editpressetdialog.h"

#include <QDialog>

namespace Ui {
class EditStuffingPressetDialog;
}

class EditStuffingPressetDialog : public EditPressetDialog
{
    Q_OBJECT

public:
    explicit EditStuffingPressetDialog(QWidget *parent = nullptr);
    ~EditStuffingPressetDialog();

private:
    Ui::EditStuffingPressetDialog *ui;
private slots:
    void onTxtNameClicked();
    void onTxtClicked();
    // EditPressetDialog interface
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

public:
    PressetConfig *getConfig() override;
    void setConfig(PressetConfig *config) override;
};

#endif // EDITSTUFFINGPRESSETDIALOG_H
