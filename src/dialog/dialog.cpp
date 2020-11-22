#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

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

void Dialog::setEdit(bool flag)
{
    this->edit = flag;

    if(edit){
        setWindowTitle("Добавление/Вставка/Переименование");
        ui->label->setText("Язык программирования/Фреймворк");
    } else {
        setWindowTitle("Установление пути для скачивания базы");
        ui->label->setText("Путь для скачивания базы");
    }
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
