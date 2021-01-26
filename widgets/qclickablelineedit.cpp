#include "qclickablelineedit.h"
#include "ui_qclickablelineedit.h"

QClickableLineEdit::QClickableLineEdit(QWidget *parent) :
    QLineEdit(parent),
    ui(new Ui::QClickableLineEdit)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    _mousePressed = false;
}

QClickableLineEdit::~QClickableLineEdit()
{
    delete ui;
}

bool QClickableLineEdit::eventFilter(QObject *object, QEvent *event)
{
    if(object == this && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mEvent = static_cast<QMouseEvent*>(event);
        if(mEvent->button()==Qt::LeftButton)
            _mousePressed = true;
        return false; // lets the event continue to the edit
    }else if(object == this && event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mEvent = static_cast<QMouseEvent*>(event);
        if(mEvent->button()==Qt::LeftButton)
        {
            if(_mousePressed)
            {
                _mousePressed = false;
                emit clicked();
            }
        }
    }
    return false;
}
