#ifndef QUESTIONDIALOG_H
#define QUESTIONDIALOG_H

#include <QDialog>

namespace Ui {
class QuestionDialog;
}

class QuestionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionDialog(QWidget *parent = nullptr);
    ~QuestionDialog();
    void setText(QString text);
private slots:
    void on_btnOK_released();

    void on_btnCancel_released();

private:
    Ui::QuestionDialog *ui;
};

#endif // QUESTIONDIALOG_H
