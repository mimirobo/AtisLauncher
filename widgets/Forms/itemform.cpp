#include "itemform.h"
#include "ui_itemform.h"
//Qt
#include <QUrl>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QDesktopServices>
//Qt Animations
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
//Qt Json
#include <QJsonArray>
//Process
#include "process/processmanager.h"
//Material
#include "qtmaterialcircularprogress.h"

//constructor for setup items
ItemForm::ItemForm(const QString &caption,
                   const QString &section,
                   const QString &profile,
                   const bool &is_selected,
                   const Utilities::TaskTypes &task_type,
                   const ParamsType &params,
                   const int &run_delay,
                   QWidget *parent) :
    QWidget(parent),
    model(new ItemModel(caption,
                        section,
                        profile,
                        task_type,
                        params,
                        is_selected,
                        run_delay,
                        this)),
    ui(new Ui::ItemForm),
    mItemType(ItemType::Setup)
{
    init();
}

//constructor for runtime items
ItemForm::ItemForm(ItemModel *model, QWidget *parent) :
    QWidget(parent),
    model(model),
    ui(new Ui::ItemForm),
    mItemType(ItemType::Runtime)
{
    init();
}

void ItemForm::init()
{
    ui->setupUi(this);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ItemForm::customContextMenuRequested, this, &ItemForm::showContextMenu);
    ui->editable_lbl->setCaption(model->caption());
    ui->delayBox->setValue(model->runDelay());

    InitializeType();
}

ItemForm::~ItemForm()
{
    if(mItemType == ItemType::Setup)
        model->deleteLater();
    delete ui;
}

void ItemForm::on_delBtn_clicked()
{
    emit ItemRemoveRequested(model->caption(), model->section(), model->profile());
}

void ItemForm::showContextMenu(const QPoint &pos)
{
    if(mItemType == ItemType::Setup)
        showContextMenuForSetupItem(pos);
    else
        showContextMenuForRunItem(pos);
}

void ItemForm::InitializeType()
{
    switch (this->mItemType)
    {
    case ItemForm::ItemType::Setup:
        connect(ui->editable_lbl, &EditableLabel::CaptionChanged,
                [this](QString new_caption)
        {
            if(!new_caption.compare(model->caption()))
                return;

            emit ItemRenameRequested(model->caption(),
                                     new_caption,
                                     model->section(),
                                     model->profile());
        });
        ui->delayBox->hide();
        ui->delay_cap->hide();
        ui->playBtn->hide();

        ui->loadingWidget->hide();
        break;
    case ItemForm::ItemType::Runtime:
        ui->delayBox->blockSignals(true);
        ui->delayBox->setValue(model->runDelay());
        ui->delayBox->blockSignals(false);
        ui->captionBox->hide();
        //setRunDelay(mRunDelay);
        ui->editable_lbl->setEditable(false);
        ui->delBtn->setVisible(false);

        ui->loadingWidget->setSize(25);
        ui->loadingWidget->setProgressType(Material::DeterminateProgress);
        ui->loadingWidget->setValue(0);
        break;
    }

    switch (model->type())
    {
    case Utilities::TaskTypes::Roslaunch:
        ui->icon_lbl->setPixmap(QPixmap(":icons/new_task/Types32/roslaunch_32px.png"));
        break;
    case Utilities::TaskTypes::Rosrun:
        ui->icon_lbl->setPixmap(QPixmap(":icons/new_task/Types32/rosrun_32px.png"));
        ui->openBtn->hide();
        break;
    case Utilities::TaskTypes::Command:
        ui->icon_lbl->setPixmap(QPixmap(":icons/new_task/Types32/command_32px.png"));
        ui->openBtn->hide();
        break;
    case Utilities::TaskTypes::Script:
        ui->icon_lbl->setPixmap(QPixmap(":icons/new_task/Types32/script_32px.png"));
        break;
    }
}

ItemModel *ItemForm::getModel() const
{
    return model;
}

void ItemForm::RecaptionSilent(const QString &new_caption)
{
    ui->editable_lbl->setCaption(new_caption);
    model->setCaption(new_caption);
}

void ItemForm::ChangeRuntimeSilent(bool state)
{
    model->setRuntime(state);
    ui->captionBox->blockSignals(true);
    ui->captionBox->setChecked(state);
    ui->captionBox->blockSignals(false);
}

void ItemForm::setRunDelay(int delay)
{
    ui->delayBox->setValue(delay);
}

