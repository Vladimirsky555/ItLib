#include "lessonmodel.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QRegExp>
#include <QDebug>

LessonModel::LessonModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    items = new QList<Data*>;
    s_items = new QList<Data*>;
    all_items = new QList<Data*>;

    //Список экшенов
   {
        QAction *A = actShowLesson = new QAction(tr("Ссылка на урок"), this);
        QPixmap p(":/images/lesson_book.png"); A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+L"));
        connect(A, SIGNAL(triggered()), this, SLOT(show_lesson()));
        allActions << A;
    } {
        QAction *A = actShowCourse = new QAction(tr("Ссылка на курс"), this);
        QPixmap p(":/images/course.png"); A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+F"));
        connect(A, SIGNAL(triggered()), this, SLOT(show_course()));
        allActions << A;
    }{
        QAction *A = actShowInfo = new QAction(tr("Информация"), this);
        QPixmap p(":/images/info.png"); A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+I"));
        connect(A, SIGNAL(triggered()), this, SLOT(show_info()));
        allActions << A;
    }
}

void LessonModel::setItemsfromView(QList<Data *> *items)
{
    this->items->clear();
    for(int i = 0; i < items->count(); i++)
    {
        this->items->append(items->at(i));
    }

   beginResetModel();
   endResetModel();
}

int LessonModel::getCount()
{
    return items->count();
}

QList<Data *> *LessonModel::Items()
{
    return this->items;
}


QList<Data *> *LessonModel::All_Items()
{
    return this->all_items;
}

void LessonModel::addItem(Data *item)
{
    if(item != NULL){
        items->append(item);
    }
    return;
}

void LessonModel::setItems(QList<Data *> *items)
{
    this->items->clear();
    for(int i = 0; i < items->count(); i++){
        this->items->append(items->at(i));
    }
}

//Используется один раз при загрузке данных для копирования
void LessonModel::setAllItems()
{
    for(int i = 0; i < items->count(); i++)
    {
        this->all_items->append(items->at(i));
    }
}

void LessonModel::clearItems()
{
    items->clear();
}

bool LessonModel::checkRegExp(QRegExp rx)
{
    if(rx.isValid() && !rx.isEmpty() && !rx.exactMatch("")){
           return true;
       } else {
           QMessageBox::information(NULL,"Информсообщение",
                                  trUtf8("Некорректный шаблон регулярного выражения!"));
           return false;
    }    
}

void LessonModel::selectAll()
{
    beginResetModel();
    endResetModel();
}

bool LessonModel::checkInfo(QString pattern, QString info)
{
    QRegExp rx(pattern);
    if(!checkRegExp(rx))return false;
    int pos = 0;
    while((pos = rx.indexIn(info, pos)) != -1){
        pos += rx.matchedLength();
        return true;
    }

    return false;
}


void LessonModel::shutdown()
{
    //Получили сигнал о закрытии из представления, передаём дальше
    emit shutdown_w();
}

void LessonModel::acceptIndexfromView(QModelIndex index)
{
//    qDebug() << getData(currentIndex)->lessonLink();
    this->currentIndex = index;
    emit sendLink(getData(currentIndex)->lessonLink());
}

void LessonModel::acceptPattern(QString pattern)
{
    this->pattern = pattern;
}

Data *LessonModel::getData(const QModelIndex &I) const
{
    int id = I.row();
    if(id < 0 || id >= items->size()) return NULL;
    return items->at(id);
}

void LessonModel::show_course()
{
    Data* item = getItem(currentIndex);

    if(currentIndex.row() == -1){
        QMessageBox::information(NULL, "Информация",
                                 "Чтобы просмотреть видеозапись, "
                                 "необходимо кликнуть на неё левой кнопкой мыши!");
        return;
    }

    if(item->courseLink() == "")return;
    else{
        QDesktopServices::openUrl(QUrl(item->courseLink()));
    }
}

void LessonModel::show_lesson()
{
    Data* item = getItem(currentIndex);

    if(currentIndex.row() == -1){
        QMessageBox::information(NULL, "Информация",
                                 "Чтобы просмотреть видеозапись, "
                                 "необходимо кликнуть на неё левой кнопкой мыши!");
        return;
    }

    if(item->lessonLink() == "")return;
    else{
        QDesktopServices::openUrl(QUrl(item->lessonLink()));
    }
}

void LessonModel::show_info()
{
    Data* item = getItem(currentIndex);
    ShowInfo *si = new ShowInfo(item->courseInfo(),
                                item->lessonInfo(),
                                item->lessonName(),
                                item->courseName(),
                                pattern);

    //Сигнал переданный от представления shutdown(), модель передаёт
    //свой сигнал shutdown_w()
    connect(this, SIGNAL(shutdown_w()),
            si, SLOT(shutdown()));
    si->show();
}

int LessonModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return items->count();
    } else {
        return 0;
    }
}

int LessonModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return 4;
    } else {
        return 0;
    }
}

QVariant LessonModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:             return dataDisplay(index);
    case Qt::TextAlignmentRole: return dataTextAlignment(index);
    case Qt::ToolTipRole:             return dataToolTip(index);
    case Qt::BackgroundRole:    return dataBackground(index);
    default:  return QVariant();
    }
}

QVariant LessonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //Для горизонтальных заголовков оставляем данные по умолчанию
    if(orientation != Qt::Horizontal){
        return QAbstractTableModel::headerData(section,orientation,role);
    }

    switch (role) {
    case Qt::DisplayRole:
        switch (section) {
        case 0: return tr("Дата");
        case 1: return tr("Язык прогр");
        case 2: return tr("Курс");
        case 3: return tr("Урок");
        case 4: return tr("Информация");
        default: return QVariant();
        }
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignBaseline | Qt::AlignHCenter);
    case Qt::ForegroundRole:
    {
        QFont F;
        F.setBold(true);
        return F;
    }
    default: return QVariant();
    }
}

