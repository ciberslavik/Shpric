#ifndef EDITSWIPERPRESSETDIALOG_H
#define EDITSWIPERPRESSETDIALOG_H

#include "editpressetdialog.h"

#include <QDialog>

namespace Ui {
class EditSwiperPressetDialog;
}

class EditSwiperPressetDialog : public EditPressetDialog
{
    Q_OBJECT

public:
    explicit EditSwiperPressetDialog(QWidget *parent = nullptr);
    ~EditSwiperPressetDialog();
private slots:
    void onTxtNameClicked();
    void onTxtClicked();
    void on_pushButton_released();

    void on_pushButton_2_released();

private:
    Ui::EditSwiperPressetDialog *ui;

    // EditPressetDialog interface
public:
    PressetConfig *getConfig() override;
    void setConfig(PressetConfig *config) override;
};

#endif // EDITSWIPERPRESSETDIALOG_H
