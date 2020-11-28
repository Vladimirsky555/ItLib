#include "lessondialog.h"
#include "ui_lessondialog.h"

#include <QMessageBox>

LessonDialog::LessonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LessonDialog)
{
    ui->setupUi(this);
    setWindowTitle("Добавление урока");
    ui->dateEdit->setDate(QDate::currentDate());
}

LessonDialog::~LessonDialog()
{
    delete ui;
}

QDate LessonDialog::getDate()
{
    return this->_date;
}

void LessonDialog::setDate(QDate date)
{
    ui->dateEdit->setDate(date);
}

QString LessonDialog::getName()
{
    return this->_name;
}

void LessonDialog::setName(QString name)
{
    ui->edtName->setText(name);
}

QString LessonDialog::getLink()
{
    return this->_link;
}

void LessonDialog::setLink(QString link)
{
    ui->edtLink->setText(link);
}

QString LessonDialog::getInfo()
{
    return this->_lessonInfo;
}

void LessonDialog::setInfo(QString lessonInfo)
{
    ui->edtInfo->setText(lessonInfo);
}

void LessonDialog::on_btnCancel_clicked()
{
    _date = ui->dateEdit->date();
    _name = ui->edtName->text();
    _link = ui->edtLink->text();
    _lessonInfo = ui->edtInfo->toHtml();
    close();
}

void LessonDialog::on_btnAdd_clicked()
{
    this->_date = ui->dateEdit->date();
    this->_name = ui->edtName->text();
    this->_link = ui->edtLink->text();
    this->_lessonInfo = ui->edtInfo->toHtml();

    if(_name == "" || _link == "")
    {
        QMessageBox::information(this, "Сообщение!", "Пустая строка!");
        return;
    }

    close();
}

void LessonDialog::closeEvent(QCloseEvent *)
{
    _date = ui->dateEdit->date();
    _name = ui->edtName->text();
    _link = ui->edtLink->text();
    _lessonInfo = ui->edtInfo->toHtml();
}

void LessonDialog::on_btnToYouTube_clicked()
{
    QRegExp e("([a-zA-Z0-9-_]+){11}");
    int pos = 0;
    while ((pos = e.indexIn(ui->edtLink->text(), pos)) != -1) {
        pos += e.matchedLength();
        ui->edtLink->setText("https://youtu.be/" + e.cap(0));
    }
}
