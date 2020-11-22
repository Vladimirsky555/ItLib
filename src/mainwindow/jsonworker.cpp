#include "jsonworker.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QVariantMap>
#include <QThread>
#include <QDebug>

JsonWorker::JsonWorker(Storage *s, QJsonObject object,
                       QMutex *mutex, QObject *parent) :
    QObject(parent), QRunnable ()
{
    this->s = s;
    this->object = object;
    this->mutex = mutex;
}

void JsonWorker::run()
{
//    qInfo() << QThread::currentThread();
    QVariantMap map = object.toVariantMap();

    Data *D = new Data();
    QDate Date;

    D->setLanName(map["lan_name"].toString());
    D->setCourseName(map["course_name"].toString());
    D->setCourseLink(map["course_link"].toString());
    D->setCourseInfo(map["course_info"].toString());
    D->setLessonDate(Date.fromString(map["lesson_date"].toString(), "dd.MM.yyyy"));
    D->setLessonName(map["lesson_name"].toString());
    D->setLessonLink(map["lesson_link"].toString());
    D->setLessonInfo(map["lesson_info"].toString());

    QMutexLocker locker(mutex);
    s->addModelItem(D);
}
