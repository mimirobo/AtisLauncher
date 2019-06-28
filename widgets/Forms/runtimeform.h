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
class SearchForm;
class QtMaterialIconButton;
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

    void initSearchTool();
    void initSearchFilter();

    ItemForm *AddItem(ItemModel *model);
    void RemoveItem(const QString &caption);
    void RenameItem(const QString &old_caption,
                    const QString &new_caption);
    void MoveRunItem(int index, int dest_index);
    void ChangeWaitForRunItem(const QString &caption,
                              bool waiting);
    void RequestRunAll();
    //Search Tool
    void toggleSearchTool();
    void FilterByCaption(const QString &filter_string);
    void HideAll();
    //Getters
    ItemForm* Item(const QString &caption)
    {return (items_widget.contains(caption))?items_widget[caption]:nullptr;}
    QString profile() const;
    QList<ItemModel *> getRunItems() const;
    QJsonObject Serialize() const;
    SearchForm *getSearchWidget() const;
signals:
    void runAllRequested(const QString &profile);
private slots:
    void on_pushButton_clicked();
public slots:
    void onRunAllBegan();
    void onRunAllFinished();
    void DisplaySearchBar(bool display);
protected:
    void resizeEvent(QResizeEvent *event);
private:
    Ui::RuntimeForm *ui;
    QString mProfile;
    //Ui Queue
    QHash<QString, ItemForm*> items_widget;
    QHash<QString, QListWidgetItem*> items;
    //Internal Queue
    QList<ItemModel*> runItems;
    //Search
    SearchForm *searchWidget;
    QtMaterialIconButton *clearFilterBtn;
};

#endif // RUNTIMEFORM_H
