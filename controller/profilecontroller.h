#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include <QObject>
#include "Utils/utilities.h"
#include "model/itemmodel.h"

//views
class MainWindow;
class ProfileForm;
class BaseStackedPage;
class LogPageForm;

class ProfileController : public QObject
{
    Q_OBJECT
public:
    ProfileController(QWidget *profile_view,
                      QWidget *stacked_setup_view,
                      QWidget *stacked_runtime_view,
                      QWidget *log_view,
                      QObject *parent=nullptr);
    ~ProfileController();

    bool AddProfile(const QString &profile);
    void RemoveProfile(const QString &profile);
    void AddSection(const QString &section,
                    const QString &profile);
    void AddSection(const QString &section,
                       const QString &profile,
                       int row, int col);

    bool AddSetupItem(const QString &caption,
                      const QString &section,
                      const QString &profile,
                      const unsigned char &type,
                      const ParamsType &params,
                      const int &delay=0);

    void changeItemRuntimeState(const QString &caption,
                                const QString &section,
                                const QString &profile,
                                const bool    &state);

    void connectViewSignals();

    void ShowTaskDialog(const Utilities::TaskDialogMode &task_mode,
                        ItemModel *input_item=nullptr);
    void TransferSetupItem(const QString &source_caption, const QString &source_section,
                           const QString &dest_section, const QString &profile);
    void RenameSetupItem(const QString &old_name, const QString &new_name,
                         const QString &section, const QString &profile);
    void EditSetupItem(const QString &intended_caption,
                                        const QString &intended_section,
                                        const QString &new_caption,
                                        const QString &new_section,
                                        const ParamsType &new_data,
                                        const QString &profile);


    bool loadJSonConfig(const QString &path);
    bool saveJSonConfig(const QString &path);

    QString getCurrentProfile() const;

private:
    ProfileForm *profileView;
    BaseStackedPage *stackedSetupView, *stackedRuntimeView;
    MainWindow *mainWindow;
    LogPageForm *logView;
    QStringList sectionNames;

    QStringList allProfiles;
    QString currentProfile;
};

#endif // PROFILECONTROLLER_H
