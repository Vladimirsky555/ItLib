#ifndef COURSEMODEL_H
#define COURSEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QAction>

#include "data/storage.h"

class CourseModel : public QAbstractTableModel
{
    Q_OBJECT

    QList<Lesson*>*lessons;//Список указателей на элементы
    QModelIndex currentIndex;
    QString courseLink;//передаётся из представления

public:
    //Экшены
    QList<QAction*> allActions;
    QAction *actShowCourse;
    QAction *actShowLesson;

public:
    CourseModel(QObject *parent = 0);
    ~CourseModel(){}

    int Count();
    QList<Lesson*> *Lessons();
    void selectAll();
    void addLesson(Lesson *lesson);//В модель
    void setLessons(QList<Lesson*>*lessons);
    void setAllLessons();
    void clearLessons();
    void setCourseLink(QString link);

    //Методы экшенов
protected slots:
    void show_course();
    void show_lesson();

    //Реакция на сигналы из главного окна
private slots:
    void acceptIndexfromView(QModelIndex index);

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
    virtual Lesson *getLesson(const QModelIndex &index)const;
    virtual QVariant    dataToolTip(const QModelIndex &I) const;
    virtual QVariant    dataBackground(const QModelIndex &index) const;//Заливка ячейки
};

#endif // COURSEMODEL_H
