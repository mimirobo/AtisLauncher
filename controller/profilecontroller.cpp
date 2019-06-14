#include "profilecontroller.h"

//views
#include "widgets/Stacked/basestackedpage.h"
#include "widgets/Forms/profileform.h"
#include "widgets/Forms/setupform.h"
#include "widgets/Forms/sectionform.h"
#include "widgets/Forms/runtimeform.h"
#include "widgets/Forms/itemform.h"
#include "mainwindow.h"
//Dialogs
#include "widgets/Dialogs/taskdialog.h"
#include "widgets/Dialogs/taskcreationform.h"
//Proces
#include "process/processmanager.h"
//Qts
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
//Qts Json
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>



ProfileController::ProfileController(QWidget *profile_view,
                                     QWidget *stacked_setup_view,
                                     QWidget *stacked_runtime_view,
                                     QObject *parent)
    :QObject(parent)
{
    profileView = dynamic_cast<ProfileForm*>(profile_view);
    stackedSetupView = dynamic_cast<BaseStackedPage*>(stacked_setup_view);
    stackedRuntimeView = dynamic_cast<BaseStackedPage*>(stacked_runtime_view);
    mainWindow = dynamic_cast<MainWindow*>(parent);
    Q_ASSERT(profileView);
    Q_ASSERT(stackedSetupView);
    Q_ASSERT(stackedRuntimeView);
    Q_ASSERT(mainWindow);
    sectionNames = QStringList()<<
                                   "Sensors"<<
                                   "Commands"<<
                                   "Competitions"<<
                                   "Scripts";
    connectViewSignals();

    loadJSonConfig(QDir::homePath()+"/AtisDashboardConfig.json");
}

ProfileController::~ProfileController()
{
}

bool ProfileController::AddProfile(const QString &profile)
{
    if(allProfiles.contains(profile))
    {
        profileView->onAlreadyExists();
        return false;
    }
    allProfiles.append(profile);
    profileView->onProfileAdded(profile);
    stackedSetupView->ProfileAdded(profile);
    stackedRuntimeView->ProfileAdded(profile);

    //Runtime Signals
    RuntimeForm *runtime = dynamic_cast<RuntimeForm*>(stackedRuntimeView->getProfileWidget(profile));
    connect(runtime, &RuntimeForm::runAllRequested,
            [this](const QString &profile)
    {
        RuntimeForm *runtime = dynamic_cast<RuntimeForm*>(stackedRuntimeView->getProfileWidget(profile));
        ProcessManager::getInstance()->RunMultiple(runtime->getRunItems());
    });
    connect(ProcessManager::getInstance(), &ProcessManager::runStateChanged,
            [this,runtime](QString caption, QString profile, bool waiting)
    {
        runtime->ChangeWaitForRunItem(caption, waiting);
    });
    connect(ProcessManager::getInstance(), &ProcessManager::RunAllBegan,
            runtime, &RuntimeForm::onRunAllBegan);
    connect(ProcessManager::getInstance(), &ProcessManager::RunAllFinished,
            runtime, &RuntimeForm::onRunAllFinished);

    mainWindow->setNewTaskEnable(true);
    return true;
}

void ProfileController::RemoveProfile(const QString &profile)
{
    stackedSetupView->ProfileRemoved(profile);
    stackedRuntimeView->ProfileRemoved(profile);
    allProfiles.removeAll(profile);
    profileView->onProfileRemoved(profile);
    if(allProfiles.count()==0)
        mainWindow->setNewTaskEnable(false);
}

void ProfileController::AddSection(const QString &section,
                                   const QString &profile)
{
    SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->
                                                getProfileWidget(profile));
    SectionForm  *section_ui = setup->AddSection(section);
    connect(section_ui, &SectionForm::requestDragDropTransfer,
            [this](QString source_caption, QString source_section,
            QString dest_section, QString profile)
    {
        TransferSetupItem(source_caption,
                          source_section,
                          dest_section,
                          profile);
    });
}

