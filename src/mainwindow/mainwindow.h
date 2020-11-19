#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>

#include "data/storage.h"
#include "editor/editor.h"
#include "search/searchwindow.h"
#include "model/coursemodel.h"
#include "helpers/regexphighlighter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

     Storage *s;
     CourseModel *M;
     LanItem *currentLan;
     Course* currentCourse;
     Lesson* currentLesson;
     bool click;
     QJsonObject currentJsonObject;

     //для поиска
     QString pattern;
     RegexpHighlighter *highlighter;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void refreshLans();
    void refreshCourses();
    void createUI();

private slots:
    void on_actionEditor_triggered();
    void on_actionSearch_triggered();
    void on_cbxLan_currentIndexChanged(int index);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_edtPattern_textChanged(const QString &str);
    void on_btnFont_clicked();
    void ExportToJSON();
    //Клики по полю
    void on_lstCourses_clicked(const QModelIndex &index);
    void on_tableView_clicked(const QModelIndex &index);

signals:
    void sendIndextoModel(QModelIndex index);
    void shutdown();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
