#include "profileform.h"
#include "ui_profileform.h"
//Qts
#include <QMessageBox>
//Dialogs
#include "widgets/Dialogs/newprofiledialog.h"

ProfileForm::ProfileForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileForm)
{
    ui->setupUi(this);
    ui->removeProfileBtn->setEnabled(false);
    ui->duplicateProfileBtn->setEnabled(false);
}

ProfileForm::~ProfileForm()
{
    delete ui;
}


//Combobox : user changes the current profile
void ProfileForm::on_profileComboBox_currentTextChanged(const QString &arg1)
{
    emit currentProfileChanged(arg1);
}
// Buttons Click Callbacks : user adds/removes/duplicates profiles
void ProfileForm::on_addProfileBtn_clicked()
{
    ui->addProfileBtn->setEnabled(false);
    ui->removeProfileBtn->setEnabled(false);
    ui->duplicateProfileBtn->setEnabled(false);

    NewProfileDialog *new_dialog = new NewProfileDialog(this, false);
    connect(new_dialog, &NewProfileDialog::profileAdded,
            [this, new_dialog](QString profile_name)
    {
        emit AddProfileRequest(profile_name);
        new_dialog->deleteLater();
    });

    connect(new_dialog, &NewProfileDialog::rejected,
            [this]()
    {
        ui->addProfileBtn->setEnabled(true);
        if(ui->profileComboBox->count() > 0)
        {
            ui->removeProfileBtn->setEnabled(true);
            ui->duplicateProfileBtn->setEnabled(true);
        }
    });
    new_dialog->exec();
}

void ProfileForm::on_removeProfileBtn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove Profile", "Are you sure you want to remove the current profile?",
                                  QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes);
    if (reply == QMessageBox::Cancel)
        return;

    emit RemoveCurrentProfileRequest();
}

void ProfileForm::on_duplicateProfileBtn_clicked()
{
    ui->addProfileBtn->setEnabled(false);
    ui->removeProfileBtn->setEnabled(false);
    ui->duplicateProfileBtn->setEnabled(false);

    NewProfileDialog *new_dialog = new NewProfileDialog(this, false);
    connect(new_dialog, &NewProfileDialog::profileAdded,
            [this, new_dialog](QString profile_name)
    {
        emit DuplicateProfileRequest(profile_name);
        new_dialog->deleteLater();
    });

    connect(new_dialog, &NewProfileDialog::rejected,
            [this]()
    {
        ui->addProfileBtn->setEnabled(true);
        if(ui->profileComboBox->count() > 0)
        {
            ui->removeProfileBtn->setEnabled(true);
            ui->duplicateProfileBtn->setEnabled(true);
        }
    });
    new_dialog->exec();
}

void ProfileForm::onProfileAdded(const QString &profile)
{
    ui->profileComboBox->addItem(profile);
    ui->profileComboBox->setCurrentText(profile);


    ui->addProfileBtn->setEnabled(true);
    ui->removeProfileBtn->setEnabled(true);
    ui->duplicateProfileBtn->setEnabled(true);
}

void ProfileForm::onProfileRemoved(const QString &profile)
{
    Q_UNUSED(profile);
    //wtf??? find a way to get index of item by text
    ui->profileComboBox->removeItem(ui->profileComboBox->currentIndex());

    if(ui->profileComboBox->count() == 0)
    {
        ui->removeProfileBtn->setEnabled(false);
        ui->duplicateProfileBtn->setEnabled(false);
    }
}

void ProfileForm::onAlreadyExists()
{
    ui->addProfileBtn->setEnabled(true);
    if(ui->profileComboBox->count()!=0)
    {
        ui->removeProfileBtn->setEnabled(true);
        ui->duplicateProfileBtn->setEnabled(true);
    }
}
