#ifndef LESSON_H
#define LESSON_H

#include <QObject>
#include <QDate>

class Lesson : public QObject
{
    Q_OBJECT

    QDate _date;// - дата публикации в youtube
    QString _name;// - название видеоурока
    QString _link;// - ссылка на видеоурок
    QString _lessonInfo;// - информация об уроке

public:
    Lesson(QDate date, QString name, QString link,
           QString lessonInfo, QObject *parent = 0);
    Lesson(QByteArray arr, QObject *parent = nullptr);//скачиваем из файла
    QByteArray saveIt();//сохраняем в файл каждый урок

    QDate Date();
    void setDate(QDate date);

    QString Name();
    void setName(QString name);

    QString Link();
    void setLink(QString link);

    QString lessonInfo();
    void setLessonInfo(QString lessonInfo);

};

//QDataStream &operator<<(QDataStream &out, const Lesson &l);
//QDataStream &operator>>(QDataStream &in, Lesson &l);



#endif // LESSON_H
