#-------------------------------------------------
#
# Project created by QtCreator 2011-07-25T11:13:12
#
#-------------------------------------------------

QT       += core gui xml

TARGET = Plume-Creator
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    newprojectwizard.cpp \
    prjmanager.cpp \
    maintree.cpp \
    statsbox.cpp \
    itembox.cpp \
    digitalclock.cpp \
    notezone.cpp \
    texttab.cpp \
    textzone.cpp \
    timer.cpp \
    wordcount.cpp \
    settingsdialog.cpp \
    fullscreeneditor.cpp \
    fulltextzone.cpp \
    exporter.cpp \
    outlineitem.cpp \
    outline.cpp \
    attendbox.cpp \
    updater.cpp \
    wordcountthread.cpp \
    findreplace.cpp \
    outlinerspreadsheet.cpp \
    outlinerbase.cpp \
    orientationbutton.cpp \
    editmenu.cpp \
    menubar.cpp

HEADERS  += mainwindow.h \
    newprojectwizard.h \
    prjmanager.h \
    maintree.h \
    statsbox.h \
    itembox.h \
    digitalclock.h \
    notezone.h \
    texttab.h \
    textzone.h \
    timer.h \
    wordcount.h \
    settingsdialog.h \
    fullscreeneditor.h \
    fulltextzone.h \
    exporter.h \
    outlineitem.h \
    outline.h \
    attendbox.h \
    updater.h \
    wordcountthread.h \
    findreplace.h \
    outlinerspreadsheet.h \
    outlinerbase.h \
    orientationbutton.h \
    editmenu.h \
    menubar.h

RESOURCES += \
    pics.qrc \
    langs.qrc

CODECFORTR = UTF-8

TRANSLATIONS = plume-creator_fr_FR.ts \
#    plume-creator_ro_RO.ts

include(./externals/qtsingleapplication/src/qtsingleapplication.pri)






