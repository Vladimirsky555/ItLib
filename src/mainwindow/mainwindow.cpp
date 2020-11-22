#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFontDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVariantMap>
#include <QThreadPool>
#include <QMutex>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mNetManager(new QNetworkAccessManager(this)),
    mNetReply(NULL)
{
    ui->setupUi(this);
    setWindowTitle("Библиотека видеокурсов");
    s = new Storage();

    currentLan = NULL;
    currentCourse = NULL;
    currentLesson = NULL;


    s->loadData();//в свою модель
    s->fillLessonModel();//в таблицу для поиска

    //Считываем данные в табличную модель с сервера, если никаких данных нет
    if(s->getDataCount() == 0){
        //Initialize our API data
        const QUrl API_ENDPOINT("http://vladimir-bervin.myjino.ru/var/it.json");
        //    const QUrl API_ENDPOINT("http://hoondok.ru/ss.json");
        QNetworkRequest request;
        request.setUrl(API_ENDPOINT);

        mNetReply = mNetManager->get(request);
        connect(mNetReply,&QIODevice::readyRead,this,&MainWindow::dataReadyRead);
        connect(mNetReply,&QNetworkReply::finished,this,&MainWindow::dataReadFinished);
    }

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

    //Кнопка для экспорта в JSON
    ui->btnExportToJSON->setDefaultAction(ui->actionExportToJSON);//привязали к toolbutton
    connect(ui->actionExportToJSON, SIGNAL(triggered()),this, SLOT(ExportToJSON()));
    QPixmap pExp(":/images/json.png");
    ui->actionExportToJSON->setIcon(QIcon(pExp));

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

//Заполняем нашу иерархическую модель из данных, полученных с сервера
//то есть списочной модели для поиска
void MainWindow::restoreDataFromModel()
{
    for(int i =0; i < s->getDataCount(); i++)
    {
        if(!s->lanExist(s->getDataById(i)->lanName())){
            currentLan = new LanItem(s->getDataById(i)->lanName());
            s->addLanItem(currentLan);
        } else {
            currentLan = s->getLanByName(s->getDataById(i)->lanName());

            if(!currentLan->courseExist(s->getDataById(i)->courseName())){
                currentCourse = new Course(s->getDataById(i)->courseName(),
                                        s->getDataById(i)->courseLink(),
                                        s->getDataById(i)->courseInfo());
                currentLan->addCourse(currentCourse);
            } else {
                currentCourse = currentLan->getcourseByName(s->getDataById(i)->courseName());

                if(!currentCourse->lessonExist(s->getDataById(i)->lessonName())){
                    currentLesson = new Lesson(s->getDataById(i)->lessonDate(),
                                           s->getDataById(i)->lessonName(),
                                           s->getDataById(i)->lessonLink(),
                                           s->getDataById(i)->lessonInfo());
                    currentCourse->addLesson(currentLesson);
                }
            }
        }
    }
}

void MainWindow::on_cbxLan_currentIndexChanged(int index)
{
    ui->lstCourses->clear();
    currentLan = s->getLanById(index);
    refreshCourses();
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

void MainWindow::dataReadyRead()
{
    arr.append(mNetReply->readAll());
}

void MainWindow::dataReadFinished()
{
    if( mNetReply->error())
    {
        qDebug() << "Error : " << mNetReply->errorString();
    }else
    {
       QJsonDocument doc = QJsonDocument::fromJson(arr);
       QJsonArray array = doc.array();
       QThreadPool* pool = QThreadPool::globalInstance();
       QMutex mutex(QMutex::Recursive);

       for (int i = 0; i < array.size(); i++)
       {
           QJsonObject object = array.at(i).toObject();

           JsonWorker* jw = new JsonWorker(s, object, &mutex, nullptr);
           jw->setAutoDelete(true);
           pool->start(jw);
       }

       pool->waitForDone();
    }

    //C сервера получаем только линейные данные табличной модели
    //Их необходимо трансформировать в нашу иерархическую
    restoreDataFromModel();
    ui->cbxLan->addItems(s->getList());//Выводим список языков программирования
    s->saveData();//Сохраняем данные в файл на будущее
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

//Экспорт в JSON
void MainWindow::ExportToJSON()
{
    QJsonObject textObject;

    for(int i = 0; i < s->getDataCount(); i++){
        Data* item = s->getDataById(i);

        textObject["lan_name"] = item->lanName();
        textObject["course_name"] = item->courseName();
        textObject["course_link"] = item->courseLink();
        textObject["course_info"] = item->courseInfo();
        textObject["lesson_date"] = item->lessonDate().toString("dd.MM.yyyy");
        textObject["lesson_name"] = item->lessonName();
        textObject["lesson_link"] = item->lessonLink();
        textObject["lesson_info"] = item->lessonInfo();

        QJsonArray textsArray = currentJsonObject["list"].toArray();
        textsArray.append(textObject);
        currentJsonObject["list"] = textsArray;
    }

    // С помощью диалогового окна получаем имя файла с абсолютным путём
       QString saveFileName = QFileDialog::getSaveFileName(this,
                                                           tr("Save Json File"),
                                                           QString(),
                                                           tr("JSON (*.json)"));
       QFileInfo fileInfo(saveFileName);   // С помощью QFileInfo
       QDir::setCurrent(fileInfo.path());  // установим текущую рабочую директорию, где будет файл, иначе может не заработать
       // Создаём объект файла и открываем его на запись
       QFile jsonFile(saveFileName);
       if (!jsonFile.open(QIODevice::WriteOnly))
       {
           return;
       }

       // Записываем текущий объект Json в файл
       jsonFile.write(QJsonDocument(currentJsonObject).toJson(QJsonDocument::Indented));
       jsonFile.close();   // Закрываем файл
}
