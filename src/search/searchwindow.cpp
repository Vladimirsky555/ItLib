#include "searchwindow.h"
#include "ui_searchwindow.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>

//Отражаем в представлении:
//1) QDate lesson_date;// - дата публикации в youtube
//2) QString lan_name;// - название языка программирования
//3) QString course_name;// - название курса
//4) QString lesson_name;// - название видеоурока
//5) QString lesson_info;// - информация об уроке

SearchWindow::SearchWindow(Storage *s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    setWindowTitle("Поисковик");

    this->s = s;
    M = new LessonModel(this);
    //Отправляем индекс из представления в модель
    connect(this, SIGNAL(sendIndextoModel(QModelIndex)),
            M, SLOT(acceptIndexfromView(QModelIndex)));
    //Получаем из модели ссылку
    connect(M, SIGNAL(sendLink(QString)),
            this, SLOT(acceptLink(QString)));
    //Передаём сигнал о закрытии всех приложений, запущенных в модели
    connect(this, SIGNAL(shutdown_m()),
            M, SLOT(shutdown()));
    //Передаю в модель поисковое слово для передачи дальше
    connect(this, SIGNAL(sendPattern(QString)),
            M, SLOT(acceptPattern(QString)));
    createUI();

    //Берём список из хранилища и передаём в модель
    QList<Data*> items = s->getItems();
    M->setItemsfromView(&items);
    //Запоминаем все данные в модели
    M->setAllItems();

    //Кнопка обновления списка до изначального
    {
        ui->btnRefresh->setDefaultAction(ui->actionRefreshList);
        connect(ui->actionRefreshList, SIGNAL(triggered()),this, SLOT(refreshList()));
        QPixmap p(":/images/ex.png");
        ui->actionRefreshList->setIcon(QIcon(p));
    }{
        //Кнопка очистки перед поиском
        ui->btnClear->setDefaultAction(ui->actionClear);
        connect(ui->actionClear, SIGNAL(triggered()),this, SLOT(searchClear()));
        QPixmap p(":/images/clear.png");
        ui->actionClear->setIcon(QIcon(p));
    }

    //Ставим текущую дату при открытии
    ui->dateEdit->setDate(QDate::currentDate());

    //Картинка для красоты
    QPixmap pix(":/images/progr.png");
    ui->lblPix->setPixmap(pix.scaledToWidth(250));

    //Заполняем комбобокс значениями из хранилища
    ui->cbxLan->addItem("---");
    ui->cbxLan->addItems(s->getList());
}

SearchWindow::~SearchWindow()
{
    delete ui;
}

void SearchWindow::createUI()
{
    ui->tableView->setModel(M);
//    connect(this, SIGNAL(sendPattern(QString)), M, SLOT(acceptPattern(QString)));
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->setColumnHidden(4, true);
    ui->tableView->setAlternatingRowColors(false);

    //Добавляем экшены в представление
    ui->tableView->addActions(M->allActions);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

bool SearchWindow::check_index(int index)
{
    if(index == 0){
        return false;
    }

    return true;
}

void SearchWindow::shutdown()
{
    //Передаём сигнал о закрытии основного окна в модель
    emit shutdown_m();
    close();
}

void SearchWindow::searchClear()
{
    ui->cbxLan->setCurrentIndex(0);
    ui->edtPattern->setText("");
    ui->rYear->setChecked(true);
    ui->cbDate->setChecked(false);
}


void SearchWindow::refreshList()
{
    M->clearItems();//очищаем модель перед поиском
    M->setItems(M->All_Items());//Заполняем модель всеми данными
    M->selectAll();//Обновляем представление
}

void SearchWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
//    qDebug() << link;

    if(index.row() == -1){
        QMessageBox::information(NULL, "Информация",
                                 "Чтобы просмотреть видеозапись, "
                                 "необходимо кликнуть на неё левой кнопкой мыши!");
        return;
    }

    if(link == "")return;
    else{
        QDesktopServices::openUrl(QUrl(link));
    }
}

void SearchWindow::acceptLink(QString link)
{
    this->link = link;
}

void SearchWindow::on_tableView_clicked(const QModelIndex &index)
{
    emit sendIndextoModel(index);
}

void SearchWindow::closeEvent(QCloseEvent *event)
{
    //При закрытии этого окна передаём в модель сигнал о закрытии виджетов,
    //запущенных из модели
    emit shutdown_m();
    QWidget::closeEvent(event);
}


void SearchWindow::on_btnSearch_clicked()
{
    //Объявляем локальные переменные для поиска
    QDate date; bool _date;
    QString lan; bool _lan;
    QString info; bool _info;
    int flag;

    //Заполняем массив items, иначе не будет результатов поиска
    M->clearItems();//очищаем модель перед поиском
    M->setItems(M->All_Items());//Заполняем модель всеми данными
    M->selectAll();//Обновляем представление

    //Определяем дату для поиска
    date = ui->dateEdit->date();

    if(ui->rYear->isChecked()){
        flag = 1;
    } else if(ui->rMonth->isChecked()){
        flag = 2;
    } else if(ui->rDay->isChecked()){
        flag = 3;
    }

    //Определяем язык программирования для поиска
    for(int i = 0; i < s->getCount(); i++)
    {
        if(ui->cbxLan->currentIndex() == i+1)
        {
            lan = s->getLanById(i)->Name();
        }
    }

    //Определяем ключевое слово для поиска
    info = ui->edtPattern->text();

    //Определяем какие параметры для поиска использовать
    if(ui->cbDate->isChecked()){
        _date = true;
    } else {
        _date = false;
    }

    if(ui->edtPattern->text() == ""){
        _info = false;
    } else {
        _info = true;
    }
    _lan = check_index(ui->cbxLan->currentIndex());

    //Запускаем поиск
    M->search(date, _date, flag, lan, _lan, info, _info);
}

//Передаём поисковое слово для тех, кто подписался на событие
void SearchWindow::on_edtPattern_textChanged(const QString &str)
{
    emit sendPattern(str);
}
