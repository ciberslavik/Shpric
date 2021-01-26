#ifndef QCLICKABLELINEEDIT_H
#define QCLICKABLELINEEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QMouseEvent>

namespace Ui {
class QClickableLineEdit;
}

class QClickableLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit QClickableLineEdit(QWidget *parent = nullptr);
    ~QClickableLineEdit();
signals:
    void clicked();
private:
    Ui::QClickableLineEdit *ui;

    bool eventFilter(QObject* object, QEvent* event);

    bool _mousePressed;
};

#endif // QCLICKABLELINEEDIT_H
