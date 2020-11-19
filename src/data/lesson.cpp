#include "lesson.h"

#include <QDataStream>
#include <QIODevice>

Lesson::Lesson(QDate date, QString name, QString link,
               QString lessonInfo, QObject *parent) : QObject(parent)
{
    this->_date = date;
    this->_name = name;
    this->_link = link;
    this->_lessonInfo = lessonInfo;
}

//Считываем урок из файла
Lesson::Lesson(QByteArray arr, QObject *parent) :
    QObject(parent)
{
    QDataStream str(&arr, QIODevice::ReadOnly);
    str >> this->_date >> this->_name >> this->_link >> this->_lessonInfo;
}

//Записываем урок в файл
QByteArray Lesson::saveIt()
{
    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);
    str << this->_date << this->_name << this->_link << this->_lessonInfo;
    return arr;
}

QDate Lesson::Date()
{
    return this->_date;
}

void Lesson::setDate(QDate date)
{
    this->_date = date;
}

QString Lesson::Name()
{
    return this->_name;
}

void Lesson::setName(QString name)
{
    this->_name = name;
}

QString Lesson::Link()
{
    return this->_link;
}

void Lesson::setLink(QString link)
{
    this->_link = link;
}

QString Lesson::lessonInfo()
{
    return this->_lessonInfo;
}

void Lesson::setLessonInfo(QString lessonInfo)
{
    this->_lessonInfo = lessonInfo;
}




//QDataStream &operator<<(QDataStream &out, const Lesson &l)
//{
//    out << l._date << l._name << l._link << l._lessonInfo;
//    return out;
//}

//QDataStream &operator>>(QDataStream &in, Lesson &l)
//{
//    in >> l._date >> l._name >> l._link >> l._lessonInfo;
//    return in;
//}
