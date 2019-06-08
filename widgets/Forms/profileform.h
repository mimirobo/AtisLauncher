#ifndef PROFILEFORM_H
#define PROFILEFORM_H

#include <QWidget>

namespace Ui {
class ProfileForm;
}

class ProfileForm : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileForm(QWidget *parent = 0);
    ~ProfileForm();

private slots:
    void on_profileComboBox_currentTextChanged(const QString &arg1);
    void on_addProfileBtn_clicked();
    void on_removeProfileBtn_clicked();
    void on_duplicateProfileBtn_clicked();
public slots:
    void onProfileAdded(const QString &profile);
    void onProfileRemoved(const QString &profile);
    void onAlreadyExists();
signals:
    void AddProfileRequest(const QString &profile);
    void RemoveCurrentProfileRequest();
    void currentProfileChanged(const QString &current_profile_name);
private:
    Ui::ProfileForm *ui;
};

#endif // PROFILEFORM_H
