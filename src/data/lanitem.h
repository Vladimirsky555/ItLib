#ifndef LANITEM_H
#define LANITEM_H

#include <QObject>

#include "course.h"

class LanItem : public QObject
{
    Q_OBJECT

    QString _name;// - название языка программирования
    QList<Course*> courses;// - список курсов по языку программирования

public:
    LanItem(QString name, QObject *parent = 0);
    LanItem(QByteArray arr, QObject *parent = nullptr);
    QByteArray saveIt();

    QString Name();
    void setName(QString name);

    Course* getCourse(int id);
    Course* getcourseByName(QString name);
    int getCoursesCount();
    QList<Course*> getCourses();
    void deleteCourseByObj(Course *course);
    void deleteCourseById(int id);
    void up(int id);
    void down(int id);

    //Admin
    void addCourse(Course* course);
    void insertDataFirst(QString newCourse, QString newLink, QString newInfo);
    void insertData(QString source, QString newCourse, QString newLink, QString newInfo);
    void insertDataAtEnd(QString newCourse, QString newLink, QString newInfo);

};

#endif // LANITEM_H
