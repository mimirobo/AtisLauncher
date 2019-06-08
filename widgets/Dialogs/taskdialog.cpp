#include "taskdialog.h"
#include "ui_taskdialog.h"

#include "taskcreationform.h"

TaskDialog::TaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDialog)
{
    ui->setupUi(this);
}

TaskDialog::~TaskDialog()
{
    delete ui;
}

TaskCreationForm *TaskDialog::getNewTaskForm() const
{
    return ui->taskWidget;
}

void TaskDialog::setCurrentMode(const Utilities::TaskDialogMode &value)
{
    currentMode = value;
    setWindowTitle((currentMode == Utilities::TaskDialogMode::NewTask)?"New Task":"Edit Item");
}

void TaskDialog::on_buttonBox_accepted()
{
    switch (currentMode) {
    case Utilities::TaskDialogMode::NewTask:
        ui->taskWidget->AddTask(false);
        break;
    case Utilities::TaskDialogMode::EditTask:
        ui->taskWidget->AddTask(true);
        break;
    }
}
