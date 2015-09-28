#-------------------------------------------------
#
# Project created by QtCreator 2015-09-20T18:43:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = druff_kassa
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    shopstate.cpp \
    logindialog.cpp \
    sellingview.cpp \
    skladview.cpp \
    bonuscardsview.cpp \
    bonuscarddialog.cpp \
    sellerdialog.cpp \
    bonuscardchoose.cpp \
    sellersview.cpp \
    paydialog.cpp

HEADERS  += mainwindow.h \
    shopstate.h \
    logindialog.h \
    sellingview.h \
    skladview.h \
    bonuscardsview.h \
    bonuscarddialog.h \
    sellerdialog.h \
    bonuscardchoose.h \
    sellersview.h \
    paydialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    sellingview.ui \
    skladview.ui \
    bonuscardsview.ui \
    bonuscarddialog.ui \
    sellerdialog.ui \
    bonuscardchoose.ui \
    sellersview.ui \
    paydialog.ui
