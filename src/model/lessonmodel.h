#ifndef LESSONMODEL_H
#define LESSONMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QAction>

#include "data/data.h"
#include "search/showinfo.h"

class LessonModel : public QAbstractTableModel
{
    Q_OBJECT

    QList<Data*>*items;//Список указателей на элементы
    QList<Data*>*s_items;
    QList<Data*>*all_items;
    QModelIndex currentIndex;
    QString pattern;//поисковое слово

public:
    //Экшены
    QList<QAction*> allActions;
    QAction *actShowCourse;
    QAction *actShowLesson;
    QAction *actShowInfo;

public:
    LessonModel(QObject *parent = 0);
    virtual ~LessonModel(){}

    //передаём список из представления
    //в представлении он оказался из MainWindow, при создании представления
    void setItemsfromView(QList<Data*>*items);
    int getCount();
    QList<Data*> *Items();
    QList<Data*> *All_Items();

    void addItem(Data *item);//В модель
    void setItems(QList<Data*>*items);

    bool checkRegExp(QRegExp rx);
    void selectAll();

    bool checkInfo(QString pattern, QString info);
    void search(QDate date, bool _date, int flag, QString lan, bool _lan,
                       QString pattern, bool _info);

    void setAllItems();//Запускается один раз в начале
    void setItems();
    void clearItems();

    //Реакция на сигналы из главного окна
private slots:
    void shutdown();
    void acceptIndexfromView(QModelIndex index);
    void acceptPattern(QString pattern);

protected:
    Data *getData(const QModelIndex &I)const;

    //Методы экшенов
protected slots:
    void show_course();
    void show_lesson();
    void show_info();

signals:
    void sendLink(QString link);
    void shutdown_w();

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &) const;

    //Вспомогательные функции
protected:
    virtual QVariant    dataDisplay(const QModelIndex &index) const;
    virtual QVariant    dataTextAlignment(const QModelIndex &) const;
    virtual Data *getItem(const QModelIndex &index)const;
    virtual QVariant    dataToolTip(const QModelIndex &I) const;
    virtual QVariant    dataBackground(const QModelIndex &I) const;//Заливка ячейки
};

#endif // LESSONMODEL_H

