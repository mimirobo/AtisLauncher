#include "setupform.h"
#include "ui_setupform.h"

#include <QJsonArray>
#include "widgets/Forms/sectionform.h"

SetupForm::SetupForm(const QString &profile_name, QWidget *parent) :
    BaseForm(parent),
    ui(new Ui::SetupForm),
    mProfile(profile_name),
    row(0),
    col(0)
{
    ui->setupUi(this);
}

SetupForm::~SetupForm()
{
    qDeleteAll(sectionWidgets);
    delete ui;
}

SectionForm *SetupForm::AddSection(const QString &section_name)
{
    SectionForm *section = new SectionForm(section_name,mProfile, this);
    sectionWidgets.insert(section_name, section);
    ui->gridLayout->addWidget(section, row, col);
    col++;
    if(col==2)
    {
        row++;col=0;
    }
    return section;
}

void SetupForm::RemoveSection(QString section_name)
{
    if(!sectionWidgets.contains(section_name))
        return;
    SectionForm *section = sectionWidgets.take(section_name);
    section->deleteLater();
}

QJsonObject SetupForm::Serialize() const
{
    QJsonObject obj;
    QJsonArray sections_arr_json;
    foreach (auto item_key, sectionWidgets.keys())
    {
        sections_arr_json.append(sectionWidgets[item_key]->Serialize());
    }
    obj.insert("Sections", sections_arr_json);
    return obj;
}
