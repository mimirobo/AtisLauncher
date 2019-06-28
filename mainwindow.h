#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ProfileController;
class QtMaterialSnackbar;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setNewTaskEnable(bool enable);

private slots:
    void on_actionNew_Task_triggered();
    void on_actionSave_Configurations_triggered();

    void on_actionFind_triggered();

    void on_actionRun_All_triggered();

    void on_actionOpen_All_triggered();

protected:
    void closeEvent(QCloseEvent *event);
private:
    void initializeTabs();
private:
    Ui::MainWindow *ui;
    ProfileController *profileCntrl;
    QtMaterialSnackbar *mSnackbar;
};

#endif // MAINWINDOW_H
