#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QObject>

class BaseModel : public QObject
{
    Q_OBJECT
public:
    explicit BaseModel(QObject *parent = nullptr);
    virtual ~BaseModel();

signals:

public slots:
};

#endif // BASEMODEL_H
