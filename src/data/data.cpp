#include "data.h"

Data::Data(QObject *parent) : QObject(parent)
{  
    this->lan_name = "";
    this->course_name = "";
    this->course_link = "";
    this->course_info = "";
    this->lesson_date = QDate::currentDate();
    this->lesson_name = "";
    this->lesson_link = "";
    this->lesson_info = "";
}

QString Data::lanName()
{
    return this->lan_name;
}

void Data::setLanName(QString value)
{
    this->lan_name = value;
}

QString Data::courseName()
{
    return this->course_name;
}

void Data::setCourseName(QString value)
{
    this->course_name = value;
}

QString Data::courseLink()
{
    return this->course_link;
}

void Data::setCourseLink(QString value)
{
    this->course_link = value;
}

QString Data::courseInfo()
{
    return this->course_info;
}

void Data::setCourseInfo(QString value)
{
    this->course_info = value;
}

QDate Data::lessonDate()
{
    return this->lesson_date;
}

void Data::setLessonDate(QDate value)
{
    this->lesson_date = value;
}

QString Data::lessonName()
{
    return this->lesson_name;
}

void Data::setLessonName(QString value)
{
    this->lesson_name = value;
}

QString Data::lessonLink()
{
    return this->lesson_link;
}

void Data::setLessonLink(QString value)
{
    this->lesson_link = value;
}

QString Data::lessonInfo()
{
    return this->lesson_info;
}

void Data::setLessonInfo(QString value)
{
    this->lesson_info = value;
}

QString Data::view()
{
    QString view;
    view = this->lesson_name + this->lesson_info;
    return view;
}
