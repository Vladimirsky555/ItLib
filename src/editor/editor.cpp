#include "editor.h"
#include "ui_editor.h"

#include "QMessageBox"
#include <QFont>
#include <QDebug>

#include "dialog/dialog.h"
#include "dialog/coursedialog.h"
#include "dialog/lessondialog.h"

Editor* Editor::_instance = NULL;

Editor *Editor::Current(Storage *s)
{
    if (_instance == NULL)
        _instance = new Editor(s);

    return _instance;
}

Editor::Editor(Storage *s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    setWindowTitle("Редактор каталогов");

    this->s = s;

    currentLan = NULL;
    currentCourse = NULL;
    currentLesson = NULL;

    ui->lstCourses->setEnabled(false);
    ui->lstLessons->setEnabled(false);
    ui->edtInfo->setEnabled(false);

    refreshLans();
    addActions();

    ui->lstLans->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->lstCourses->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->lstLessons->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequsted(QPoint)));

}

Editor::~Editor()
{
    delete ui;
}

void Editor::addActions()
{
    //События списка языков программирования
    {
        QAction *A = lanInsertFirst = new QAction(this);
        QPixmap p(":/images/insert-in-the-front.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+Q"));
        A->setText(tr("Вставить в начало"));
        connect(A, SIGNAL(triggered()),this, SLOT(lan_Insert_First()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLans->addAction(A);
        lanActions << A;
    }{
        QAction *A = lanInsert = new QAction(this);
        QPixmap p(":/images/insert-inside.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+W"));
        A->setText(tr("Вставить"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(lan_Insert()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLans->addAction(A);
        lanActions << A;
    }{
        QAction *A = lanInsertEnd = new QAction(this);
        QPixmap p(":/images/insert-at-the-end.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+E"));
        A->setText(tr("Вставить в конец"));
        connect(A, SIGNAL(triggered()),this, SLOT(lan_Insert_End()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLans->addAction(A);
        lanActions << A;
    }{
        QAction *A = lanUp = new QAction(this);
        QPixmap p(":/images/up.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+R"));
        A->setText(tr("Вверх"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(lan_Up()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLans->addAction(A);
        lanActions << A;
    }{
        QAction *A = lanDown = new QAction(this);
        QPixmap p(":/images/down.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+T"));
        A->setText(tr("Вниз"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(lan_Down()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLans->addAction(A);
        lanActions << A;
    }{
        QAction *A = lanEdit = new QAction(this);
        QPixmap p(":/images/edit.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+Y"));
        A->setText(tr("Редактировать"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(lan_Edit()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLans->addAction(A);
        lanActions << A;
    }{
        QAction *A = lanDelete = new QAction(this);
        QPixmap p(":/images/delete.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+U"));
        A->setText(tr("Удалить"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(lan_Delete()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLans->addAction(A);
        lanActions << A;
    }

    //События списка курсов
    {
        QAction *A = courseInsertFirst = new QAction(this);
        QPixmap p(":/images/insert-in-the-front.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+A"));
        A->setText(tr("Вставить в начало"));
        connect(A, SIGNAL(triggered()),this, SLOT(course_Insert_First()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstCourses->addAction(A);
        courseActions << A;
    }{
        QAction *A = courseInsert = new QAction(this);
        QPixmap p(":/images/insert-inside.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+S"));
        A->setText(tr("Вставить"));
        connect(A, SIGNAL(triggered()),this, SLOT(course_Insert()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstCourses->addAction(A);
        courseActions << A;
    }  {
        QAction *A = courseInsertEnd = new QAction(this);
        QPixmap p(":/images/insert-at-the-end.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+D"));
        A->setText(tr("Вставить в конец"));
        connect(A, SIGNAL(triggered()),this, SLOT(course_Insert_End()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstCourses->addAction(A);
        courseActions << A;
    }{
        QAction *A = courseUp = new QAction(this);
        QPixmap p(":/images/up.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+F"));
        A->setText(tr("Вверх"));
        connect(A, SIGNAL(triggered()),this, SLOT(course_Up()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstCourses->addAction(A);
        courseActions << A;
    }{
        QAction *A = courseDown = new QAction(this);
        QPixmap p(":/images/down.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+G"));
        A->setText(tr("Вниз"));
        connect(A, SIGNAL(triggered()),this, SLOT(course_Down()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstCourses->addAction(A);
        courseActions << A;
    }{
        QAction *A = courseEdit = new QAction(this);
        QPixmap p(":/images/edit.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+H"));
        A->setText(tr("Редактировать"));
        connect(A, SIGNAL(triggered()),this, SLOT(course_Edit()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstCourses->addAction(A);
        courseActions << A;
    }{
        QAction *A = courseDelete = new QAction(this);
        QPixmap p(":/images/delete.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+D"));
        A->setText(tr("Удалить"));
        connect(A, SIGNAL(triggered()),this, SLOT(course_Delete()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstCourses->addAction(A);
        courseActions << A;
    }

    //События списка уроков
    {
        QAction *A = lessonInsertFirst = new QAction(this);
        QPixmap p(":/images/insert-in-the-front.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+Z"));
        A->setText(tr("Вставить в начало"));
        connect(A, SIGNAL(triggered()),this, SLOT(lesson_Insert_First()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLessons->addAction(A);
        lessonActions << A;
    }{
        QAction *A = lessonInsert = new QAction(this);
        QPixmap p(":/images/insert-inside.png");
        A->setIcon(QIcon(p)); A->setShortcut(tr("Ctrl+X"));
        A->setText(tr("Вставить"));
        connect(A, SIGNAL(triggered()),this, SLOT(lesson_Insert()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLessons->addAction(A);
        lessonActions << A;
    }{
        QAction *A = lessonInsertEnd = new QAction(this);
        QPixmap p(":/images/insert-at-the-end.png");
        A->setIcon(QIcon(p)); A->setShortcut(tr("Ctrl+C"));
        A->setText(tr("Вставить в конец"));
        connect(A, SIGNAL(triggered()),this, SLOT(lesson_Insert_End()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLessons->addAction(A);
        lessonActions << A;
    }{
        QAction *A = lessonUp = new QAction(this);
        QPixmap p(":/images/up.png");
        A->setIcon(QIcon(p)); A->setShortcut(tr("Ctrl+V"));
        A->setText(tr("Вверх"));
        connect(A, SIGNAL(triggered()),this, SLOT(lesson_Up()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLessons->addAction(A);
        lessonActions << A;
    }{
        QAction *A = lessonDown = new QAction(this);
        QPixmap p(":/images/down.png");
        A->setIcon(QIcon(p)); A->setShortcut(tr("Ctrl+B"));
        A->setText(tr("Вниз"));
        connect(A, SIGNAL(triggered()),this, SLOT(lesson_Down()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLessons->addAction(A);
        lessonActions << A;
    }{
        QAction *A = lessonEdit = new QAction(this);
        QPixmap p(":/images/edit.png");
        A->setIcon(QIcon(p)); A->setShortcut(tr("Ctrl+N"));
        A->setText(tr("Редактировать"));
        connect(A, SIGNAL(triggered()),this, SLOT(lesson_Edit()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLessons->addAction(A);
        lessonActions << A;
    }{
        QAction *A = lessonDelete = new QAction(this);
        QPixmap p(":/images/delete.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+M"));
        A->setText(tr("Удалить"));
        connect(A, SIGNAL(triggered()),this, SLOT(lesson_Delete()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstLessons->addAction(A);
        lessonActions << A;
    }
}

void Editor::refreshLans()
{
    ui->lstLans->clear();

    for(int i = 0; i < s->getCount(); i++)
    {
        ui->lstLans->addItem(s->getLanById(i)->Name());
        ui->lstLans->item(i)->setIcon(QIcon(":/images/coding.png"));
    }

    ui->lstCourses->clear();
    ui->lstLessons->clear();
    ui->edtInfo->clear();
}

void Editor::refreshCourses()
{
    ui->lstCourses->clear();

    for(int i = 0; i < currentLan->getCoursesCount(); i++)
    {
        ui->lstCourses->addItem(currentLan->getCourse(i)->Name());
        ui->lstCourses->item(i)->setIcon(QIcon(":/images/programming.png"));
    }

    ui->lstLessons->clear();
    ui->edtInfo->clear();
}

void Editor::refreshLessons()
{
    ui->lstLessons->clear();

    for(int i = 0; i < currentCourse->getLessonCount(); i++)
    {
        ui->lstLessons->addItem(currentCourse->getLesson(i)->Name());
        ui->lstLessons->item(i)->setIcon(QIcon(":/images/lesson.png"));

    }

    ui->edtInfo->clear();
}

void Editor::contextMenuRequsted(const QPoint &p)
{
    QMenu lan(this);
    lan.addActions(lanActions);
    lan.exec(mapToGlobal(p));

    QMenu course(this);
    course.addActions(courseActions);
    course.exec(mapToGlobal(p));

    QMenu lesson(this);
    lesson.addActions(lessonActions);
    lesson.exec(mapToGlobal(p));
}

void Editor::shutdown()
{
    close();
}

void Editor::lan_Up()
{
    s->up(ui->lstLans->currentRow());
    refreshLans();
}

void Editor::lan_Down()
{
    s->down(ui->lstLans->currentRow());
    refreshLans();
}

void Editor::lan_Insert_First()
{
    Dialog ld;
    ld.setEdit(true);
    ld.exec();

    if (ld.getName() == "")
    {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    s->insertDataFirst(ld.getName());
    refreshLans();

}

void Editor::lan_Insert()
{
    if(currentLan == NULL) return;

    Dialog ld;
    ld.setEdit(true);
    ld.exec();

    if (ld.getName() == "") {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    s->insertData(currentLan->Name(), ld.getName());
    refreshLans();
}

void Editor::lan_Insert_End()
{
    Dialog ld;
    ld.setEdit(true);
    ld.exec();

    if (ld.getName() == "") {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    s->insertDataAtEnd(ld.getName());
    refreshLans();
}

void Editor::lan_Edit()
{
    Dialog ld;
    if(currentLan != NULL){
        ld.setName(currentLan->Name());
    } else {
        QMessageBox::information(this, "Предупреждение!", "Вы не выбрали ни одного "
                                                          "языка программирования!");
        return;
    }
    ld.exec();

    currentLan->setName(ld.getName());
    refreshLans();
}

void Editor::lan_Delete()
{
    if(currentLan == NULL) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Вопрос!", "Вы действительно хотите удалить раздел?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        //Удаляем указатель массива

        s->deleteLanByObj(currentLan);
        refreshLans();

        ui->lstCourses->clear();
        ui->lstLessons->clear();
        ui->edtInfo->clear();
        ui->lstCourses->setEnabled(false);
        ui->lstLessons->setEnabled(false);
        ui->edtInfo->setEnabled(false);

        currentLan = NULL;
    } else {
        return;
    }
}

void Editor::course_Up()
{
    currentLan->up(ui->lstCourses->currentRow());
    refreshCourses();
}

void Editor::course_Down()
{
    currentLan->down(ui->lstCourses->currentRow());
    refreshCourses();
}

void Editor::course_Insert_First()
{
    CourseDialog cd;
    cd.exec();

    if (cd.getName() == "" && cd.getLink() =="")
    {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    currentLan->insertDataFirst(cd.getName(), cd.getLink(), cd.getInfo());
    refreshCourses();
}

void Editor::course_Insert()
{
    if(currentCourse == NULL) return;

    CourseDialog cd;
    cd.exec();

    if (cd.getName() == "" && cd.getLink() == "")
    {
        QMessageBox::information(this, "Информация!", "Файл не может быть пустым!");
        return;
    }

    currentLan->insertData(currentCourse->Name(), cd.getName(),
                           cd.getLink(), cd.getInfo());
    refreshCourses();
}

void Editor::course_Insert_End()
{
    CourseDialog cd;
    cd.exec();

    if (cd.getName() == "" && cd.getLink() == "")
    {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    currentLan->insertDataAtEnd(cd.getName(), cd.getLink(), cd.getInfo());
    refreshCourses();
}

void Editor::course_Edit()
{
    CourseDialog cd;
    if(currentCourse != NULL){
    cd.setName(currentCourse->Name());
    cd.setLink(currentCourse->Link());
    cd.setInfo(currentCourse->courseInfo());
    } else {
        QMessageBox::information(this, "Предупреждение!", "Вы не выбрали ни одной книги!");
        return;
    }
    cd.exec();

    currentCourse->setName(cd.getName());
    currentCourse->setLink(cd.getLink());
    currentCourse->setCourseInfo(cd.getInfo());
    refreshCourses();
}

void Editor::course_Delete()
{
    if(currentCourse == NULL) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Вопрос!", "Удаляем курс?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        currentLan->deleteCourseByObj(currentCourse);

        refreshCourses();
        ui->lstLessons->clear();
        ui->edtInfo->clear();
        currentCourse = NULL;
    } else {
        return;
    }
}

void Editor::lesson_Up()
{
    currentCourse->up(ui->lstLessons->currentRow());
    refreshLessons();
}

void Editor::lesson_Down()
{
    currentCourse->down(ui->lstLessons->currentRow());
    refreshLessons();
}

void Editor::lesson_Insert_First()
{
    LessonDialog ld;
    ld.exec();

    if (ld.getName() == "" && ld.getLink() =="")
    {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    currentCourse->insertDataFirst(ld.getDate(), ld.getName(),
                                   ld.getLink(), ld.getInfo());
    refreshLessons();
}

void Editor::lesson_Insert()
{
    if(currentLesson == NULL) return;

    LessonDialog ld;
    ld.exec();

    if (ld.getName() == "" && ld.getLink() == "")
    {
        QMessageBox::information(this, "Информация!", "Файл не может быть пустым!");
        return;
    }

    currentCourse->insertData(currentCourse->Name(), ld.getDate(),ld.getName(),
                              ld.getLink(), ld.getInfo());
    refreshLessons();
}

void Editor::lesson_Insert_End()
{
    LessonDialog ld;
    ld.exec();

    if (ld.getName() == "" && ld.getLink() == "")
    {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    currentCourse->insertDataAtEnd(ld.getDate(), ld.getName(),
                                   ld.getLink(), ld.getInfo());
    refreshLessons();
}

void Editor::lesson_Edit()
{
    LessonDialog ld;
    if(currentLesson != NULL){
        ld.setDate(currentLesson->Date());
        ld.setName(currentLesson->Name());
        ld.setLink(currentLesson->Link());
        ld.setInfo(currentLesson->lessonInfo());
    } else {
        QMessageBox::information(this, "Предупреждение!",
                                 "Вы не выбрали ни одной книги!");
        return;
    }

    ld.exec();

    currentLesson->setDate(ld.getDate());
    currentLesson->setName(ld.getName());
    currentLesson->setLink(ld.getLink());
    currentLesson->setLessonInfo(ld.getInfo());
    refreshLessons();
}

void Editor::lesson_Delete()
{
    if(currentLesson == NULL) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Вопрос!", "Удаляем урок?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        currentCourse->deleteLessonByObj(currentLesson);

        refreshLessons();

        ui->edtInfo->clear();
        currentLesson = NULL;
    } else {
        return;
    }
}


void Editor::on_lstLans_clicked(const QModelIndex &index)
{
    int id = index.row();
    ui->lstCourses->setEnabled(true);

    if (id == -1) return;
    currentLan = s->getLanById(id);

    ui->lstCourses->setEnabled(true);
    ui->lstLessons->setEnabled(false);
    ui->edtInfo->setEnabled(false);
    refreshCourses();
}

void Editor::on_lstCourses_clicked(const QModelIndex &index)
{
    int id = index.row();
    if (id == -1) return;
    currentCourse = currentLan->getCourse(id);
    QString tmp = currentCourse->courseInfo();
    ui->lstLessons->setEnabled(true);
    ui->edtInfo->setEnabled(true);
    refreshLessons();
    ui->edtInfo->setHtml(tmp);
}

void Editor::on_lstLessons_clicked(const QModelIndex &index)
{
    int id = index.row();
    if (id == -1) return;
    currentLesson = currentCourse->getLesson(id);

    ui->edtInfo->setEnabled(true);
    ui->edtInfo->setHtml(currentLesson->lessonInfo());
}

void Editor::on_btnSaveCatalog_clicked()
{
    s->saveData();
    close();
}
