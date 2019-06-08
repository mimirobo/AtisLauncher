#ifndef BASESTACKEDPAGE_H
#define BASESTACKEDPAGE_H

#include <QObject>
#include <QStackedWidget>
#include <QHash>

class BaseForm;

class BaseStackedPage : public QStackedWidget
{
    Q_OBJECT
    Q_PROPERTY(WidgetType mType READ getType WRITE setType)

public:
    enum class WidgetType
    {
        Setup,
        Runtime
    };
    BaseStackedPage(QWidget *parent=0,
                    WidgetType type=WidgetType::Setup);
    virtual ~BaseStackedPage();

    inline BaseForm* getProfileWidget(const QString &profile_name){
        return (widgets.contains(profile_name))?widgets[profile_name]:nullptr;
    }
    WidgetType getType() const;
    void setType(const WidgetType &type);

public slots:
    void ProfileAdded(const QString &profile_name);
    void ProfileRemoved(const QString &profile_name);
    void CurrentProfileChanged(const QString &profile);
protected:
    QHash<QString, BaseForm*> widgets;
    WidgetType mType;
};

#endif // BASESTACKEDPAGE_H
