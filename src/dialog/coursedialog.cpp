#include "coursedialog.h"
#include "ui_coursedialog.h"

#include <QMessageBox>

CourseDialog::CourseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CourseDialog)
{
    ui->setupUi(this);
    setWindowTitle("Добавить курс");

    this->_name = "";
    this->_link = "";
    this->_courseInfo = "";
}

CourseDialog::~CourseDialog()
{
    delete ui;
}

QString CourseDialog::getName()
{
   return this->_name;
}

void CourseDialog::setName(QString name)
{
   ui->edtName->setText(name);
}

QString CourseDialog::getLink()
{
   return this->_link;
}

void CourseDialog::setLink(QString link)
{
   ui->edtLink->setText(link);
}

QString CourseDialog::getInfo()
{
   return this->_courseInfo;
}

void CourseDialog::setInfo(QString courseInfo)
{
   ui->edtInfo->setText(courseInfo);
}

void CourseDialog::on_btnCancel_clicked()
{
    _name = ui->edtName->text();
    _link = ui->edtLink->text();
    _courseInfo = ui->edtInfo->toHtml();

    close();
}

void CourseDialog::on_btnAdd_clicked()
{
    this->_name = ui->edtName->text();
    this->_link = ui->edtLink->text();
    this->_courseInfo = ui->edtInfo->toHtml();

    if(_name == "" || _link == "")
    {
        QMessageBox::information(this, "Сообщение!", "Пустая строка!");
        return;
    }

    close();
}

void CourseDialog::closeEvent(QCloseEvent *event)
{
    _name = ui->edtName->text();
    _link = ui->edtLink->text();
    _courseInfo = ui->edtInfo->toHtml();
}
