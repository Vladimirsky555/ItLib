#-------------------------------------------------
# Project created by QtCreator 2020-11-13T22:36:51
#-------------------------------------------------

QT       += core gui  network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ItLib
TEMPLATE = app

VERSION = 1.0
QMAKE_TARGET_COMPANY = BearWin
QMAKE_TARGET_PRODUCT = IT Library
QMAKE_TARGET_DESCRIPTION = IT Library
QMAKE_TARGET_COPYRIGHT = BearWin

INCLUDEPATH += $${_PRO_FILE_PWD_}/src
win32: RC_ICONS = $$PWD/bin/images/app.ico

SOURCES += src\main.cpp\
        src\mainwindow\mainwindow.cpp \
    src\data\lesson.cpp \
    src\data\course.cpp \
    src\data\lanitem.cpp \
    src\data\storage.cpp \
    src\editor\editor.cpp \
    src\dialog\coursedialog.cpp \
    src\dialog\lessondialog.cpp \
    src\dialog\dialog.cpp \
    src\model\coursemodel.cpp \
    src\model\lessonmodel.cpp \
    src\search\searchwindow.cpp \
    src\data\data.cpp \
    src\search\showinfo.cpp \
    src\helpers\regexphighlighter.cpp \
    src\mainwindow\jsonworker.cpp

HEADERS  += src\mainwindow\mainwindow.h \
    src\data\lesson.h \
    src\data\course.h \
    src\data\lanitem.h \
    src\data\storage.h \
    src\editor\editor.h \
    src\dialog\dialog.h \
    src\dialog\coursedialog.h \
    src\dialog\lessondialog.h \
    src\model\coursemodel.h \
    src\model\lessonmodel.h \
    src\search\searchwindow.h \
    src\data\data.h \
    src\search\showinfo.h \
    src\helpers\regexphighlighter.h \
    src\mainwindow\jsonworker.h

FORMS    += src\mainwindow\mainwindow.ui \
    src\editor\editor.ui \
    src\dialog\landialog.ui \
    src\dialog\dialog.ui \
    src\dialog\coursedialog.ui \
    src\dialog\lessondialog.ui \
    src\search\searchwindow.ui \
    src\search\showinfo.ui

RESOURCES += \
    bin/img.qrc
