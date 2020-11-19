#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

    QString name;

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    QString getName();
    void setName(QString name);

private slots:
    void on_btnCancel_clicked();
    void on_btnSave_clicked();

private:
    Ui::Dialog *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // DIALOG_H
