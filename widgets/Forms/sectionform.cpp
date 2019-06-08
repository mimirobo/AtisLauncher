#include "sectionform.h"
#include "ui_sectionform.h"

//Ui
#include "itemform.h"

//Qt Drag and Drop
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
//Qt
#include <QMessageBox>
#include <QDebug>

//Qt Json
#include <QJsonArray>

SectionForm::SectionForm(const QString &name,
                         const QString &profile,
                         QWidget *parent) :
    QWidget(parent),
    mSection(name),
    mProfile(profile),
    ui(new Ui::SectionForm)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    ui->groupBox->setTitle(name);
}

SectionForm::~SectionForm()
{
    qDeleteAll(items);
    delete ui;
}

QJsonObject SectionForm::Serialize() const
{
    QJsonObject section_JObj;
    section_JObj.insert("SectionName", mSection);

    QJsonArray items_arr_json;

    foreach (auto item_key, items.keys())
    {
        items_arr_json.append(items[item_key]->Serialize());
    }
    section_JObj.insert("Items", items_arr_json);
    return section_JObj;
}

ItemForm *SectionForm::AddItem(const QString &caption,
                               const Utilities::TaskTypes &taskType,
                               const ParamsType &params,
                               const int &delay)
{
    if(items.contains(caption))
        return nullptr;
    ItemForm *item = ItemForm::Builder()
            .setTaskType(taskType)
            .setCaption(caption)
            .setSection(mSection)
            .setProfile(mProfile)
            .setParams(params)
            .setRunDelay(delay)
            .setParent(this)
            .build();
    items.insert(caption, item);

    if (ui->layout->count() == 1)
        ui->layout->insertWidget(0, item);
    else
        ui->layout->insertWidget(ui->layout->count() - 1, item);
    return item;
}

ItemForm *SectionForm::AddItem(ItemForm *item)
{
    item->setParent(this);
    items.insert(item->caption(), item);
    item->getModel()->setSection(mSection);
    if (ui->layout->count() == 1)
        ui->layout->insertWidget(0, item);
    else
        ui->layout->insertWidget(ui->layout->count() - 1, item);
    return item;
}

void SectionForm::RemoveItem(const QString &caption)
{
    if(!items.contains(caption))
        return;
    ItemForm *item = items.take(caption);
    item->PlayRemoveAnimation();
    connect(item, &ItemForm::FinalAnimationFinished, [this,item,caption]()
    {
        ui->layout->removeWidget(item);
        item->deleteLater();
    });
}

ItemForm *SectionForm::TakeItem(const QString &caption)
{
    if(!items.contains(caption))
        return nullptr;
    ItemForm *item = items.take(caption);
    item->PlayRemoveAnimation();
    QMetaObject::Connection * const connection = new QMetaObject::Connection;
    *connection = connect(item, &ItemForm::FinalAnimationFinished, [this,item,caption, connection]()
    {
        ui->layout->removeWidget(item);
        QObject::disconnect(*connection);
        delete connection;
    });
    return item;
}

bool SectionForm::RenameItem(const QString &old_caption, const QString &new_caption)
{
    if(items.contains(new_caption))
    {
        QMessageBox msgbox;
        msgbox.setText("This name already exists. try another caption.");
        msgbox.exec();
        items.value(old_caption)->RecaptionSilent(old_caption);
        return false;
    }
    ItemForm *item = items.take(old_caption);
    item->RecaptionSilent(new_caption);
    items.insert(new_caption, item);
    item->getModel()->setCaption(new_caption);
    return true;
}

void SectionForm::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::RightButton)
        return;

    QWidget *child = childAt(event->pos());
    ItemForm *item = nullptr;
    if (child != 0)
    {
        QString classname(child->metaObject()->className());
        QString parentClassName(child->parentWidget()->metaObject()->className());
        if (classname == "QFrame")
        {
            item = qobject_cast<ItemForm *>(childAt(event->pos())->parentWidget());
        }
        else if (parentClassName == "QFrame")
        {
            item = qobject_cast<ItemForm *>(childAt(event->pos())->parentWidget()->parentWidget());
        }else if (parentClassName == "EditableLabel")
        {
            item = qobject_cast<ItemForm *>(childAt(event->pos())->parentWidget()->parentWidget()->parentWidget());
        }
    }
    if (!item)
        return;

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(item->caption());
    QByteArray bytes;
    bytes.append(mSection);
    mimeData->setData("text", bytes);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap((item->getIcon()));
    drag->setHotSpot(QPoint(drag->pixmap().width()/2,drag->pixmap().height()/2));
    //Qt::DropAction dropAction = drag->exec(Qt::MoveAction, Qt::MoveAction);
    drag->exec(Qt::MoveAction, Qt::MoveAction);
    //drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::MoveAction);
}

void SectionForm::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText())
    {
        if (event->source() != this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->ignore();
    }
}

void SectionForm::dropEvent(QDropEvent *event)
{
    //to do: make sure that it is dragged from the current app
    if (event->mimeData()->hasText())
    {
        const QMimeData *mime = event->mimeData();
        QString source_caption = mime->text();
        QString source_section = QString(mime->data("text").split(' ').first());

        if (event->source() != this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
        emit requestDragDropTransfer(source_caption, source_section,
                                     mSection, mProfile);
    }
    else
    {
        event->ignore();
    }
}
