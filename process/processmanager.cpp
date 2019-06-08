#include "process/processmanager.h"
#include "model/itemmodel.h"
#include "widgets/Forms/itemform.h"
#include "Utils/utilities.h"
#include <QProcess>
#include <QObject>
#include <QDebug>
#include <QThread>

ProcessManager *ProcessManager::instance = 0;

ProcessManager::ProcessManager()
{
}
ProcessManager::~ProcessManager()
{
}

QStringList ProcessManager::getRunningLaunchFilePath() const
{
    return QStringList(runningLaunchFiles.values());
}

QString ProcessManager::getCaptionOfLaunchFile(const QString &file_path) const
{
    return runningLaunchFiles.key(file_path);
}

QString ProcessManager::generateCommand(ItemModel *item)
{
    switch (item->type())
    {
    case Utilities::TaskTypes::Roslaunch:
        return generateCommandForLaunchFile(item);
    case Utilities::TaskTypes::Rosrun:
        return generateCommandForRosrun(item);
    case Utilities::TaskTypes::Command:
        return generateCommandForSystemCommand(item);
    case Utilities::TaskTypes::Script:
        return generateCommandForScript(item);
    }
    return QString();
}
QString ProcessManager::generateCommandForLaunchFile(ItemModel *item)
{
    return QString("roslaunch %1 %2").arg(item->params()[Utilities::PACKAGE][0], item->params()[Utilities::LAUNCHFILE][0]);
}
QString ProcessManager::generateCommandForRosrun(ItemModel *item)
{
    return QString("rosrun %1 %2").arg(item->params()[Utilities::PACKAGE][0], item->params()[Utilities::NODENAME][0]);
}
QString ProcessManager::generateCommandForScript(ItemModel *item)
{
    return QString("%1 %2").arg(item->params()[Utilities::PYTHONVERSION][0], item->params()[Utilities::FULLPATH][0]);
}
QString ProcessManager::generateCommandForSystemCommand(ItemModel *item)
{
    return QString(item->params()[Utilities::COMMAND][0]);
}

void ProcessManager::RunSingle(ItemModel *item)
{
    if(item->type() == Utilities::TaskTypes::Roslaunch)
    {
        if(!runningLaunchFiles.contains(item->caption()))
        {
            runningLaunchFiles.insert(item->caption(),
                                      item->params()[Utilities::FULLPATH][0]);
        }
        else
        {
            runningLaunchFiles.remove(item->caption());
            runningLaunchFiles.insert(item->caption(),
                                      item->params()[Utilities::FULLPATH][0]);
        }
    }
    ItemsQueue items;
    items.append(item);
    SchedulerThread *schedulerThread = new SchedulerThread(items);
    schedulerThread->setApplyDelay(false);
    connect(schedulerThread, &SchedulerThread::started, [this] {
        qDebug() << "The scheduler thread is started";
    });
    connect(schedulerThread, &SchedulerThread::finished, [this,schedulerThread] {
        qDebug() << "The scheduler thread is finished";
        schedulerThread->deleteLater();
    });
    schedulerThread->start();
}

void ProcessManager::RunMultiple(const ItemsQueue &items)
{
    if(items.count() == 0)
        return;
    //runningLaunchFiles.clear();
    for(ItemModel *item : items)
    {
        if(item->type() != Utilities::TaskTypes::Roslaunch)
            continue;
        if(!runningLaunchFiles.contains(item->caption()))
        {
            runningLaunchFiles.insert(item->caption(),
                                      item->params()[Utilities::FULLPATH][0]);
        }else
        {
            runningLaunchFiles.remove(item->caption());
            runningLaunchFiles.insert(item->caption(),
                                      item->params()[Utilities::FULLPATH][0]);
        }
    }
    qDebug()<<runningLaunchFiles;
    SchedulerThread *schedulerThread = new SchedulerThread(items);
    schedulerThread->setApplyDelay(true);
    connect(schedulerThread, &SchedulerThread::started, [this] {
        qDebug() << "The scheduler thread is started";
    });
    connect(schedulerThread, &SchedulerThread::finished, [this,schedulerThread] {
        qDebug() << "The scheduler thread is finished";
        schedulerThread->deleteLater();
    });
    schedulerThread->start();
}

void SchedulerThread::run()
{
    QString rosSetupPath = Utilities::getInstance()->getRosSetupPath();
    QString workspaceSetupPath = Utilities::getInstance()->getWorkSpaceSetupPath();
    for (auto item : queued_items)
    {
        QString terminal_app = "gnome-terminal";
        QStringList terminal_params = QStringList() << "--working-directory"
                                                    << "=~";
        QString source_cmd = "source ~/.bashrc ; source "+ rosSetupPath +"; source "+ workspaceSetupPath+"; ";
        QString rename_tab_name = "PS1=$ PROMPT_COMMAND= echo -en \"\\033]0;"+item->caption()+"-atislauncher\\a\";";
        QString built_cmd = QString("bash -c ") + QString("\'") +source_cmd + rename_tab_name +ProcessManager::generateCommand(item) +  QString("\'");
        terminal_params << "-e" << built_cmd;
        if(applyDelay)
            if (item->runDelay() > 0)
                QThread::sleep(item->runDelay());
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();//what to do?
        QProcess::startDetached(terminal_app, terminal_params);
    }
}

void SchedulerThread::setApplyDelay(bool value)
{
    applyDelay = value;
}
