#include "course.h"

#include <QDataStream>
#include <QIODevice>

Course::Course(QString name, QString link, QString courseInfo, QObject *parent) :
    QObject(parent)
{
    this->_name = name;
    this->_link = link;
    this->_courseInfo = courseInfo;
}



//Считываем курс из файла
Course::Course(QByteArray arr, QObject *parent) :
    QObject(parent)
{
    QDataStream str(&arr, QIODevice::ReadOnly);
    str >> this->_name >> this->_link >> this->_courseInfo;

    while(!str.atEnd()){
        QByteArray tmp;
        str >> tmp;
        lessons.append(new Lesson(tmp));
    }
}

//Записываем курс в файл
QByteArray Course::saveIt()
{
    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);

    str << this->_name << this->_link << this->_courseInfo;

    for(int i = 0; i < lessons.count(); i++)
    {
        str << this->lessons[i]->saveIt();
    }

    return arr;
}

QString Course::Name()
{
    return this->_name;
}

void Course::setName(QString name)
{
    this->_name = name;
}

QString Course::Link()
{
    return this->_link;
}

void Course::setLink(QString link)
{
    this->_link = link;
}

QString Course::courseInfo()
{
    return this->_courseInfo;
}

void Course::setCourseInfo(QString courseInfo)
{
    this->_courseInfo = courseInfo;
}

Lesson *Course::getLesson(int id)
{
    if(id < lessons.count()){
        return lessons.at(id);
    }
    return NULL;
}

Lesson *Course::getLessonByName(QString name)
{
    for(int i = 0; i < lessons.count(); i++){
        if(lessons[i]->Name() == name)
            return lessons[i];
    }

    return NULL;
}


void Course::addLesson(Lesson *lesson)
{
    if(lesson != NULL)
        lessons.push_back(lesson);
}

void Course::insertDataFirst(QDate date, QString lesson, QString link, QString info)
{
    Lesson *tmp = new Lesson(date, lesson, link, info);
    lessons.insert(0,tmp);
}

void Course::insertData(QString source, QDate date, QString lesson, QString link, QString info)
{
    Lesson *tmp = new Lesson(date, lesson, link, info);

    for(int i = 0; i < lessons.count(); i++)
    {
        if(lessons[i]->Name().contains(source))
        {
            lessons.insert(i+1,tmp);
            return;
        }
    }

    return;
}

void Course::insertDataAtEnd(QDate date, QString lesson, QString link, QString info)
{
    Lesson *li = new Lesson(date, lesson, link, info);
    lessons.push_back(li);
}

bool Course::lessonExist(QString name)
{
    for(int i = 0; i < lessons.count(); i++){
        if(lessons.at(i)->Name() == name)
            return true;
    }

    return false;
}


int Course::getLessonCount()
{
    return lessons.count();
}

void Course::deleteLessonByObj(Lesson *lesson)
{
    if(lesson != NULL)
        lessons.removeOne(lesson);
    delete lesson;
}

void Course::deleteLessonById(int id)
{
    lessons.removeAt(id);
    delete lessons[id];
}

void Course::up(int id)
{
    Lesson *tmp;
    if(id > 0){
        tmp = lessons[id];
        lessons[id] = lessons[id - 1];
        lessons[id - 1] = tmp;
    }

    return;
}

void Course::down(int id)
{
    Lesson *tmp;
    if(id < lessons.count() - 1){
        tmp = lessons[id];
        lessons[id] = lessons[id + 1];
        lessons[id + 1] = tmp;
    }

    return;
}

QList<Lesson *> Course::getLessons()
{
    return this->lessons;
}


