#include "taskcreationform.h"
#include "ui_taskcreationform.h"


#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QFileInfo>
#include <QCompleter>

#include "Utils/utilities.h"



QString TaskCreationForm::latest_path = "~";

TaskCreationForm::TaskCreationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskCreationForm),
    current_type(0),
    sec_index(0)
{
    ui->setupUi(this);
    ui->filePathWidget->setVisible(true);
    ui->packageWidget->setVisible(false);
    ui->cmdWidget->setVisible(false);
    ui->python_lbl->setVisible(false);
    ui->python_combobox->setVisible(false);
    typeBtns.insert((unsigned char)Utilities::TaskTypes::Roslaunch, ui->roslaunchTB);
    typeBtns.insert((unsigned char)Utilities::TaskTypes::Rosrun, ui->rosrunTB);
    typeBtns.insert((unsigned char)Utilities::TaskTypes::Command, ui->cmdTB_2);
    typeBtns.insert((unsigned char)Utilities::TaskTypes::Script, ui->scriptTB_2);

    qDebug()<<Utilities::getInstance()->getRosPackages();

    QCompleter *pkgCompleter = new QCompleter(Utilities::getInstance()->getRosPackages(),
                                              this);
    pkgCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    pkgCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->pkgBox->setCompleter(pkgCompleter);
    ui->pkgForlaunchBox->setCompleter(pkgCompleter);
    //ui->testBox->setDataSource(QStringList()<<"aa"<<"bb"<<"ab"<<"ba");

}

TaskCreationForm::~TaskCreationForm()
{
    delete ui;
}

void TaskCreationForm::AddTask(bool edit)
{
    ParamsType data;
    switch (current_type)
    {
    case 0: //Roslaunch
    {

        QString full_path = ui->pathLineEdit->text();
        if (full_path.indexOf("/launch/") < 0)
        {
            QMessageBox msgbox;
            msgbox.setText("This is not a valid launch file");
            msgbox.exec();
            return;
        }
        QStringList path_splitted = full_path.split("/");
        QString launch_file_name = path_splitted[path_splitted.length() - 1];
        QString package_name = ui->pkgForlaunchBox->text();
        data.insert(Utilities::PACKAGE, QStringList() << package_name);
        data.insert(Utilities::LAUNCHFILE, QStringList() << launch_file_name);
        data.insert(Utilities::FULLPATH, QStringList() << full_path);
    }
        break;
    case 1: //Rosrun
        data.insert(Utilities::PACKAGE, QStringList() << ui->pkgBox->text());
        data.insert(Utilities::NODENAME, QStringList() << ui->nodeBox->text());
        break;
    case 2: //system Command
        data.insert(Utilities::COMMAND, QStringList() << ui->cmdLineEdit->text());
        break;
    case 3: //Script
        data.insert(Utilities::PYTHONVERSION, QStringList() << ui->python_combobox->currentText());
        data.insert(Utilities::FULLPATH, QStringList() << ui->pathLineEdit->text());
        break;
    }
    if (edit)
        Q_EMIT ItemEditedSignal(ui->captionLineEdit->text(),
                                sectionsName[sec_index],
                                data);
    else
        Q_EMIT createNewItemSignal(ui->captionLineEdit->text(),
                                   sectionsName[sec_index],
                                   current_type,
                                   data);
}

void TaskCreationForm::setSectionsName(const QStringList &sections_name)
{
    sectionsName = sections_name;
    sectionBtns.insert(sectionsName[0],ui->sensorTB);
    sectionBtns.insert(sectionsName[1],ui->cmdTB);
    sectionBtns.insert(sectionsName[2],ui->compTB);
    sectionBtns.insert(sectionsName[3],ui->scriptTB);
}

void TaskCreationForm::fillDataForEditMode(const QString &caption,
                                           const QString &section,
                                           const unsigned char &type,
                                           const ParamsType &data)
{
    ui->captionLineEdit->setText(caption);
    sectionBtns[section]->setChecked(true);
    typeBtns[type]->setChecked(true);
    ui->typeGroupBox->setEnabled(false);

    switch ((Utilities::TaskTypes)type) {
    case Utilities::TaskTypes::Roslaunch:
        ui->pkgForlaunchBox->setText(data[Utilities::PACKAGE][0]);
        ui->pathLineEdit->setText(data[Utilities::FULLPATH][0]);
        break;
    case Utilities::TaskTypes::Rosrun:
        ui->pkgBox->setText(data[Utilities::PACKAGE][0]);
        ui->nodeBox->setText(data[Utilities::NODENAME][0]);
        break;
    case Utilities::TaskTypes::Command:
        ui->cmdLineEdit->setText(data[Utilities::COMMAND][0]);
        break;
    case Utilities::TaskTypes::Script:
        ui->pathLineEdit->setText(data[Utilities::FULLPATH][0]);
        break;
    }
}

void TaskCreationForm::on_sensorTB_toggled(bool checked)
{
    if (checked)
        sec_index = 0;
}

void TaskCreationForm::on_cmdTB_toggled(bool checked)
{
    if (checked)
        sec_index = 1;
}

void TaskCreationForm::on_compTB_toggled(bool checked)
{
    if (checked)
        sec_index = 2;
}

void TaskCreationForm::on_scriptTB_toggled(bool checked)
{
    if (checked)
        sec_index = 3;
}

void TaskCreationForm::on_roslaunchTB_toggled(bool checked)
{
    if (checked)
    {
        current_type = 0;
        typeIndexChanged();
    }
}

void TaskCreationForm::on_rosrunTB_toggled(bool checked)
{
    if (checked)
    {
        current_type = 1;
        typeIndexChanged();
    }
}

void TaskCreationForm::on_cmdTB_2_toggled(bool checked)
{
    if (checked)
    {
        current_type = 2;
        typeIndexChanged();
    }
}

void TaskCreationForm::on_scriptTB_2_toggled(bool checked)
{
    if (checked)
    {
        current_type = 3;
        typeIndexChanged();
    }
    ui->python_lbl->setVisible(checked);
    ui->python_combobox->setVisible(checked);
}

void TaskCreationForm::typeIndexChanged()
{
    switch (current_type)
    {
    case 0:
        ui->filePathWidget->setVisible(true);
        ui->packageWidget->setVisible(false);
        ui->cmdWidget->setVisible(false);
        break;
    case 1:
        ui->filePathWidget->setVisible(false);
        ui->packageWidget->setVisible(true);
        ui->cmdWidget->setVisible(false);
        break;
    case 2:
        ui->filePathWidget->setVisible(false);
        ui->packageWidget->setVisible(false);
        ui->cmdWidget->setVisible(true);
        break;
    case 3:
        ui->filePathWidget->setVisible(true);
        ui->packageWidget->setVisible(false);
        ui->cmdWidget->setVisible(false);
        break;
    }
}

void TaskCreationForm::on_openFileBtn_clicked()
{
    QString open_path = TaskCreationForm::latest_path;
    if(current_type == (unsigned char)Utilities::TaskTypes::Roslaunch)
        open_path = Utilities::getInstance()->getPathOfRosPackage(ui->pkgForlaunchBox->text());
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Any File"), open_path , tr("Any File (*.*)"));

    QFileInfo info(fileName);
    TaskCreationForm::latest_path = info.dir().path();
    ui->pathLineEdit->setText(fileName);
}
