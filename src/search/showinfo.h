#ifndef SHOWINFO_H
#define SHOWINFO_H

#include <QWidget>

#include "helpers/regexphighlighter.h"

namespace Ui {
class ShowInfo;
}

class ShowInfo : public QWidget
{
    Q_OBJECT

    RegexpHighlighter *highlighter_lesson;
    RegexpHighlighter *highlighter_course;

public:
    explicit ShowInfo(QString courseInfo, QString lessonInfo, QString lessonName,
                      QString courseName,
                      QString pattern, QWidget *parent = 0);
    ~ShowInfo();

private slots:
    void shutdown();

private:
    Ui::ShowInfo *ui;
};

#endif // SHOWINFO_H