void ProfileController::AddSection(const QString &section,
                                      const QString &profile,
                                      int row, int col)
{
    SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->
                                                getProfileWidget(profile));
    SectionForm  *section_ui = setup->InsertSection(section,row, col);
    connect(section_ui, &SectionForm::requestDragDropTransfer,
            [this](QString source_caption, QString source_section,
            QString dest_section, QString profile)
    {
        TransferSetupItem(source_caption,
                          source_section,
                          dest_section,
                          profile);
    });
}

bool ProfileController::AddSetupItem(const QString &caption,
                                     const QString &section,
                                     const QString &profile,
                                     const unsigned char &type,
                                     const ParamsType &params,
                                     const int &delay)
{
    SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->
                                                getProfileWidget(profile));
    ItemForm *item = setup->getSection(section)->
            AddItem(caption, (Utilities::TaskTypes&)type, params , delay);
    if (item == nullptr)
    {
        QMessageBox msgbox;
        msgbox.setText(QString("The Task with name of %1 already exists.\nYou can't create two tasks with the same name.").arg(caption));
        msgbox.exec();
        return false;
    }

    connect (item, &ItemForm::ItemRenameRequested,
             [this](QString old_name, QString new_name,
             QString section, QString profile)
    {
        RenameSetupItem(old_name,
                        new_name,
                        section,
                        profile);
    });
    connect (item, &ItemForm::ItemRemoveRequested,
             [this](QString caption, QString section, QString profile)
    {
        RuntimeForm * runtime = dynamic_cast<RuntimeForm*>(stackedRuntimeView->getProfileWidget(profile));
        SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->getProfileWidget(profile));
        runtime->RemoveItem(caption);
        setup->getSection(section)->RemoveItem(caption);
    });

    connect(item, &ItemForm::ItemEditRequested,
            [this](QString caption, QString section, QString profile)
    {
        SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->getProfileWidget(profile));
        ItemForm *item_ui = setup->getSection(section)->Item(caption);
        ShowTaskDialog(Utilities::TaskDialogMode::EditTask, item_ui->getModel());
    });

    connect( item, &ItemForm::ItemRuntimeStateChanged,
             [this](QString caption, QString section, QString profile, bool state)
    {
        changeItemRuntimeState(caption,
                               section,
                               profile,
                               state);
    });
    return true;
}

void ProfileController::changeItemRuntimeState(const QString &caption,
                                               const QString &section,
                                               const QString &profile,
                                               const bool &state)
{
    SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->getProfileWidget(profile));
    RuntimeForm *runtime = dynamic_cast<RuntimeForm*>(stackedRuntimeView->getProfileWidget(profile));
    ItemForm *item =setup->getSection(section)->Item(caption);
    if(state)
        runtime->AddItem(item->getModel());
    else
        runtime->RemoveItem(item->caption());
    item->ChangeRuntimeSilent(state);
}

void ProfileController::connectViewSignals()
{
    connect( profileView, &ProfileForm::AddProfileRequest,
             [this](const QString &profile_name)
    {
        bool result = AddProfile(profile_name);
        if(!result)
            return;
        for(auto current_section: sectionNames)
        {
            //Add Section
            AddSection(current_section, profile_name);
        }
    });
    connect( profileView, &ProfileForm::RemoveCurrentProfileRequest,
             [this]()
    {
        RemoveProfile(currentProfile);
    });
    connect( profileView, &ProfileForm::currentProfileChanged,
             [this](const QString  &current_profile_name)
    {
        currentProfile = current_profile_name;
        stackedSetupView->CurrentProfileChanged(currentProfile);
        stackedRuntimeView->CurrentProfileChanged(currentProfile);
    });
}

void ProfileController::ShowTaskDialog(const Utilities::TaskDialogMode &task_mode,
                                       ItemModel *input_item)
{
    TaskDialog *newDialog = new TaskDialog();
    TaskCreationForm *tcw = newDialog->getNewTaskForm();
    tcw->setSectionsName(sectionNames);
    newDialog->setCurrentMode(task_mode);
    switch (task_mode)
    {
    case Utilities::TaskDialogMode::NewTask:
    {
        connect(tcw, &TaskCreationForm::createNewItemSignal,
                [=](QString caption, QString section, unsigned char type, ParamsType params) {
            AddSetupItem(caption, section, currentProfile, type, params);
            newDialog->deleteLater();
        });
    }
        break;
    case Utilities::TaskDialogMode::EditTask:
    {
        tcw->fillDataForEditMode(input_item->caption(),
                                 input_item->section(),
                                 (unsigned char)input_item->type(),
                                 input_item->params());
        connect(tcw, &TaskCreationForm::ItemEditedSignal,
                [=](QString new_caption, QString new_section,
                ParamsType new_data){
            EditSetupItem(input_item->caption(),
                          input_item->section(),
                          new_caption,
                          new_section,
                          new_data,
                          currentProfile);
            newDialog->deleteLater();
        });
    }
        break;
    }
    newDialog->exec();
}

