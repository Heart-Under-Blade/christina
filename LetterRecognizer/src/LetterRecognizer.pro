#-------------------------------------------------
#
# Project created by QtCreator 2014-07-25T14:00:44
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LetterRecognizer
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    WordReader.cpp \
    Sign.cpp

HEADERS  += widget.h \
    WordReader.h \
    Sign.h

FORMS    += widget.ui

OTHER_FILES += \
    task.txt

RESOURCES += \
    res.qrc
