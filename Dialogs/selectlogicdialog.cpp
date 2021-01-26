#include "selectlogicdialog.h"
#include "ui_selectlogicdialog.h"

SelectLogicDialog::SelectLogicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectLogicDialog)
{
    ui->setupUi(this);
    ui->cbLogic->addItem("Набивка");
    ui->cbLogic->addItem("Порционирование");
    ui->cbLogic->addItem("Перекрут");

}

SelectLogicDialog::~SelectLogicDialog()
{
    delete ui;
}

Logic SelectLogicDialog::getSelectedLogic()
{
    if(ui->cbLogic->currentIndex()==0)
        return Logic::Stuffing;
    else if(ui->cbLogic->currentIndex()==1)
        return Logic::Portion;
    else if(ui->cbLogic->currentIndex()==2)
        return Logic::Swiper;

}

void SelectLogicDialog::on_btnPrev_released()
{
    reject();
}

void SelectLogicDialog::on_btnNext_released()
{
    _selectedLogic = getSelectedLogic();
    accept();
}


