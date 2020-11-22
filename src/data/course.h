#ifndef COURSE_H
#define COURSE_H

#include <QObject>

#include "lesson.h"

class Course : public QObject
{
    Q_OBJECT

    QString _name;// - название курса
    QString _link;// - ссылка на курс в vk
    QString _courseInfo;// - описание курса
    QList<Lesson*> lessons;// - список видеоуроков в курсе

public:
    Course(QString name, QString link, QString courseInfo, QObject *parent = 0);
    Course(QByteArray arr, QObject *parent = nullptr);
    QByteArray saveIt();//сохраняем в файл каждый курс

    QString Name();
    void setName(QString name);

    QString Link();
    void setLink(QString link);

    QString courseInfo();
    void setCourseInfo(QString courseInfo);

    Lesson* getLesson(int id);
    Lesson* getLessonByName(QString name);
    QList<Lesson*> getLessons();
    int getLessonCount();
    void deleteLessonByObj(Lesson *lesson);
    void deleteLessonById(int id);
    void up(int id);
    void down(int id);

    //Admin
    void addLesson(Lesson* lesson);
    void insertDataFirst(QDate date, QString lesson, QString link, QString info);
    void insertData(QString source, QDate date, QString lesson, QString link, QString info);
    void insertDataAtEnd(QDate date, QString lesson, QString link, QString info);
    bool lessonExist(QString name);

};


#endif // COURSE_H
