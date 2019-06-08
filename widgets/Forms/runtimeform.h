#ifndef RUNTIMEFORM_H
#define RUNTIMEFORM_H

#include "baseform.h"
#include "Utils/utilities.h"
//Qts
#include <QHash>
#include <QJsonObject>

class ItemForm;
class ItemModel;
class QListWidgetItem;

namespace Ui {
class RuntimeForm;
}

class RuntimeForm : public BaseForm
{
    Q_OBJECT
public:
    explicit RuntimeForm(const QString &profile_name,
                       QWidget *parent = 0);
    ~RuntimeForm();

    ItemForm *AddItem(ItemModel *model);
    void RemoveItem(const QString &caption);
    void RenameItem(const QString &old_caption,
                    const QString &new_caption);
    void MoveRunItem(int index, int dest_index);

    //Getters
    ItemForm* Item(const QString &caption)
    {return (items_widget.contains(caption))?items_widget[caption]:nullptr;}
    QString profile() const;
    QList<ItemModel *> getRunItems() const;
    QJsonObject Serialize() const;

signals:
    void runAllRequested(const QString &profile);
private slots:
    void on_pushButton_clicked();

private:
    Ui::RuntimeForm *ui;
    QString mProfile;
    //Ui Queue
    QHash<QString, ItemForm*> items_widget;
    QHash<QString, QListWidgetItem*> items;
    //Internal Queue
    QList<ItemModel*> runItems;
};

#endif // RUNTIMEFORM_H
