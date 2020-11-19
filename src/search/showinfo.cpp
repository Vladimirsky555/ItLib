#include "showinfo.h"
#include "ui_showinfo.h"

ShowInfo::ShowInfo(QString courseInfo, QString lessonInfo,
                   QString lessonName, QString courseName,
                   QString pattern, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowInfo)
{
    ui->setupUi(this);
    setWindowTitle("Информация");
    QString str;
    str = "<b>Курс:</b> ";
    str += courseName;
    str += "<br>";
    str += "<b>Информация:</b>";
    str += courseInfo;
    ui->courseInfo->setHtml(str);

    //Подсветка поисковой фразы для курса
    highlighter_course = new RegexpHighlighter(this);
    highlighter_course->setDocument(ui->courseInfo->document());
    highlighter_course->setPattern(pattern);
    highlighter_course->rehighlight();

    QString lesson;
    lesson = "<b>Урок:</b> ";
    lesson += lessonName;
    lesson += "<br>";
    lesson += "<b>Информация:</b>";
    lesson += lessonInfo;
    ui->lessonInfo->setHtml(lesson);

    //Подсветка поисковой фразы для урока
    highlighter_lesson = new RegexpHighlighter(this);
    highlighter_lesson->setDocument(ui->lessonInfo->document());
    highlighter_lesson->setPattern(pattern);
    highlighter_lesson->rehighlight();
}

ShowInfo::~ShowInfo()
{
    delete ui;
}

void ShowInfo::shutdown()
{
    close();
}
