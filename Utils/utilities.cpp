#include "Utils/utilities.h"
#include <QProcess>
#include <QProcessEnvironment>

Utilities *Utilities::instance = 0;

const QString Utilities::PACKAGE = "package";
const QString Utilities::LAUNCHFILE = "launchfile";
const QString Utilities::FULLPATH = "fullpath";
const QString Utilities::NODENAME = "nodename";
const QString Utilities::COMMAND = "command";
const QString Utilities::PYTHONVERSION = "pythonversion";

const std::string Utilities::debugLevel = "debug";
const std::string Utilities::infoLevel = "info";
const std::string Utilities::warnLevel = "warn";
const std::string Utilities::errorLevel = "error";
const std::string Utilities::fatalLevel = "fatal";

void Utilities::getListofAllRosPackages()
{
    QProcess *proc = new QProcess;
    QStringList params = QStringList() << "-c" << "source ~/.bashrc;source "+
                                           rosSetupPath+
                                           ";source "+
                                           wsSetupPath+
                                           ";"+
                                           rospackPath+
                                           " list;";
    proc->start("bash",params);
    proc->waitForFinished();
    QString output(proc->readAllStandardOutput());
    QStringList lines = output.split("\n");
    for(QString line : lines)
    {
        if(line.isEmpty())
            continue;
        QStringList parts = line.split(" ");
        mRosPackages.insert(parts[0],parts[1]);
    }
}

QStringList Utilities::getRosPackages() const
{
    return QStringList(mRosPackages.keys());
}

QString Utilities::getPathOfRosPackage(const QString &package_name)
{
    return mRosPackages.value(package_name);
}

QString Utilities::getRosSetupPath() const
{
    return rosSetupPath;
}

void Utilities::setRosSetupPath(const QString &value)
{
    rosSetupPath = value;
}

QString Utilities::getWorkSpaceSetupPath() const
{
    return wsSetupPath;
}

void Utilities::setWorkSpaceSetupPath(const QString &value)
{
    wsSetupPath = value;
}

QString Utilities::getRospackPath() const
{
    return rospackPath;
}

void Utilities::setRospackPath(const QString &value)
{
    rospackPath = value;
}

void Utilities::setWindowSize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

int Utilities::getWindowWidth() const
{
    return windowWidth;
}

int Utilities::getWindowHeight() const
{
    return windowHeight;
}
