#ifndef NEWPROFILEDIALOG_H
#define NEWPROFILEDIALOG_H

#include <QDialog>

namespace Ui {
class NewProfileDialog;
}

class NewProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProfileDialog(QWidget *parent = 0, bool display_already_exists = false);
    ~NewProfileDialog();
public slots:
    void accept() override;
    int exec() override;
signals:
    void profileAdded(QString profile_name);
private:
    Ui::NewProfileDialog *ui;
};

#endif // NEWPROFILEDIALOG_H
