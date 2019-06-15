#include "mainwindow.h"
#include "ui_mainwindow.h"
//Qt
#include <QMessageBox>
#include <QTabBar>
#include <QGraphicsDropShadowEffect>
#include <QDir>
#include <QSettings>
#include <QKeyEvent>
//material libs
#include "qtmaterialtabs.h"
//utils
#include "Utils/utilities.h"
//Controller
#include "controller/profilecontroller.h"
//Runtime view
#include "widgets/Forms/runtimeform.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionNew_Task->setEnabled(false);
    ui->runtimeStackedWidget->setType(BaseStackedPage::WidgetType::Runtime);
    this->setWindowIcon(QIcon("://icons/logo_cropped.png"));
    profileCntrl = new ProfileController(ui->profileWidget,
                                         ui->setupStackedWidget,
                                         ui->runtimeStackedWidget,
                                         this);
    initializeTabs();
    this->resize(Utilities::getInstance()->getWindowWidth(),
                 Utilities::getInstance()->getWindowHeight());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setNewTaskEnable(bool enable)
{
    ui->actionNew_Task->setEnabled(enable);
}

void MainWindow::on_actionNew_Task_triggered()
{
    profileCntrl->ShowTaskDialog(Utilities::TaskDialogMode::NewTask);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit Atis Launcher", "Are you sure you want to exit?",
                                  QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes);
    if (reply == QMessageBox::Cancel)
    {
        event->ignore();
        return;
    }


    ui->logWidget->removeAllTabPages();
    QMainWindow::closeEvent(event);
}

void MainWindow::initializeTabs()
{
    QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect();
    effect->setOffset(.0);
    effect->setBlurRadius(25.0);
    effect->setColor(QColor("#EF6C00"));
    ui->tabHeaderWidget->setGraphicsEffect(effect);

    ui->tabWidget->tabBar()->hide();
    ui->tabHeaderWidget->addTab("Module Prep");
    ui->tabHeaderWidget->addTab("Runtime");
    ui->tabHeaderWidget->addTab("Logs");
    ui->tabHeaderWidget->setUseThemeColors(true);
    connect(ui->tabHeaderWidget, &QtMaterialTabs::currentChanged,
            this, [this](int index) {
        ui->tabWidget->setCurrentIndex(index);
    });

    ui->tabHeaderWidget->setBackgroundColor(QColor("#EF6C00"));
    ui->tabHeaderWidget->setInkColor(QColor("#C0CA33"));
}

void MainWindow::on_actionSave_Configurations_triggered()
{
    profileCntrl->saveJSonConfig(QDir::homePath()+"/AtisDashboardConfig.json");
}

void MainWindow::on_actionFind_triggered()
{
    RuntimeForm *runtime = dynamic_cast<RuntimeForm*>(ui->runtimeStackedWidget->getProfileWidget(profileCntrl->getCurrentProfile()));
    runtime->toggleSearchTool();
}
