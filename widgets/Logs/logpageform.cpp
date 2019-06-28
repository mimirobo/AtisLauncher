#include "logpageform.h"
#include "ui_logpageform.h"
#include <QProcess>
#include <QWindow>
#include <QListWidgetItem>
#include "process/processmanager.h"

LogPageForm::LogPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogPageForm)
{
    ui->setupUi(this);
}

LogPageForm::~LogPageForm()
{
    delete ui;
}

void LogPageForm::addConsoleTabPage(const QString &title, const long &wid)
{
    QWindow *window = QWindow::fromWinId(wid);
    window->setFlags(Qt::FramelessWindowHint);
    QWidget *widget = QWidget::createWindowContainer(window,this);
    ui->logTab->addTab(widget, title);
    windows.insert(title, window);
    tabPages.insert(title, widget);
    openedTabNames.append(title);
}

void LogPageForm::removeConsoleTabPage(const QString &title)
{
    int index = ui->logTab->indexOf(tabPages[title]);
    windows[title]->setParent(nullptr);
    windows[title]->setFlags(Qt::Window);
    ui->logTab->removeTab(index);
    QWidget *widget = tabPages.take(title);
    widget->deleteLater();
    openedTabNames.removeAll(title);
}

void LogPageForm::removeAllTabPages()
{
    for(int i=0;i<ui->logTab->tabBar()->count();i++)
        removeConsoleTabPage(i);
}

void LogPageForm::openConsoleInTab(const QString &title)
{
    if(openedTabNames.contains(title))
        ui->logTab->setCurrentIndex(openedTabNames.indexOf(title));
    else
        addConsoleTabPage(title, window_ids[title]);
}

void LogPageForm::closeAllTerminalWindows()
{
    for(auto win_name : windows_fullname)
    {
        QProcess proc;
        proc.start("wmctrl",QStringList()<<"-c"<<win_name);
        proc.waitForFinished();
        //QProcess::startDetached("wmctrl", );
    }
}

void LogPageForm::OpenAllWindows()
{
    on_updateBtn_clicked();
    for(int i = 0; i < ui->windowsList->count(); ++i)
    {
        QListWidgetItem* item = ui->windowsList->item(i);
        QString name = item->text();
        openConsoleInTab(name);
    }
}

void LogPageForm::updateWindowsList()
{
    QProcess wmctrl_p;
    QProcess grep_p;
    QProcess awk_p;
    wmctrl_p.setStandardOutputProcess(&grep_p);
    grep_p.setStandardOutputProcess(&awk_p);

    wmctrl_p.start("wmctrl",QStringList()<<"-lp");
    wmctrl_p.waitForFinished();

    QStringList grep_params;
    grep_params<<"-e"<<"atislauncher";
    QStringList launchfilepath = ProcessManager::getInstance()->getRunningLaunchFilePath();
    for(QString path : launchfilepath)
    {
        grep_params<<"-e"<<path.append("*");
    }
    grep_p.start("grep",grep_params);
    grep_p.waitForFinished();

    awk_p.start("awk", QStringList()<<"{print $1,$5}");
    awk_p.waitForFinished();
    QString output(awk_p.readAllStandardOutput());
    QStringList windows_list = output.split("\n");
    windows_list.removeAll("");
    window_ids.clear();
    windows_fullname.clear();
    ui->windowsList->clear();
    for(QString window : windows_list)
    {
        QStringList parts = window.split(" ");
        long id= (parts[0]).toLong(0,16);
        QString name = parts[1];
        windows_fullname.append(name);
        if( launchfilepath.contains(name) )
            name = ProcessManager::getInstance()->getCaptionOfLaunchFile(name);
        window_ids.insert(name, id);
        ui->windowsList->addItem(name);
    }

    for(QString window : openedTabNames)
    {
        ui->windowsList->addItem(window);
    }
}

void LogPageForm::on_expandBtn_clicked()
{
    if(ui->expandBtn->text() == "<<")
    {
        ui->groupBox->setVisible(false);
        ui->expandBtn->setText(">>");
    }else
    {
        ui->groupBox->setVisible(true);
        ui->expandBtn->setText("<<");
    }
}

void LogPageForm::on_updateBtn_clicked()
{
    updateWindowsList();
}

void LogPageForm::on_windowsList_itemDoubleClicked(QListWidgetItem *item)
{
    //to do: if the window is already closed and the list is not updated
    QString name = item->text();
    openConsoleInTab(name);
}

void LogPageForm::removeConsoleTabPage(const int &index)
{
    QString title = ui->logTab->tabText(index);
    removeConsoleTabPage(title);
}

void LogPageForm::on_logTab_tabCloseRequested(int index)
{
    removeConsoleTabPage(index);
}

void LogPageForm::on_openallBtn_clicked()
{
    OpenAllWindows();
}

void LogPageForm::on_closeallTermBtn_clicked()
{
    //removeAllTabPages();
    closeAllTerminalWindows();
    on_updateBtn_clicked();
}

void LogPageForm::on_closeAllTabsBtn_clicked()
{
    removeAllTabPages();
    ui->closeallTermBtn->setEnabled(true);
}
