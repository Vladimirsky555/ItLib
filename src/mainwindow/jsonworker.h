#ifndef JSONWORKER_H
#define JSONWORKER_H

#include <QObject>
#include <QRunnable>
#include <QJsonObject>
#include <QMutex>

#include "data/data.h"
#include "data/storage.h"

//Служит добавлению данных табличной модели многопоточно
class JsonWorker : public QObject, public QRunnable
{
    Q_OBJECT

    Storage *s;
    QJsonObject object;
    QMutex* mutex;

public:
    JsonWorker(Storage *s, QJsonObject object, QMutex *mutex,
               QObject *parent = nullptr);

    // QRunnable interface
public:
    virtual void run();
};

#endif // JSONWORKER_H
