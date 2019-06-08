#ifndef UTILITIES_H
#define UTILITIES_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QObject>
typedef QMap<QString, QStringList> ParamsType;

class Utilities : public QObject
{
    Q_OBJECT
  public:
    enum class TaskTypes : unsigned char
    {
        Roslaunch,
        Rosrun,
        Command,
        Script
    };
    enum class TaskDialogMode : unsigned char
    {
        NewTask,
        EditTask
    };

    static const std::string debugLevel;
    static const std::string infoLevel;
    static const std::string warnLevel;
    static const std::string errorLevel;
    static const std::string fatalLevel;

    static const QString PACKAGE;
    static const QString LAUNCHFILE;
    static const QString FULLPATH;
    static const QString NODENAME;
    static const QString COMMAND;
    static const QString PYTHONVERSION;

    Utilities()
    {
    }

    static Utilities *getInstance()
    {
        if (instance == 0)
            instance = new Utilities();
        return instance;
    }

    void getListofAllRosPackages();
    QStringList getRosPackages() const;
    QString getPathOfRosPackage(const QString &package_name);

    QString getRosSetupPath() const;
    void setRosSetupPath(const QString &value);

    QString getWorkSpaceSetupPath() const;
    void setWorkSpaceSetupPath(const QString &value);

    QString getRospackPath() const;
    void setRospackPath(const QString &value);

private:
    static Utilities *instance;
    QMap<QString, QString> mRosPackages;//all ros packages available on system

    QString rosSetupPath, wsSetupPath, rospackPath;
};

class NotImplementedException : public std::logic_error
{
  public:
    NotImplementedException() : std::logic_error("The method is not implemented yet!") {}
};

#endif // UTILITIES_H
