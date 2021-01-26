#include "inputtextdialog.h"
#include "ui_inputtextdialog.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalMapper>
#include <QSizePolicy>

#define NEXT_ROW_MARKER 0
#define WHITESPACE_MARKER -1
struct KeyboardLayoutEntry{
    int key;
    const char16_t *label;
};

KeyboardLayoutEntry keyboardLayout[] = {
    { Qt::Key_1, u"1" },
    { Qt::Key_2, u"2" },
    { Qt::Key_3, u"3" },
    { Qt::Key_4, u"4" },
    { Qt::Key_5, u"5" },
    { Qt::Key_6, u"6" },
    { Qt::Key_7, u"7" },
    { Qt::Key_8, u"8" },
    { Qt::Key_9, u"9" },
    { Qt::Key_0, u"0" },
    { Qt::Key_Backspace, u"<-" },
    { NEXT_ROW_MARKER, 0 },
    { Qt::Key_Q, u"Й" },
    { Qt::Key_W, u"Ц" },
    { Qt::Key_E, u"У" },
    { Qt::Key_R, u"К" },
    { Qt::Key_T, u"Е" },
    { Qt::Key_Z, u"Н" },
    { Qt::Key_U, u"Г" },
    { Qt::Key_I, u"Ш" },
    { Qt::Key_O, u"Щ" },
    { Qt::Key_P, u"З" },
    { Qt::Key_BracketLeft, u"Х" },
    { Qt::Key_BracketRight, u"Ъ" },
    { NEXT_ROW_MARKER, 0 },
    { Qt::Key_A, u"Ф" },
    { Qt::Key_S, u"Ы" },
    { Qt::Key_D, u"В" },
    { Qt::Key_F, u"А" },
    { Qt::Key_G, u"П" },
    { Qt::Key_H, u"Р" },
    { Qt::Key_J, u"О" },
    { Qt::Key_K, u"Л" },
    { Qt::Key_L, u"Д" },
    { Qt::Key_Semicolon, u"Ж" },
    { Qt::Key_QuoteDbl, u"Э" },
    { NEXT_ROW_MARKER, 0 },
    { Qt::Key_Y, u"Я" },
    { Qt::Key_X, u"Ч" },
    { Qt::Key_C, u"С" },
    { Qt::Key_V, u"М" },
    { Qt::Key_B, u"И" },
    { Qt::Key_N, u"Т" },
    { Qt::Key_M, u"Ь" },
    { Qt::Key_Comma, u"Б" },
    { Qt::Key_Period, u"Ю" },
    { Qt::Key_Slash, u"." },
    { Qt::Key_Enter, u"Enter" },
    { NEXT_ROW_MARKER, 0 },
    { Qt::Key_Space, u" " }
};

const static int layoutSize = (sizeof(keyboardLayout) / sizeof(KeyboardLayoutEntry));

static QString keyToCharacter(int key)
{
    for (int i = 0; i < layoutSize; ++i) {
        if (keyboardLayout[i].key == key)
            return QString::fromUtf16(keyboardLayout[i].label);
    }

    return QString();
}


InputTextDialog::InputTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputTextDialog)
{
    ui->setupUi(this);

    QGridLayout *grid = new QGridLayout(this);
    setLayout(grid);
    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(int)), SLOT(buttonClicked(int)));

    textbox = new QLineEdit(this);

    grid->addWidget(textbox,0,0,1,7);
    int row = 1;
    int column = 1;
    grid->addItem(new QSpacerItem(0,10, QSizePolicy::Expanding, QSizePolicy::Expanding),0,0);
    for (int i = 0; i < layoutSize; ++i) {
        if (keyboardLayout[i].key == NEXT_ROW_MARKER) {
            row++;
            column = 0;

            QSpacerItem *spacer = new QSpacerItem(0,10, QSizePolicy::Expanding, QSizePolicy::Expanding);

            grid->addItem(spacer, row, column++);
            continue;
        }

        QPushButton *button = new QPushButton(this);
        //button->setLayout(grid);
        button->setFixedWidth(50);
        button->setFixedHeight(50);
        button->setText(QString::fromUtf16(keyboardLayout[i].label));
        QFont font = button->font();
        font.setWeight(16);
        button->setFont(font);
        mapper->setMapping(button, keyboardLayout[i].key);
        connect(button, SIGNAL(clicked()), mapper, SLOT(map()));

        if(keyboardLayout[i].key==Qt::Key_Backspace)
        {
            QIcon *icon = new QIcon(":/images/backspace-arrow.png");
            button->setIcon(*icon);
            button->setIconSize(QSize(24,24));
            button->setText("");
            button->setFixedWidth(100);
            grid->addWidget(button, row, column,1,2,Qt::AlignHCenter);
        }
        else if(keyboardLayout[i].key==Qt::Key_Enter)
        {
            button->setFixedWidth(100);
            grid->addWidget(button, row, column,1,2,Qt::AlignHCenter);
        }
        else if(keyboardLayout[i].key==Qt::Key_Space)
        {
            button->setFixedWidth(250);
           //button->setLayout(grid);
            grid->addWidget(button, row, column+2,1,5,Qt::AlignHCenter);
        }
        else
        {
            grid->addWidget(button, row, column);
        }
        column++;
    }
}

InputTextDialog::~InputTextDialog()
{
    delete ui;
}

QString InputTextDialog::getText()
{
    return textbox->text();
}

void InputTextDialog::setText(QString text)
{
   textbox->setText(text);
}

void InputTextDialog::buttonClicked(int key)
{
    if(key==Qt::Key_Enter)
    {
        accept();
        return;
    }
    else if(key==Qt::Key_Backspace)
    {
        QString txt = textbox->text();
        txt.chop(1);
        textbox->setText(txt);
    }
    else
    {
        QString character = keyToCharacter(key);
        textbox->setText(textbox->text()+character);
    }
}

void InputTextDialog::createKeyboard()
{



}
