#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>


#include "lanitem.h"
#include "data.h"

class Storage : public QObject
{
    Q_OBJECT

    QList<LanItem*> lans;//элементы иерархической самопальной модели
    QString path;//Путь к серверу для скачивания при первом запуске
    QList<Data*> items;//элементы табличной линейной модели

    LanItem *currentLan;
    Course *currentCourse;
    Lesson *currentLesson;

public:
    Storage(QObject *parent = 0);

    //Загрузка и сохранение в файл
    void loadData();
    void saveData();
    void loadPath();
    void savePath();

    QString getPath();
    void setPath(QString path);//Передайм путь к серверу, установив его в основном окне

    //Иерархическая модель
    LanItem* getLanById(int id);
    LanItem* getLanByName(QString name);
    QList<LanItem*> LanItems();
    int getCount();
    void deleteLanByObj(LanItem *item);
    QStringList getList();
    void up(int id);
    void down(int id);

    //Admin
    void addLanItem(LanItem *item);
    void restoreDataFromModel();//Функция для трансформации данных, полученных с сервера
    void insertDataFirst(QString newItem);
    void insertData(QString source, QString newItem);
    void insertDataAtEnd(QString newItem);
    void replaceItem(int, LanItem *item);
    bool lanExist(QString name);//для трансформации из данных сервера

    //Табличная модель
    void addModelItem(Data *item);//При считывании с сервера
    int getDataCount();
    Data* getDataById(int id);
    void fillLessonModel();//Заполняем модель для вывода в табличное представление
    QList<Data*>getItems();//для установки в модель


};

#endif // STORAGE_H
