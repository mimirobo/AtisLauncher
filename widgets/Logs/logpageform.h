#ifndef LOGPAGEFORM_H
#define LOGPAGEFORM_H

#include <QWidget>
#include <QMap>

class QListWidgetItem;
namespace Ui {
class LogPageForm;
}

class LogPageForm : public QWidget
{
    Q_OBJECT

public:
    explicit LogPageForm(QWidget *parent = 0);
    ~LogPageForm();

    void addConsoleTabPage(const QString &title, const long &wid);
    void removeConsoleTabPage(const QString &title);

    void removeAllTabPages();

private slots:
    void on_expandBtn_clicked();

    void on_updateBtn_clicked();

    void on_windowsList_itemDoubleClicked(QListWidgetItem *item);
    void removeConsoleTabPage(const int &index);

    void on_logTab_tabCloseRequested(int index);

private:
    Ui::LogPageForm *ui;
    QMap<QString, long> window_ids;
    QMap<QString, QWindow*> windows;
    QMap<QString, QWidget*> tabPages;
    QStringList openedTabNames;
};

#endif // LOGPAGEFORM_H
