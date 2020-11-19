#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QWidget>

#include "model/lessonmodel.h"
#include "data/storage.h"


namespace Ui {
class SearchWindow;
}

class SearchWindow : public QWidget
{
    Q_OBJECT

    Storage *s;
    LessonModel *M;
    QString link;//приходит сигналом из модели для отображения

public:
    SearchWindow(Storage *s, QWidget *parent = 0);
    ~SearchWindow();

    void createUI();
    bool check_index(int index);

signals:
    void shutdown_m();
    void sendIndextoModel(QModelIndex index);
    void sendPattern(QString);//для подстветки

private slots:
    void shutdown();
    void searchClear();
    void refreshList();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void acceptLink(QString link);//принятие ссылки из модели
    void on_tableView_clicked(const QModelIndex &index);
    void on_btnSearch_clicked();
    void on_edtPattern_textChanged(const QString &str);

private:
    Ui::SearchWindow *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // SEARCHWINDOW_H
