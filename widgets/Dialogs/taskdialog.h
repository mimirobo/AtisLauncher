#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>
#include "Utils/utilities.h"

class TaskCreationForm;

namespace Ui {
class TaskDialog;
}

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskDialog(QWidget *parent = 0);
    ~TaskDialog();
    TaskCreationForm *getNewTaskForm() const;
    void setCurrentMode(const Utilities::TaskDialogMode &value);
private slots:
    void on_buttonBox_accepted();
private:
    Ui::TaskDialog *ui;
    Utilities::TaskDialogMode currentMode;
};

#endif // TASKDIALOG_H
