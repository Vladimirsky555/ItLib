#include "coursemodel.h"

#include <QFont>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

CourseModel::CourseModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    lessons = new QList<Lesson*>;

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
    }
}

int CourseModel::Count()
{
    return lessons->count();
}

QList<Lesson *> *CourseModel::Lessons()
{
    return this->lessons;
}


void CourseModel::selectAll()
{
    beginResetModel();
    endResetModel();
}

void CourseModel::addLesson(Lesson *lesson)
{
    if(lesson != NULL){
        lessons->append(lesson);
    }
    return;
}

void CourseModel::setLessons(QList<Lesson *> *lessons)
{
    this->lessons->clear();
    for(int i = 0; i < lessons->count(); i++)
    {
        this->lessons->append(lessons->at(i));
    }
//    this->lessons = lessons;
   beginResetModel();
   endResetModel();
}

void CourseModel::setAllLessons()
{
    beginResetModel();
    endResetModel();
}

void CourseModel::clearLessons()
{
    this->lessons->clear();
}

void CourseModel::setCourseLink(QString link)
{
    this->courseLink = link;
}

void CourseModel::show_course()
{
    Lesson *lesson = getLesson(currentIndex);
    if(currentIndex.row() == -1){
        QMessageBox::information(NULL, "Информация",
                                 "Чтобы просмотреть видеозапись, "
                                 "необходимо кликнуть на неё левой кнопкой мыши!");
        return;
    }

    if(courseLink == "")return;
    else{
        QDesktopServices::openUrl(QUrl(courseLink));
    }
}

void CourseModel::show_lesson()
{
    Lesson *lesson = getLesson(currentIndex);
    if(currentIndex.row() == -1){
        QMessageBox::information(NULL, "Информация",
                                 "Чтобы просмотреть видеозапись, "
                                 "необходимо кликнуть на неё левой кнопкой мыши!");
        return;
    }

    if(lesson->Link() == "")return;
    else{
        QDesktopServices::openUrl(QUrl(lesson->Link()));
    }
}

void CourseModel::acceptIndexfromView(QModelIndex index)
{
    this->currentIndex = index;
}


int CourseModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return lessons->count();
    } else {
        return 0;
    }
}

int CourseModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return 2;
    } else {
        return 0;
    }
}

QVariant CourseModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:             return dataDisplay(index);
    case Qt::TextAlignmentRole: return dataTextAlignment(index);
    case Qt::ToolTipRole:             return dataToolTip(index);
    case Qt::BackgroundRole:    return dataBackground(index);
    default:  return QVariant();
    }
}

QVariant CourseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //Для горизонтальных заголовков оставляем данные по умолчанию
    if(orientation != Qt::Horizontal){
        return QAbstractTableModel::headerData(section,orientation,role);
    }

    switch (role) {
    case Qt::DisplayRole:
        switch (section) {
        case 0: return tr("Дата");
        case 1: return tr("Название");
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

Qt::ItemFlags CourseModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant CourseModel::dataDisplay(const QModelIndex &index) const
{
    Lesson *lesson = lessons->at(index.row());
    switch (index.column()) {
    case 0: return lesson->Date().isValid() ? lesson->Date().toString("dd.MM.yyyy") : "";
    case 1: return lesson->Name();
    default: return QVariant();
    }
}

QVariant CourseModel::dataTextAlignment(const QModelIndex &) const
{
    //    int Result = Qt::AlignBaseline;
        int Result = Qt::AlignVCenter;
    //    Result = index.column() == 1 ? Qt::AlignLeft : Qt::AlignHCenter;
        return Result;
}


Lesson *CourseModel::getLesson(const QModelIndex &index) const
{
    int id = index.row();
    if(id < 0 || id >= lessons->size()) return 0;
    return lessons->at(id);
}

QVariant CourseModel::dataToolTip(const QModelIndex &I) const
{
    Lesson *lesson = getLesson(I);
    if(!lesson) return QVariant();

    switch (I.column()) {
    case 0:
    {
        if(!lesson->Date().isValid()) return QVariant();
        return tr("%1").arg(lesson->Date().toString("dd.MM.yyyy"));
    }
    case 1: return lesson->Name();
    default: return QVariant();
    }
}

QVariant CourseModel::dataBackground(const QModelIndex &index) const
{
//    Lesson *lesson = lessons->at(index.row());
//    QColor result;

//    if(!item->isNew()) {
//        if(item->Type() == "Premium"){
//            result = QColor(240, 255, 255);
//        } else if(item->Type() == "Yes" ){
//            result = QColor(144, 238, 144);
//        } else if(item->Type() == "Exclusive" ){
//            result = QColor(32, 178, 170);
//        } else if(item->Type() == "Sugar" ){
//            result = QColor(255, 255, 0);
//        } else {
//            result = QColor("white");
//        }
//    } else {
//        result = QColor("white");
//    }

//    result.setAlphaF(0.2);
//    return result;

//QColor(189, 183, 107);
// QColor(255, 222, 173);
//QColor(240, 255, 255);
}

