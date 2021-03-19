#include "pressetmanagerframe.h"
#include "ui_pressetmanagerframe.h"

#include <setting/settingstorage.h>

#include <Dialogs/editportionpressetdialog.h>
#include <Dialogs/editstuffingpressetdialog.h>
#include <Dialogs/editswiperpressetdialog.h>
#include <Dialogs/questiondialog.h>
#include <Dialogs/selectlogicdialog.h>

PressetManagerFrame::PressetManagerFrame(QWidget *parent) :
    ConfigFrameBase(nullptr, parent),
    ui(new Ui::PressetManagerFrame)
{
    ui->setupUi(this);
    model = new PressetsModel();

    model->populateData(SettingStorage::instance()->Pressets());

    ui->tableView->setModel((QAbstractItemModel*)model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

PressetManagerFrame::~PressetManagerFrame()
{
    delete ui;
}

void PressetManagerFrame::on_btnAddPresset_released()
{
    SelectLogicDialog *selectLogic = new SelectLogicDialog(this);

    EditPressetDialog *editPresset = nullptr;
    //PressetConfig *config;
    if(selectLogic->exec()==QDialog::Accepted)
    {
        switch (selectLogic->getSelectedLogic()) {
        case Logic::Stuffing:
            editPresset = new EditStuffingPressetDialog(this);
            break;
        case Logic::Portion:
            editPresset = new EditPortionPressetDialog(this);
            break;
        case Logic::Swiper:
            editPresset = new EditSwiperPressetDialog(this);
            break;
        case Logic::None:
            editPresset = nullptr;
            break;
        }

        if(editPresset!=nullptr)
        {
            if(editPresset->exec()==QDialog::Accepted)
            {
                SettingStorage::instance()->SaveSettings();
                model->insertRow(1);
                model->populateData(SettingStorage::instance()->Pressets());
                ui->tableView->setModel(model);
            }
            delete editPresset;
        }
    }
    delete selectLogic;
}

void PressetManagerFrame::on_btnEditPresset_released()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    QString pressetName;

    QModelIndex index = selection.at(0);

    pressetName = index.sibling(index.row(),0).data().toString();

    PressetConfig *config = SettingStorage::instance()->getPressetConfig(pressetName);
    EditPressetDialog *dialog = nullptr;
    switch (static_cast<Logic>(config->Logic())) {
    case Logic::Swiper:
        dialog=new EditSwiperPressetDialog(this);
        break;
    case Logic::Portion:
        dialog=new EditPortionPressetDialog(this);
        break;
    case Logic::Stuffing:
        dialog=new EditStuffingPressetDialog(this);
        break;
    case Logic::None:
        dialog = nullptr;
        break;
    }
    if(dialog!=nullptr)
    {
        dialog->setConfig(config);
        if(dialog->exec()==QDialog::Accepted)
        {

            SettingStorage::instance()->SaveSettings();
            model->insertRow(1);
            model->populateData(SettingStorage::instance()->Pressets());
            ui->tableView->setModel(model);
        }
    }
}

void PressetManagerFrame::on_btnRemovePresset_released()
{
    QuestionDialog *question = new QuestionDialog(this);

    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    QString pressetName;

    QModelIndex index = selection.at(0);

    pressetName = index.sibling(index.row(),0).data().toString();
    question->setText("Вы действительно хотите удалить \r\n"+pressetName);
    if(question->exec()==QDialog::Accepted)
    {
        SettingStorage::instance()->removePressetConfig(pressetName);
        model->populateData(SettingStorage::instance()->Pressets());
        model->removeRows(index.row(),1);
        ui->tableView->setModel(model);
        SettingStorage::instance()->SaveSettings();
    }
}