QJsonObject ItemForm::Serialize() const
{
    QJsonObject params_Jobj = QJsonObject();

    for (QString param_name : model->params().keys())
    {
        QJsonArray params_value_Jarr = QJsonArray::fromStringList(model->params()[param_name]);
        params_Jobj.insert(param_name, params_value_Jarr);
    }
    QJsonObject runOption_Jobj = QJsonObject({{"Delay", model->runDelay()},
                                              {"InRuntime", model->isInRuntime()}});
    QJsonObject obj = {{"Caption", model->caption()}, {"Section", model->section()},
                       {"Type", (int)model->type()}};
    obj.insert("Data", params_Jobj);
    obj.insert("RunOptions", runOption_Jobj);
    return obj;
}

void ItemForm::setSelected(bool selected)
{
    ui->captionBox->setChecked(selected);
}

void ItemForm::setWaitForRun(bool wait)
{
    if(wait)
    {
        ui->loadingWidget->setProgressType(Material::IndeterminateProgress);
        ui->frame->setStyleSheet("#frame {border: 2px dashed orange;}");
    }else
    {
        ui->loadingWidget->setProgressType(Material::DeterminateProgress);
        ui->loadingWidget->setValue(0);
        ui->frame->setStyleSheet("");
    }
}

void ItemForm::PlayRemoveAnimation()
{
    QPropertyAnimation *move_anim = new QPropertyAnimation(this, "geometry");
    move_anim->setDuration(300);
    move_anim->setStartValue(this->geometry());
    move_anim->setEndValue(QRect(-this->width(),this->y(),this->width(),this->height()));
    move_anim->setEasingCurve(QEasingCurve::OutBack);

    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(eff);
    QPropertyAnimation *fade_anim = new QPropertyAnimation(eff,"opacity");
    fade_anim->setDuration(300);
    fade_anim->setStartValue(1);
    fade_anim->setEndValue(0);
    fade_anim->setEasingCurve(QEasingCurve::OutBack);


    QParallelAnimationGroup *par_anim = new QParallelAnimationGroup();
    par_anim->addAnimation(fade_anim);
    par_anim->addAnimation(move_anim);
    par_anim->start(QAbstractAnimation::DeleteWhenStopped);
    connect(par_anim, &QParallelAnimationGroup::finished,this,
            [this,eff]()
    {
        eff->deleteLater();
        emit FinalAnimationFinished();
    });
}

const QPixmap &ItemForm::getIcon() const
{
    return *(ui->icon_lbl->pixmap());
}

void ItemForm::on_captionBox_toggled(bool checked)
{
    model->setRuntime(checked);
    Q_EMIT ItemRuntimeStateChanged(model->caption(),
                                   model->section(),
                                   model->profile(),
                                   checked);
}

void ItemForm::on_delayBox_valueChanged(int arg1)
{
    model->setRunDelay(arg1);
}

void ItemForm::on_openBtn_clicked()
{
    QString path = model->params()[Utilities::FULLPATH][0];
    QDesktopServices::openUrl(QUrl("file://"+path));
}

void ItemForm::on_playBtn_clicked()
{
    ProcessManager::getInstance()->RunSingle(model);
}

void ItemForm::showContextMenuForSetupItem(const QPoint &pos)
{
    QMenu contextMenu("Item Menu",this);
    QAction copyCMDAction("Copy Command", this);
    QAction editCMDAction("Edit", this);
    QAction deleteCMDAction("Delete", this);


    connect(&editCMDAction, &QAction::triggered, [this]()
    {
        emit ItemEditRequested(model->caption(), model->section(), model->profile());
    });

    connect(&copyCMDAction, &QAction::triggered, [this]()
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(ProcessManager::getInstance()->generateCommand(model));
    });

    connect(&deleteCMDAction, &QAction::triggered, [this]()
    {
        on_delBtn_clicked();
    });

    contextMenu.addAction(&copyCMDAction);
    contextMenu.addSeparator();
    contextMenu.addAction(&editCMDAction);
    contextMenu.addSeparator();
    contextMenu.addAction(&deleteCMDAction);
    contextMenu.exec(mapToGlobal(pos));
}

void ItemForm::showContextMenuForRunItem(const QPoint &pos)
{
    QMenu contextMenu("Item Menu",this);
    QAction runCMDAction("Run", this);
    QAction copyCMDAction("Copy Command", this);

    connect(&runCMDAction, &QAction::triggered, [this]()
    {
        on_playBtn_clicked();
    });

    connect(&copyCMDAction, &QAction::triggered, [this]()
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(ProcessManager::getInstance()->generateCommand(model));
    });

    contextMenu.addAction(&runCMDAction);
    contextMenu.addSeparator();
    contextMenu.addAction(&copyCMDAction);
    contextMenu.exec(mapToGlobal(pos));
}
