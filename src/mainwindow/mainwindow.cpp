#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Библиотека видеокурсов");
    s = new Storage();

    currentLan = NULL;
    currentCourse = NULL;
    currentLesson = NULL;

    s->loadData();//в свою модель
    s->fillLessonModel();//в таблицу для поиска

    ui->cbxLan->addItems(s->getList());

    M = new CourseModel(this);
    //Отправляем индекс из представления в модель
    connect(this, SIGNAL(sendIndextoModel(QModelIndex)),
            M, SLOT(acceptIndexfromView(QModelIndex)));
    createUI();

    //Подсветка поисковой фразы
    highlighter = new RegexpHighlighter(this);
    highlighter->setDocument(ui->edtText->document());
    highlighter->setPattern(ui->edtPattern->text());

    click = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshLans()
{
    ui->cbxLan->clear();
    ui->cbxLan->addItems(s->getList());

//    for(int i = 0; i < s->getCount(); i++)
//    {
//        ui->cbxLan->addItem(QIcon(":/images/cplusplus_2.png"), s->getLanById(i)->Name());
//    }
}

void MainWindow::refreshCourses()
{
    ui->lstCourses->clear();

        for(int i = 0; i < currentLan->getCoursesCount(); i++)
        {
            ui->lstCourses->addItem(currentLan->getCourse(i)->Name());

            if(currentLan->Name() == "C++"){
                ui->lstCourses->item(i)->setIcon(QIcon(":/images/cplusplus_2.png"));
            } else if(currentLan->Name() == "JavaScript"){
                ui->lstCourses->item(i)->setIcon(QIcon(":/images/java-script.png"));
            } else if(currentLan->Name() == "C#"){
                ui->lstCourses->item(i)->setIcon(QIcon(":/images/csharp.png"));
            } else if(currentLan->Name() == "C"){
                ui->lstCourses->item(i)->setIcon(QIcon(":/images/c.png"));
            } else if(currentLan->Name() == "Python"){
                ui->lstCourses->item(i)->setIcon(QIcon(":/images/python.png"));
            } else if(currentLan->Name() == "SQL"){
                ui->lstCourses->item(i)->setIcon(QIcon(":/images/sql.png"));
            } else if(currentLan->Name() == "Java"){
                ui->lstCourses->item(i)->setIcon(QIcon(":/images/java.png"));
            }  else if(currentLan->Name() == "PHP"){
                ui->lstCourses->item(i)->setIcon(QIcon(":/images/php.png"));
            }  else {
                ui->lstCourses->item(i)->setIcon(QIcon(":/images/coding.png"));
            }
        }

    ui->edtText->clear();
    ui->edtText->setEnabled(false);
}


void MainWindow::createUI()
{
    ui->tableView->setModel(M);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->setAlternatingRowColors(false);

    //Добавляем экшены в представление
    ui->tableView->addActions(M->allActions);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}




void MainWindow::on_cbxLan_currentIndexChanged(int index)
{
    ui->lstCourses->clear();
    currentLan = s->getLanById(index);
    refreshCourses();

//    M->clearLessons();
//    M->selectAll();
}

void MainWindow::on_lstCourses_clicked(const QModelIndex &index)
{
    click = false;
    currentCourse = currentLan->getCourse(index.row());


    //Передаём список в модель для отражения в представлении
    QList<Lesson*> lessons = currentCourse->getLessons();
    M->setLessons(&lessons);
    M->setCourseLink(currentCourse->Link());//в модель для показа курса
    ui->edtText->setEnabled(true);
    ui->edtText->setText(currentCourse->courseInfo());
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    click = true;
    emit sendIndextoModel(index);
    currentLesson = currentCourse->getLesson(index.row());
    ui->edtText->setHtml(currentLesson->lessonInfo());
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(index.row() == -1){
        QMessageBox::information(NULL, "Информация",
                                 "Чтобы просмотреть видеозапись, "
                                 "необходимо кликнуть на неё левой кнопкой мыши!");
        return;
    }

    if(currentLesson->Link() == "")return;
    else{
        QDesktopServices::openUrl(QUrl(currentLesson->Link()));
    }
}

void MainWindow::on_actionEditor_triggered()
{
    Editor *Editor;
    Editor = Editor::Current(s);
    connect(this, SIGNAL(shutdown()),Editor, SLOT(shutdown()));
    Editor->show();
}

//Открытие виджета для поиска по всей базе в табличном линейном виде
void MainWindow::on_actionSearch_triggered()
{

    SearchWindow *sw = new SearchWindow(s);
    connect(this, SIGNAL(shutdown()), sw, SLOT(shutdown()));
    sw->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit shutdown();
    QWidget::closeEvent(event);
}


void MainWindow::on_edtPattern_textChanged(const QString &str)
{
    highlighter->setPattern(str);
    highlighter->rehighlight();
}

void MainWindow::on_btnFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                &ok, QFont("MS Shell Dlg 2", 12), this);
    if (ok) {
        ui->edtText->setFont(font);
        QRegExp e("font-size");

        if(click){
            QString txt = currentLesson->lessonInfo().replace(e, "");
            ui->edtText->clear();
            ui->edtText->setText(txt);
        } else {
            QString txt = currentCourse->courseInfo().replace(e, "");
            ui->edtText->clear();
            ui->edtText->setText(txt);
        }
    } else {
            QMessageBox::information(this,"Сообщение","Шрифт не выбран!");
        }
    }
