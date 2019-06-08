#ifndef ITEMFORM_H
#define ITEMFORM_H

//Qts
#include <QWidget>
#include <QJsonObject>

#include <Utils/utilities.h>
#include "model/itemmodel.h"

namespace Ui {
class ItemForm;
}

class ItemForm : public QWidget
{
    Q_OBJECT

public:
    class Builder;
    enum class ItemType{
        Setup,
        Runtime
    };
    enum class UserItemType{
        AtisItem = 1001
    };
    //setup item constructor
    explicit ItemForm(const QString &caption,
                      const QString &section,
                      const QString &profile,
                      const bool &is_selected,
                      const Utilities::TaskTypes &task_type,
                      const ParamsType &params,
                      const int &run_delay = 0,
                      QWidget *parent = 0);
    //runitem constructor
    explicit ItemForm(ItemModel *model,
                      QWidget *parent = 0);
    //general initialization
    void init();
    //destructor
    ~ItemForm();
    //getters
    const QPixmap &getIcon()const;
    ItemModel *getModel() const;
    inline QString caption() const {return model->caption();}
    //setters
    void setSelected(bool selected);
    //others
    void PlayRemoveAnimation();
    void RecaptionSilent(const QString &new_caption);
    void ChangeRuntimeSilent(bool state);
    void setRunDelay(int delay);

    QJsonObject Serialize() const;

signals:
    void ItemRuntimeStateChanged(QString caption, QString section, QString profile, bool state);
    void ItemRemoveRequested(QString caption, QString section, QString profile);
    void FinalAnimationFinished();
    void ItemRenameRequested(QString old_name, QString new_name,
                             QString section, QString profile);
    void ItemEditRequested(QString caption, QString section,
                           QString profile);
private slots:
    void on_delBtn_clicked();
    void showContextMenu(const QPoint &pos);
    void on_captionBox_toggled(bool checked);

    void on_delayBox_valueChanged(int arg1);

    void on_openBtn_clicked();

    void on_playBtn_clicked();

private:
    void InitializeType();
private:
    ItemModel *model;
    Ui::ItemForm *ui;
    ItemType mItemType;
};


class ItemForm::Builder
{
private:
    int run_delay;
    bool isSelected;
    ItemModel *model;
    Utilities::TaskTypes task_type;
    ItemForm::ItemType item_type;//setup or runtime
    QWidget *parent;
    QString caption;
    QString profile;
    QString section;
    QMap<QString,QStringList> params;

public:
    Builder() : run_delay(0),isSelected(false), model(nullptr){}
    Builder &setCaption(const QString &caption)
    {
        this->caption = caption;
        return *this;
    }
    Builder &setProfile(const QString &profile)
    {
        this->profile = profile;
        return *this;
    }
    Builder &setSection(const QString &section)
    {
        this->section = section;
        return *this;
    }
    Builder &setTaskType(const Utilities::TaskTypes &type)
    {
        this->task_type = type;
        return *this;
    }
    Builder &setIsSelected(const bool &isSelected)
    {
        this->isSelected = isSelected;
        return *this;
    }
    Builder &setParent(QWidget *parent)
    {
        this->parent = parent;
        return *this;
    }
    Builder &setRunDelay(const int &run_delay)
    {
        this->run_delay = run_delay;
        return *this;
    }
    Builder &setParams(const ParamsType &params)
    {
        this->params = params;
        return *this;
    }

    Builder &setModel(ItemModel *model)
    {
        this->model = model;
        return *this;
    }
    Builder &setItemType(const ItemForm::ItemType &type)
    {
        this->item_type = type;
        return *this;
    }
    //etc

    ItemForm *build()
    {
        if(model == nullptr)
            return new ItemForm(caption, section, profile, isSelected,
                            task_type, params, run_delay, parent);

        return new ItemForm(model, parent);
    }
};

#endif // ITEMFORM_H
