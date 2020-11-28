#ifndef LESSONDIALOG_H
#define LESSONDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class LessonDialog;
}

class LessonDialog : public QDialog
{
    Q_OBJECT

    QDate _date;// - дата публикации в youtube
    QString _name;// - название видеоурока
    QString _link;// - ссылка на видеоурок
    QString _lessonInfo;// - информация об уроке

public:
    explicit LessonDialog(QWidget *parent = 0);
    ~LessonDialog();

    QDate getDate();
    void setDate(QDate date);
    QString getName();
    void setName(QString name);
    QString getLink();
    void setLink(QString link);
    QString getInfo();
    void setInfo(QString lessonInfo);

private slots:
    void on_btnCancel_clicked();
    void on_btnAdd_clicked();

    void on_btnToYouTube_clicked();

private:
    Ui::LessonDialog *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *);
};

#endif // LESSONDIALOG_H
