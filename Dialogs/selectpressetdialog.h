#ifndef SELECTPRESSETDIALOG_H
#define SELECTPRESSETDIALOG_H

#include <QDialog>
#include <QListView>
#include <QListWidget>

#include <setting/pressetconfig.h>

namespace Ui {
class SelectPressetDialog;
}

class SelectPressetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPressetDialog(QWidget *parent = nullptr);
    ~SelectPressetDialog();

    PressetConfig* getPresset();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::SelectPressetDialog *ui;
    PressetConfig *_currentPresset;
};

#endif // SELECTPRESSETDIALOG_H