void ProfileController::TransferSetupItem(const QString &source_caption,
                                          const QString &source_section,
                                          const QString &dest_section,
                                          const QString &profile)
{
    SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->getProfileWidget(profile));
    SectionForm *source_section_ui = setup->getSection(source_section);
    SectionForm *dest_section_ui = setup->getSection(dest_section);
    if(dest_section_ui->ContainsItem(source_caption))
        return;
    ItemForm *source_item = source_section_ui->TakeItem(source_caption);
    QMetaObject::Connection * const connection = new QMetaObject::Connection;
    *connection = connect(source_item, &ItemForm::FinalAnimationFinished,
                          [this,source_item,
                          dest_section_ui,
                          connection]()
    {
        dest_section_ui->AddItem(source_item);
        QObject::disconnect(*connection);
        delete connection;
    });
}

void ProfileController::RenameSetupItem(const QString &old_name,
                                        const QString &new_name,
                                        const QString &section,
                                        const QString &profile)
{
    SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->getProfileWidget(profile));
    RuntimeForm *runtime = dynamic_cast<RuntimeForm*>(stackedRuntimeView->getProfileWidget(profile));
    bool result = setup->getSection(section)->RenameItem(old_name, new_name);
    if(result)
        runtime->RenameItem(old_name, new_name);
}

void ProfileController::EditSetupItem(const QString &intended_caption,
                                      const QString &intended_section,
                                      const QString &new_caption,
                                      const QString &new_section,
                                      const ParamsType &new_data,
                                      const QString &profile)
{
    SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->getProfileWidget(profile));
    ItemForm *item_ui = setup->getSection(intended_section)->Item(intended_caption);
    ItemModel *intended_item = item_ui->getModel();
    QString final_section = intended_section;
    QString final_caption = intended_caption;
    //compare old name with new name : if differs rename it
    if (QString::compare(intended_caption, new_caption) != 0)
    {
        //if (RenameSetupItem(item_caption, new_caption, profile))
        RenameSetupItem(intended_caption, new_caption, final_section, profile);
        final_caption = new_caption;
    }
    //check if section differs
    if (QString::compare(intended_section, new_section) != 0)
    {
        TransferSetupItem(final_caption,
                          intended_section,
                          new_section,
                          profile);//to do make it return boolean to check if transfer is possible or not
        final_section = new_section;
    }
    //update params
    intended_item->setParams(new_data);
    //restore ui to enable edit
    //to do. (disable delete button in ui the enable here)
}

