#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QString>
#include <QList>
#include <QObject>
#include <QThread>

class ItemModel;
class ItemForm;
class QProcess;

typedef QList<ItemModel *> ItemsQueue;

class SchedulerThread : public QThread
{
  Q_OBJECT
public:
  SchedulerThread(ItemsQueue items) :
  applyDelay(false){ queued_items = items; }

  void setApplyDelay(bool value);

protected:
  void run() override;

private:
  ItemsQueue queued_items;
  bool applyDelay;
};

class ProcessManager : public QObject
{
  Q_OBJECT
private:
  static ProcessManager *instance;
  QList<QProcess *> processes;
  //SchedulerThread *schedulerThread;
Q_SIGNALS:
  void newProcessStarted(QString title, qint64 pid);

public:
  static QString generateCommand(ItemModel *item);
  static QString generateCommandForLaunchFile(ItemModel *item);
  static QString generateCommandForRosrun(ItemModel *item);
  static QString generateCommandForScript(ItemModel *item);
  static QString generateCommandForSystemCommand(ItemModel *item);

  void RunSingle(ItemModel *item);
  void RunMultiple(const ItemsQueue &items);
  void runScheduler();

  ProcessManager();
  ~ProcessManager();
  static ProcessManager *getInstance()
  {
    if (instance == 0)
      instance = new ProcessManager;
    return instance;
  }

  QMap<QString,QString> runningLaunchFiles;
  QStringList getRunningLaunchFilePath() const;
  QString getCaptionOfLaunchFile(const QString &file_path) const;
};

#endif // !PROCESSMANAGER_H
