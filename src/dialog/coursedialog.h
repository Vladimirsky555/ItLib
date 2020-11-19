#ifndef COURSEDIALOG_H
#define COURSEDIALOG_H

#include <QDialog>

namespace Ui {
class CourseDialog;
}

class CourseDialog : public QDialog
{
    Q_OBJECT

    QString _name;// - название курса
    QString _link;// - ссылка на курс в vk
    QString _courseInfo;// - описание курса

public:
    explicit CourseDialog(QWidget *parent = 0);
    ~CourseDialog();

    QString getName();
    void setName(QString name);
    QString getLink();
    void setLink(QString link);
    QString getInfo();
    void setInfo(QString courseInfo);

private slots:
    void on_btnCancel_clicked();
    void on_btnAdd_clicked();

private:
    Ui::CourseDialog *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // COURSEDIALOG_H