bool ProfileController::loadJSonConfig(const QString &path)
{
    //_-_-_-_-_-_-_-_-_-_-_-_- Load File Content
    QFile file;
    file.setFileName(path);
    bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!result)
        return false;
    QString all_txt = file.readAll();
    file.close();
    //_-_-_-_-_-_-_-_-_-_-_-_- Load File Content
    QJsonDocument jsonDoc = QJsonDocument::fromJson(all_txt.toUtf8());
    QJsonArray profile_Jarr = jsonDoc.object().value("Profiles").toArray();
    //Iterate over Profiles
    for (int i = 0; i < profile_Jarr.count(); i++)
    {
        QString profile_name = profile_Jarr.at(i)["ProfileName"].toString();
        //qDebug()<<"Profile:"<<profile_name;
        AddProfile(profile_name);
        QJsonObject setup_obj = profile_Jarr.at(i)["Setup"].toObject();
        QJsonArray sections_obj = setup_obj["Sections"].toArray();

        QJsonObject grid_obj = setup_obj["GridLayout"].toObject();
        int maxRows = grid_obj["rows"].toInt();
        int maxCols = grid_obj["cols"].toInt();
        SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->
                                                    getProfileWidget(profile_name));
        setup->preAllocateGrid(maxRows, maxCols);

        QJsonObject sectionsOrder_jobj = grid_obj["sections_order"].toObject();
        QJsonArray::iterator sec_it = sections_obj.begin();
        //Iterate over sections
        for(;sec_it!=sections_obj.end();sec_it++)
        {
            QJsonObject current_sec_obj = (*sec_it).toObject();
            QString section_name = current_sec_obj.value("SectionName").toString();
            int row = sectionsOrder_jobj[section_name].toObject().value("row").toInt();
            int col = sectionsOrder_jobj[section_name].toObject().value("col").toInt();
            AddSection(section_name, profile_name,row,col);
            QJsonArray items_Jarr = current_sec_obj.value("Items").toArray();
            QJsonArray::iterator items_it = items_Jarr.begin();
            //qDebug()<<"\nSection: "<<section_name;
            //Iterate over Sections
            for(;items_it!=items_Jarr.end();items_it++)
            {
                QJsonObject current_item_obj = (*items_it).toObject();
                QString caption = current_item_obj.value("Caption").toString();
                unsigned char task_type = current_item_obj.value("Type").toInt();

                QJsonObject params_jobj = current_item_obj["Data"].toObject();
                QMap<QString, QStringList> params_map;
                //Iterate over params
                foreach (const QString &key, params_jobj.keys())
                {
                    QJsonArray params_arr = params_jobj[key].toArray();
                    QStringList params_value;
                    foreach (const QJsonValue &value, params_arr)
                    {
                        params_value << value.toString();
                    }
                    params_map.insert(key, params_value);
                }
                //qDebug()<<"\t\tCaption: "<<caption;
                //qDebug()<<"\t\tData: "<<params_map;

                //Delay
                int delay = current_item_obj["RunOptions"].toObject()["Delay"].toInt();
                //bool isInRuntime = current_item_obj["RunOptions"].toObject()["InRuntime"].toBool();
                AddSetupItem(caption,
                             section_name,
                             profile_name,
                             task_type,
                             params_map,
                             delay);
                /*if(isInRuntime)
                    changeItemRuntimeState(caption,
                                           section_name,
                                           profile_name,
                                           true);*/

            }
        }
        QJsonArray run_items_Jarr = profile_Jarr.at(i)["Runtime"].toObject()["RuntimeItems"].toArray();
        QJsonArray run_sections_Jarr = profile_Jarr.at(i)["Runtime"].toObject()["RuntimeSections"].toArray();
        QJsonArray::iterator runitems_it = run_items_Jarr.begin();
        //Iterate over runtime items
        for(;runitems_it!= run_items_Jarr.end();runitems_it++)
        {
            QString caption = (*runitems_it).toString();
            changeItemRuntimeState(caption,
                                   run_sections_Jarr.at(runitems_it.i).toString() ,
                                   profile_name,
                                   true);
        }
    }
    return true;
}

bool ProfileController::saveJSonConfig(const QString &path)
{
    QJsonObject parent_obj;
    QJsonArray profile_arr;
    foreach (auto profile, allProfiles)
    {
        QJsonObject currentProfile;
        currentProfile.insert("ProfileName", profile);
        SetupForm *setup = dynamic_cast<SetupForm*>(stackedSetupView->getProfileWidget(profile));
        RuntimeForm *runtime = dynamic_cast<RuntimeForm*>(stackedRuntimeView->getProfileWidget(profile));
        currentProfile.insert("Setup",setup->Serialize());
        currentProfile.insert("Runtime",runtime->Serialize());
        profile_arr.append(currentProfile);
    }
    parent_obj.insert("Profiles", profile_arr);

    QJsonDocument json_doc(parent_obj);
    QString json_string = json_doc.toJson();
    QFile save_file(path);
    if (!save_file.open(QIODevice::WriteOnly))
    {
        qDebug() << "failed to open save file";
        return false;
    }
    save_file.write(json_string.toLocal8Bit());
    save_file.close();
    return true;
}

QString ProfileController::getCurrentProfile() const
{
    return currentProfile;
}
