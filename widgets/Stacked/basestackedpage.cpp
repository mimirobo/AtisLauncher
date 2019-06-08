#include "basestackedpage.h"

#include "../Forms/runtimeform.h"
#include "../Forms/setupform.h"

BaseStackedPage::BaseStackedPage(QWidget *parent,
                                 WidgetType type) :
    QStackedWidget(parent),
    mType(type)

{
}

BaseStackedPage::~BaseStackedPage()
{
    qDeleteAll(widgets);
}

void BaseStackedPage::ProfileAdded(const QString &profile_name)
{
    BaseForm *page;
    switch (mType) {
    case WidgetType::Setup:
        page = new SetupForm(profile_name, this);
        break;
    case WidgetType::Runtime:
        page = new RuntimeForm(profile_name, this);
        break;
    }
    widgets.insert(profile_name, page);
    this->addWidget(page);
    CurrentProfileChanged(profile_name);
}

void BaseStackedPage::ProfileRemoved(const QString &profile_name)
{
    if(!widgets.contains(profile_name))
        return;
    BaseForm *page = widgets.take(profile_name);
    this->removeWidget(page);
    page->deleteLater();
}

void BaseStackedPage::CurrentProfileChanged(const QString &profile)
{
    if(!widgets.contains(profile))
        return;
    this->setCurrentWidget(widgets[profile]);
}

BaseStackedPage::WidgetType BaseStackedPage::getType() const
{
    return mType;
}

void BaseStackedPage::setType(const WidgetType &type)
{
    mType = type;
}
