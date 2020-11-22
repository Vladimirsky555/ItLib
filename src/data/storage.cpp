#include "storage.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>

Storage::Storage(QObject *parent) :
    QObject(parent){}

void Storage::loadData()
{
    QFile f("data.db");
    if(!f.exists()) return;

    f.open(QFile::ReadOnly);
    QDataStream reader(&f);

     reader >> this->path;

    while(!reader.atEnd()){
        QByteArray arr;
        reader >> arr;
        addLanItem(new LanItem(arr));
    }

    f.close();
}

void Storage::saveData()
{
    QFile f("data.db");
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream str(&f);

    str << this->path;

    for(int i = 0; i < lans.count(); i++)
    {
        str << lans.at(i)->saveIt();
    }

    f.close();
}

QString Storage::getPath()
{
    qDebug() << path;
    return this->path;
}

void Storage::setPath(QString path)
{
    this->path = path;
}

void Storage::fillLessonModel()
{
    for(int i = 0; i < lans.count(); i++)
    {
        currentLan = lans.at(i);
        for(int j = 0; j < currentLan->getCoursesCount(); j++)
        {
            currentCourse = currentLan->getCourse(j);
            for(int k = 0; k < currentCourse->getLessonCount(); k++)
            {
                currentLesson = currentCourse->getLesson(k);
                Data *item = new Data();

                item->setLanName(currentLan->Name());

                item->setCourseName(currentCourse->Name());
                item->setCourseLink(currentCourse->Link());
                item->setCourseInfo(currentCourse->courseInfo());

                item->setLessonDate(currentLesson->Date());
                item->setLessonName(currentLesson->Name());
                item->setLessonLink(currentLesson->Link());
                item->setLessonInfo(currentLesson->lessonInfo());
                items.append(item);

            }
        }
    }
}

QList<Data *> Storage::getItems()
{
    return this->items;
}


LanItem *Storage::getLanById(int id)
{
    if(id < lans.count()){
        return lans.at(id);
    }
    return NULL;
}

LanItem *Storage::getLanByName(QString name)
{
    for(int i = 0; i < lans.count(); i++){
        if(lans[i]->Name() == name)
            return lans[i];
    }

    return NULL;
}

QList<LanItem *> Storage::LanItems()
{
   return this->lans;
}

void Storage::addLanItem(LanItem *item)
{
    if(item != NULL)
        lans.push_back(item);
}

void Storage::restoreDataFromModel()
{
    for(int i =0; i < items.count(); i++)
    {
        if(!lanExist(items.at(i)->lanName())){
            LanItem *li = new LanItem(items.at(i)->lanName());
            addLanItem(li);
        } else {
            LanItem *li = getLanByName(items.at(i)->lanName());

            if(!li->courseExist(items.at(i)->courseName())){
                Course *ci = new Course(items.at(i)->courseName(),
                                        items.at(i)->courseLink(),
                                        items.at(i)->courseInfo());
                li->addCourse(ci);
            } else {
                Course *ci = li->getcourseByName(items.at(i)->courseName());

                if(!ci->lessonExist(items.at(i)->lessonName())){
                    Lesson *l = new Lesson(items.at(i)->lessonDate(),
                                           items.at(i)->lessonName(),
                                           items.at(i)->lessonLink(),
                                           items.at(i)->lessonInfo());
                    ci->addLesson(l);
                }
//                else {
//                    Lesson *l = ci->getLessonByName(items.at(i)->lessonName());
//                }
            }
        }
    }
}

void Storage::insertDataFirst(QString newItem)
{
    LanItem *tmp = new LanItem(newItem);
    lans.insert(0,tmp);
}

void Storage::insertData(QString source, QString newItem)
{
    LanItem *tmp = new LanItem(newItem);

    for(int i = 0; i < lans.count(); i++){
        if(lans[i]->Name().contains(source)){
            lans.insert(i+1,tmp);
            return;
        }
    }
}

void Storage::insertDataAtEnd(QString newItem)
{
    LanItem* li = new LanItem(newItem);
    lans.push_back(li);
}

//Редактирования языка программирования
void Storage::replaceItem(int id, LanItem *item)
{
    lans.replace(id, item);
}

bool Storage::lanExist(QString name)
{
    for(int i = 0; i < lans.count(); i++)
    {
        if(lans.at(i)->Name() == name)
            return true;
    }

    return false;
}

void Storage::addModelItem(Data *item)
{
    if(item != NULL)
        items.push_back(item);
}

int Storage::getCount()
{
    return lans.count();
}

int Storage::getDataCount()
{
    return items.count();
}

Data *Storage::getDataById(int id)
{
    if(id < items.count()){
        return items.at(id);
    }
    return NULL;
}

void Storage::deleteLanByObj(LanItem *item)
{
    if(item != NULL)
        lans.removeOne(item);
    delete item;
}

//Для заполнения комбобокса в MainWindow
QStringList Storage::getList()
{
    QStringList lst;
    for(int i = 0; i < lans.count(); i++){
        lst.append(lans.at(i)->Name());
    }
    return lst;
}

void Storage::up(int id)
{
    LanItem *tmp;
    if(id > 0){
        tmp = lans[id];
        lans[id] = lans[id - 1];
        lans[id - 1] = tmp;
    }
}

void Storage::down(int id)
{
    LanItem *tmp;
    if(id < lans.count() - 1){
        tmp = lans[id];
        lans[id] = lans[id + 1];
        lans[id + 1] = tmp;
    }
}
