#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ProfileController;

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

protected:
    void closeEvent(QCloseEvent *event);
private:
    void initializeTabs();
private:
    Ui::MainWindow *ui;
    ProfileController *profileCntrl;
};

#endif // MAINWINDOW_H
