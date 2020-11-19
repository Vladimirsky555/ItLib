#include "lanitem.h"

#include <QDataStream>
#include <QIODevice>

LanItem::LanItem(QString name, QObject *parent) :
    QObject(parent)
{
    this->_name = name;
}

//Считываем каталог (язык программирования) из файла
LanItem::LanItem(QByteArray arr, QObject *parent) :
    QObject(parent)
{
   QDataStream str(&arr, QIODevice::ReadOnly);
   str >> this->_name;

   while(!str.atEnd()){
       QByteArray tmp;
       str >> tmp;
       courses.append(new Course(tmp));
   }
}

//Записываем каталог (язык программирования) в файл
QByteArray LanItem::saveIt()
{
    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);

    str << this->_name;

    for(int i = 0; i < courses.count(); i++)
    {
        str << this->courses[i]->saveIt();
    }

    return arr;
}

QString LanItem::Name()
{
    return this->_name;
}

void LanItem::setName(QString name)
{
    this->_name = name;
}

Course *LanItem::getCourse(int id)
{
    if(id < courses.count()){
    return courses.at(id);
    }
    return NULL;
}

Course *LanItem::getcourseByName(QString name)
{
    for(int i = 0; i < courses.count(); i++){
        if(courses[i]->Name() == name)
            return courses[i];
    }

    return NULL;
}


void LanItem::addCourse(Course *course)
{
    if(course != NULL)
        courses.push_back(course);
}

void LanItem::insertDataFirst(QString newCourse, QString newLink, QString newInfo)
{
    Course *tmp = new Course(newCourse, newLink, newInfo);
    courses.insert(0,tmp);
}

void LanItem::insertData(QString source, QString newCourse, QString newLink, QString newInfo)
{
    Course *temp = new Course(newCourse, newLink, newInfo);

       for(int i = 0; i < courses.count(); i++){
           if(courses[i]->Name().contains(source)){
               courses.insert(i+1,temp);
               return;
           }
       }

       return;
}

void LanItem::insertDataAtEnd(QString newCourse, QString newLink, QString newInfo)
{
    Course* ci = new Course(newCourse, newLink, newInfo);
    courses.push_back(ci);
}

int LanItem::getCoursesCount()
{
    return courses.count();
}

QList<Course *> LanItem::getCourses()
{
    return this->courses;
}

void LanItem::deleteCourseByObj(Course *course)
{
    if(course != NULL)
        courses.removeOne(course);
    delete course;
}

void LanItem::deleteCourseById(int id)
{
    courses.removeAt(id);
    delete courses[id];
}

void LanItem::up(int id)
{
    Course *tmp;
    if(id > 0){
        tmp = courses[id];
        courses[id] = courses[id - 1];
        courses[id - 1] = tmp;
    }

    return;
}

void LanItem::down(int id)
{
    Course *tmp;
    if(id < courses.count() - 1){
        tmp = courses[id];
        courses[id] = courses[id + 1];
        courses[id + 1] = tmp;
    }

    return;
}