Qt::ItemFlags LessonModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant LessonModel::dataDisplay(const QModelIndex &index) const
{
    Data *item = items->at(index.row());
    switch (index.column()) {
    case 0: return item->lessonDate().isValid() ? item->lessonDate().toString("dd.MM.yyyy") : "";
    case 1: return item->lanName();
    case 2: return item->courseName();
    case 3: return item->lessonName();
    case 4: return item->lessonInfo();
    default: return QVariant();
    }
}

QVariant LessonModel::dataTextAlignment(const QModelIndex &) const
{
    //    int Result = Qt::AlignBaseline;
        int Result = Qt::AlignVCenter;
    //    Result = index.column() == 1 ? Qt::AlignLeft : Qt::AlignHCenter;
        return Result;
}

Data *LessonModel::getItem(const QModelIndex &index) const
{
    int id = index.row();
    if(id < 0 || id >= items->size()) return 0;
    return items->at(id);
}

QVariant LessonModel::dataToolTip(const QModelIndex &I) const
{
//    Data *item = items->at(index.row());
    Data *item = getData(I);
    if(!item) return QVariant();

    switch (I.column()) {
    case 0:
    {
        if(!item->lessonDate().isValid()) return QVariant();
        return tr("%1").arg(item->lessonDate().toString("dd.MM.yyyy"));
    }
    case 3: return item->lessonInfo();
    default: return QVariant();
    }
}

QVariant LessonModel::dataBackground(const QModelIndex &I) const
{
    Data *item = getData(I);
    QColor result;

    if(item->lanName() == "C"){
        result = QColor(240, 255, 255);
    } else if(item->lanName() == "C++"){
        result = QColor(144, 238, 144);
    } else if(item->lanName() == "Java"){
        result = QColor(32, 178, 170);
    } else if(item->lanName() == "JavaScript"){
        result = QColor(255, 255, 0);
    } else if(item->lanName() == "C#"){
        result = QColor(189, 183, 107);
    } else if(item->lanName() == "Python"){
        result = QColor(255, 222, 173);
    } else if(item->lanName() == "PHP"){
        result = QColor(240, 255, 255);
    } else {
        result = QColor("white");
    }

    result.setAlphaF(0.2);
    return result;
}

//Устанавливаем в модель данные по результатам поиска
void LessonModel::setItems()
{
    this->items->clear();
    for(int i = 0; i < s_items->count(); i++)
    {
        this->items->append(s_items->at(i));
    }
}


void LessonModel::search(QDate date, bool _date, int flag, QString lan, bool _lan,
                         QString pattern, bool _info)
{
    s_items->clear();
    Data *item = new Data();

    //Ищем по 1) Дата 2) Язык 3) Инфо
    if(_date && _lan && _info){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy").right(4) ==
                        date.toString("dd.MM.yyyy").right(4)){
                    if(item->lanName() == lan){
                        if(checkInfo(pattern, item->view())){
                            s_items->append(item);
                        }
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->lanName() == lan){
                        if(checkInfo(pattern, item->view())){
                            s_items->append(item);
                        }
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->lanName() == lan){
                        if(checkInfo(pattern, item->view())){
                            s_items->append(item);
                        }
                    }
                }
            }
        }//Ищем 1) Язык
    } else if(!_date && _lan && !_info){
        for(int i = 0; i < items->count(); i++){
            item = items->at(i);
            if(item->lanName() == lan){
                s_items->append(item);
            }
        }//Ищем 1) Дата 2) Язык
    } else if(_date && _lan && !_info){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy").right(4) ==
                        date.toString("dd.MM.yyyy").right(4)){
                    if(item->lanName() == lan){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(item->lanName() == lan){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(item->lanName() == lan){
                        s_items->append(item);
                    }
                }
            }
        }//Ищем 1) Дата 2) Инфо
    } else if(_date && !_lan && _info){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy").right(4) ==
                        date.toString("dd.MM.yyyy").right(4)){
                    if(checkInfo(pattern, item->view())){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    if(checkInfo(pattern, item->view())){
                        s_items->append(item);
                    }
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    if(checkInfo(pattern, item->view())){
                        s_items->append(item);
                    }
                }
            }
        }//Ищем 1)Язык 2) Инфо
    } else if(!_date && _lan && _info){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(item->lanName() == lan){
                if(checkInfo(pattern, item->view())){
                    s_items->append(item);
                }
            }
        } //Ищем Инфо
    } else if(!_date && !_lan && _info){
        for(int i = 0; i < items->count(); i++){
            Data *item = items->at(i);
            if(checkInfo(pattern, item->view())){
                s_items->append(item);
            }
        } //Ищем Дата
    } else if(_date && !_lan && !_info){
        if(flag == 1){
            for(int i = 0; i < items->count(); i++){
                Data *item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy").right(4) ==
                        date.toString("dd.MM.yyyy").right(4)){
                    s_items->append(item);
                }
            }
        } else if(flag == 2){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy").right(7) == date.toString("dd.MM.yyyy").right(7)){
                    s_items->append(item);
                }
            }
        } else if(flag == 3){
            for(int i = 0; i < items->count(); i++){
                item = items->at(i);
                if(item->lessonDate().toString("dd.MM.yyyy") == date.toString("dd.MM.yyyy")){
                    s_items->append(item);
                }
            }
        }//Ищем
    } else {
        qDebug() << "Недостаточно параметров";
    }


    if(s_items->count() == 0){
        items->clear();
        return;
    }

    beginResetModel();
    setItems(s_items);
    endResetModel();
}

