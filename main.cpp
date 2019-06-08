#include "mainwindow.h"
//Qts
#include <QFile>
#include <QThread>
#include <QSettings>
#include <QTextStream>
#include <QApplication>
#include <QSplashScreen>
//Utils
#include "Utils/utilities.h"
//Prototypes
void loadGeneralConfigs(const QString &path);
//Main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loadGeneralConfigs(QApplication::applicationDirPath()+"/configs.ini");
    QPixmap pixmap("://splash/atis_splashscreen.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();
    splash.showMessage("Loading all ros packages");
    a.processEvents();
    Utilities::getInstance()->getListofAllRosPackages();
    splash.showMessage("All Ros packages loaded");
    a.processEvents();
    splash.showMessage("Loading theme");
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
    splash.showMessage("Application is ready.");
    a.processEvents();
    MainWindow w;
    w.show();
    splash.finish(&w);
    return a.exec();
}
//functions
void loadGeneralConfigs(const QString &path)
{
    //_-_-_-_-_-_-_-_-_-_-_-_- Load File Content
    QFile file;
    file.setFileName(path);
    QSettings settings(path, QSettings::IniFormat);
    settings.sync();
    if(!file.exists())
    {
        //Default Data
        settings.setValue("workspace","");
        settings.setValue("ros","/opt/ros/kinetic/setup.bash");
        settings.setValue("rospack","/opt/ros/kinetic/bin/rospack");
        settings.sync();
    }
    //bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
    //if (!result)
    //{
    //    QMessageBox msgbox;
    //    msgbox.setText("Error in loading config file");
    //    msgbox.exec();
    //    return;
    //}
    //QString all_txt = file.readAll();
    //file.close();
    //_-_-_-_-_-_-_-_-_-_-_-_- Load File Content
    Utilities::getInstance()->setRosSetupPath(settings.value("ros").toString());
    Utilities::getInstance()->setRospackPath(settings.value("rospack").toString());
    Utilities::getInstance()->setWorkSpaceSetupPath(settings.value("workspace").toString());
}
