#ifndef INPUTTEXTDIALOG_H
#define INPUTTEXTDIALOG_H

#include <QDialog>
#include <QLineEdit>

#define NEXT_ROW_MARKER 0
namespace Ui {
class InputTextDialog;
}



class InputTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputTextDialog(QWidget *parent = nullptr);
    ~InputTextDialog();

    QString getText();
    void setText(QString text);

private slots:
    void buttonClicked(int key);
private:
    Ui::InputTextDialog *ui;

    void createKeyboard();

    QLineEdit *textbox;
};

#endif // INPUTTEXTDIALOG_H
