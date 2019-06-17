#ifndef TASKCREATIONFORM_H
#define TASKCREATIONFORM_H

#include <QWidget>
#include <QToolButton>
#include "Utils/utilities.h"

namespace Ui {
class TaskCreationForm;
}

class TaskCreationForm : public QWidget
{
    Q_OBJECT

public:
    explicit TaskCreationForm(QWidget *parent = 0);
    ~TaskCreationForm();
    void AddTask(bool edit = false);
    void setSectionsName(const QStringList &sections_name);
    void fillDataForEditMode(const QString &caption,
                             const QString &section,
                             const unsigned char &type,
                             const ParamsType &data);
private slots:
    void on_sensorTB_toggled(bool checked);
    void on_cmdTB_toggled(bool checked);
    void on_compTB_toggled(bool checked);
    void on_scriptTB_toggled(bool checked);
    void on_roslaunchTB_toggled(bool checked);
    void on_rosrunTB_toggled(bool checked);
    void on_cmdTB_2_toggled(bool checked);
    void on_scriptTB_2_toggled(bool checked);
    void typeIndexChanged();
    void sectionIndexChanged();
    void on_openFileBtn_clicked();
signals:
    void createNewItemSignal(QString caption, QString section, unsigned char type,
                             ParamsType data);
    void ItemEditedSignal(QString caption, QString section,
                          ParamsType data);
private:
    Ui::TaskCreationForm *ui;
    unsigned char current_type, sec_index;
    QString section_name;
    static QString latest_path;
    QStringList sectionsName;
    QHash<QString, QToolButton*> sectionBtns;
    QHash<unsigned char, QToolButton*> typeBtns;
};

#endif // TASKCREATIONFORM_H
