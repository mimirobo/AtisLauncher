#include "runtimeform.h"
#include "ui_runtimeform.h"
//view
#include "itemform.h"
//model
#include "model/itemmodel.h"
//Qts
#include <QDebug>
#include <QJsonArray>
#include <QListWidgetItem>

RuntimeForm::RuntimeForm(const QString &profile_name, QWidget *parent) :
    BaseForm(parent),
    ui(new Ui::RuntimeForm),
    mProfile(profile_name)
{
    ui->setupUi(this);
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    connect(ui->listWidget->model(), &QAbstractItemModel::rowsMoved,
            [this](const QModelIndex &parent,
            int start, int end,
            const QModelIndex &destination, int row)
    {
        Q_UNUSED(parent);
        Q_UNUSED(end);
        Q_UNUSED(destination);
        //qDebug()<<"Item at "<<start<<" moved to row "<<row;
        MoveRunItem(start, row);
    });
}

RuntimeForm::~RuntimeForm()
{
    delete ui;
}

ItemForm *RuntimeForm::AddItem(ItemModel *model)
{
    ItemForm *item_widget = ItemForm::Builder()
            .setItemType(ItemForm::ItemType::Runtime)
            .setModel(model)
            .setParent(ui->listWidget)
            .build();
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(item_widget->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, item_widget);
    items_widget.insert(model->caption(), item_widget);
    items.insert(model->caption(), item);
    runItems.append(model);
    return item_widget;
}

void RuntimeForm::RemoveItem(const QString &caption)
{
    if(!items_widget.contains(caption))
        return;
    ItemForm *item_widget = items_widget.take(caption);
    runItems.removeAll(item_widget->getModel());
    QListWidgetItem *item = items.take(caption);
    ui->listWidget->removeItemWidget(item);
    item_widget->deleteLater();
    delete item;
}

void RuntimeForm::RenameItem(const QString &old_caption, const QString &new_caption)
{
    if(!items_widget.contains(old_caption))
        return;
    if(items_widget.contains(new_caption))
        return;
    ItemForm *item_widget = items_widget.take(old_caption);
    QListWidgetItem *item_list = items.take(old_caption);
    item_widget->RecaptionSilent(new_caption);
    items_widget.insert(new_caption, item_widget);
    items.insert(new_caption, item_list);
}

void RuntimeForm::MoveRunItem(int index, int dest_index)
{
    //qDebug()<<"Move from "<<index<<" to "<<dest_index;
    runItems.insert(dest_index, runItems.takeAt(index));
}

void RuntimeForm::on_pushButton_clicked()
{
    emit runAllRequested(mProfile);
}

QList<ItemModel *> RuntimeForm::getRunItems() const
{
    return runItems;
}

QJsonObject RuntimeForm::Serialize() const
{
    QStringList all_captions;
    QStringList all_sections;
    for(auto item : runItems)
    {
        all_captions.append(item->caption());
        all_sections.append(item->section());
    }
    QJsonArray items_Jarr = QJsonArray::fromStringList(all_captions);
    QJsonArray itemsSection_Jarr = QJsonArray::fromStringList(all_sections);
    QJsonObject obj;
    obj.insert("RuntimeItems", items_Jarr);
    obj.insert("RuntimeSections", itemsSection_Jarr);
    return obj;
}

QString RuntimeForm::profile() const
{
    return mProfile;
}
