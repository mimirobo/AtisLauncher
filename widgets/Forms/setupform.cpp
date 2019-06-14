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

void SetupForm::preAllocateGrid(int rows, int cols)
{
    maxRow = rows;
    maxCol = cols;
}

SectionForm *SetupForm::AddSection(const QString &section_name)
{
    SectionForm *section = new SectionForm(section_name,mProfile, this);
    sectionWidgets.insert(section_name, section);
    ui->gridLayout->addWidget(section, row, col);
    col++;
    if(col==maxCol)
    {
        row++;col=0;
        maxRow++;
    }
    return section;
}

SectionForm *SetupForm::InsertSection(const QString &section_name, int row, int col)
{
    SectionForm *section = new SectionForm(section_name,mProfile, this);
    sectionWidgets.insert(section_name, section);
    ui->gridLayout->addWidget(section, row, col);
    this->row = row;
    this->col = col;
    return section;
}

void SetupForm::RemoveSection(QString section_name)
{
    if(!sectionWidgets.contains(section_name))
        return;
    SectionForm *section = sectionWidgets.take(section_name);
    section->deleteLater();
    //to do : handle maxRow, maxCol
}

QJsonObject SetupForm::Serialize() const
{
    QJsonObject obj;
    QJsonArray sections_arr_json;

    QJsonObject gridJObj;
    QJsonObject sectionsorder_obj;

    for(int index = 0; index < ui->gridLayout->count();index++)
    {
        int col=0;
        int row=0;
        int cs,rs;
        ui->gridLayout->getItemPosition(index, &row, &col, &rs, &cs);
        QWidget *widget = ui->gridLayout->itemAt(index)->widget();
        SectionForm* sec = dynamic_cast<SectionForm*>(widget);
        if(!sec)
            continue;
        QJsonObject current_obj;
        current_obj.insert("row",row);
        current_obj.insert("col",col);
        //sectionsorder_arr.append(current_obj);
        sectionsorder_obj.insert(sec->section(), current_obj);
    }

    gridJObj.insert("rows", ui->gridLayout->rowCount());
    gridJObj.insert("cols", ui->gridLayout->columnCount());
    gridJObj.insert("sections_order",sectionsorder_obj);

    foreach (auto item_key, sectionWidgets.keys())
    {
        sections_arr_json.append(sectionWidgets[item_key]->Serialize());
    }
    obj.insert("GridLayout",gridJObj);
    obj.insert("Sections", sections_arr_json);
    return obj;
}
