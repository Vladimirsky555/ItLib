#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QDate>

class Data : public QObject
{
    Q_OBJECT

    //Переменные класса LanItem
    QString lan_name;// - название языка программирования

    //Переменные класса Course
    QString course_name;// - название курса
    QString course_link;// - ссылка на курс в vk
    QString course_info;// - описание курса

    //Переменные класса Lesson
    QDate lesson_date;// - дата публикации в youtube
    QString lesson_name;// - название видеоурока
    QString lesson_link;// - ссылка на видеоурок
    QString lesson_info;// - информация об уроке

public:
    Data(QObject *parent = 0);

    QString lanName();
    void setLanName(QString value);
    QString courseName();
    void setCourseName(QString value);
    QString courseLink();
    void setCourseLink(QString value);
    QString courseInfo();
    void setCourseInfo(QString value);
    QDate lessonDate();
    void setLessonDate(QDate value);
    QString lessonName();
    void setLessonName(QString value);
    QString lessonLink();
    void setLessonLink(QString value);
    QString lessonInfo();
    void setLessonInfo(QString value);
    QString view();
};

#endif // DATA_H
