#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Добавление/Вставка/Переименование");

    this->name = "";
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getName()
{
    return this->name;
}

void Dialog::setName(QString name)
{
    ui->edtName->setText(name);
}

void Dialog::on_btnCancel_clicked()
{
    name = ui->edtName->text();
    close();
}

void Dialog::on_btnSave_clicked()
{
    name = ui->edtName->text();

    if(name == ""){
        QMessageBox mb;
        mb.setText("Пустая строка!");
        mb.exec();
        return;
    }

    close();
}

void Dialog::closeEvent(QCloseEvent *event)
{
    name = ui->edtName->text();
}
