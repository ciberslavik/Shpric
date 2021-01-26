#ifndef EDITPRESSETDIALOG_H
#define EDITPRESSETDIALOG_H

#include <QDialog>

#include <setting/pressetconfig.h>

namespace Ui {
class EditPressetDialog;
}

class EditPressetDialog : public QDialog
{
    Q_OBJECT

public:
    ~EditPressetDialog();
    virtual PressetConfig *getConfig()=0;
    virtual void setConfig(PressetConfig *config)=0;

protected:
    explicit EditPressetDialog(QWidget *parent = nullptr);

private:
    Ui::EditPressetDialog *ui;
    PressetConfig *_presset;
};

#endif // EDITPRESSETDIALOG_H
