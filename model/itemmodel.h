#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include "basemodel.h"
#include "Utils/utilities.h"

class ItemModel : public BaseModel
{
    Q_OBJECT
public:
    ItemModel(const QString &caption,
              const QString &section,
              const QString &profile,
              const Utilities::TaskTypes &type,
              const ParamsType &params,
              const bool &isInRuntime,
              const int &runtime_delay,
              QObject *parent = nullptr);
    ~ItemModel();

    operator QString() const;

    //Getters and Setters
    QString caption() const;
    void setCaption(const QString &caption);

    QString section() const;
    void setSection(const QString &section);

    QString profile() const;
    void setProfile(const QString &profile);

    Utilities::TaskTypes type() const;
    void setType(const Utilities::TaskTypes &type);

    bool isInRuntime() const;
    void setRuntime(bool isInRuntime);

    ParamsType params() const;

    int runDelay() const;
    void setRunDelay(int runDelay);

    void setParams(const ParamsType &params);

private:
    QString mCaption;
    QString mSection;
    QString mProfile;
    Utilities::TaskTypes mType;
    ParamsType mParams;
    bool mRuntime;
    int mRunDelay;
};

#endif // ITEMMODEL_H
