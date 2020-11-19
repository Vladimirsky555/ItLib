#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QAction>
#include <QMenu>

#include "data/storage.h"


namespace Ui {
class Editor;
}

class Editor : public QWidget
{
    Q_OBJECT

    Storage *s;
    static Editor *_instance;

    LanItem *currentLan;
    Course *currentCourse;
    Lesson *currentLesson;

    QList<QAction*> lanActions;
    QAction *lanUp;
    QAction *lanDown;
    QAction *lanInsertFirst;
    QAction *lanInsert;
    QAction *lanInsertEnd;
    QAction *lanEdit;
    QAction *lanDelete;

    QList<QAction*> courseActions;
    QAction *courseUp;
    QAction *courseDown;
    QAction *courseInsertFirst;
    QAction *courseInsert;
    QAction *courseInsertEnd;
    QAction *courseEdit;
    QAction *courseDelete;

    QList<QAction*> lessonActions;
    QAction *lessonUp;
    QAction *lessonDown;
    QAction *lessonInsertFirst;
    QAction *lessonInsert;
    QAction *lessonInsertEnd;
    QAction *lessonEdit;
    QAction *lessonDelete;

private:
    Editor(Storage *s, QWidget *parent = 0);
    ~Editor();

public:
     static Editor *Current(Storage *s);
     void addActions();
     void refreshLans();
     void refreshCourses();
     void refreshLessons();

private slots:
    void contextMenuRequsted(const QPoint &p);
    void shutdown();

    //События
    void lan_Up();
    void lan_Down();
    void lan_Insert_First();
    void lan_Insert();
    void lan_Insert_End();
    void lan_Edit();
    void lan_Delete();

    void course_Up();
    void course_Down();
    void course_Insert_First();
    void course_Insert();
    void course_Insert_End();
    void course_Edit();
    void course_Delete();

    void lesson_Up();
    void lesson_Down();
    void lesson_Insert_First();
    void lesson_Insert();
    void lesson_Insert_End();
    void lesson_Edit();
    void lesson_Delete();

    void on_lstLans_clicked(const QModelIndex &index);
    void on_lstCourses_clicked(const QModelIndex &index);
    void on_lstLessons_clicked(const QModelIndex &index);
    void on_btnSaveCatalog_clicked();

private:
    Ui::Editor *ui;
};

#endif // EDITOR_H
