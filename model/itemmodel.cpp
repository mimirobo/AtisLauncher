#include "itemmodel.h"

ItemModel::ItemModel(const QString &caption,
                     const QString &section,
                     const QString &profile,
                     const Utilities::TaskTypes &type,
                     const ParamsType &params,
                     const bool &isInRuntime,
                     const int &runtime_delay,
                     QObject *parent)
    :BaseModel(parent), mCaption(caption),
      mSection(section), mProfile(profile),
      mType(type) , mParams(params),
      mRuntime(isInRuntime),mRunDelay(runtime_delay)
{
}

ItemModel::~ItemModel()
{
}

QString ItemModel::caption() const
{
    return mCaption;
}

void ItemModel::setCaption(const QString &caption)
{
    mCaption = caption;
}

QString ItemModel::section() const
{
    return mSection;
}

void ItemModel::setSection(const QString &section)
{
    mSection = section;
}

QString ItemModel::profile() const
{
    return mProfile;
}

void ItemModel::setProfile(const QString &profile)
{
    mProfile = profile;
}

Utilities::TaskTypes ItemModel::type() const
{
    return mType;
}

void ItemModel::setType(const Utilities::TaskTypes &type)
{
    mType = type;
}

bool ItemModel::isInRuntime() const
{
    return mRuntime;
}

void ItemModel::setRuntime(bool isInRuntime)
{
    mRuntime = isInRuntime;
}

ParamsType ItemModel::params() const
{
    return mParams;
}

int ItemModel::runDelay() const
{
    return mRunDelay;
}

void ItemModel::setRunDelay(int runDelay)
{
    mRunDelay = runDelay;
}

void ItemModel::setParams(const ParamsType &params)
{
    mParams = params;
}

ItemModel::operator QString() const
{
    return QString("Item:\n\tCaption:%1\n\tSection:%2\n\tProfile:%3\t\nType:%4\n")
            .arg(mCaption)
            .arg(mSection)
            .arg(mProfile)
            .arg((int)mType);
}
