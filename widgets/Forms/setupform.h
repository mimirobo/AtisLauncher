#ifndef SETUPFORM_H
#define SETUPFORM_H

#include "baseform.h"
#include <QHash>
#include <QJsonObject>

class SectionForm;

namespace Ui {
class SetupForm;
}

class SetupForm : public BaseForm
{
    Q_OBJECT

public:
    explicit SetupForm(const QString &profile_name,
                       QWidget *parent = 0);
    ~SetupForm();

    inline SectionForm* getSection(const QString &section_name)
    {return (sectionWidgets.contains(section_name))?sectionWidgets[section_name]:nullptr;}

    SectionForm* AddSection(const QString &section_name);
    void RemoveSection(QString section_name);

    QJsonObject Serialize() const;

private:
    Ui::SetupForm *ui;
    QString mProfile;
    int row,col;
    QHash<QString, SectionForm*> sectionWidgets;
};

#endif // SETUPFORM_H
