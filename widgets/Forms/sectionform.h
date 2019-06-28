#ifndef SECTIONFORM_H
#define SECTIONFORM_H

#include <QWidget>
#include <QHash>
#include "Utils/utilities.h"

#include <QJsonObject>

class ItemForm;
class QDragEnterEvent;
class QDropEvent;

namespace Ui {
class SectionForm;
}

class SectionForm : public QWidget
{
    Q_OBJECT

public:
    explicit SectionForm(const QString &name,
                         const QString &profile,
                         QWidget *parent = 0);
    ~SectionForm();

    inline bool ContainsItem(const QString &caption) {return items.contains(caption);}
    QStringList getAllItemsCaption()const {return items.keys();}
    ItemForm* Item(const QString &caption)
    {return (items.contains(caption))?items[caption]:nullptr;}

    QJsonObject Serialize()const;
    QString section() const;

public slots:
    ItemForm* AddItem(const QString &caption,
                      const Utilities::TaskTypes &taskType,
                      const ParamsType &params,
                      const int &delay=0);
    //AddItem for drag and drop
    ItemForm *AddItem(ItemForm* item_ui);
    void RemoveItem(const QString &caption);
    ItemForm *TakeItem(const QString &caption);
    bool RenameItem(const QString &old_caption,
                    const QString &new_caption);
signals:
    void requestDragDropTransfer(QString source_caption, QString source_section,
                                 QString dest_section, QString profile);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private:
    QString mSection;
    QString mProfile;
    Ui::SectionForm *ui;
    QHash<QString, ItemForm*> items;
};

#endif // SECTIONFORM_H
