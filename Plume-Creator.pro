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
    fulltextzone.cpp \
    exporter.cpp \
    outlineitem.cpp \
    outline.cpp \
    attendbox.cpp \
    updater.cpp \
    wordcountthread.cpp \
    outlinerspreadsheet.cpp \
    outlinerbase.cpp \
    orientationbutton.cpp \
    menubar.cpp \
    outlinerabstractmodel.cpp \
    outlineritemdelegate.cpp \
    outlinertreeitem.cpp \
    settingsdialog.cpp \
#    editmenu_.cpp
    editmenu.cpp \
    textstyles.cpp \
    findreplace.cpp \
    fullscreeneditor.cpp

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
    fulltextzone.h \
    exporter.h \
    outlineitem.h \
    outline.h \
    attendbox.h \
    updater.h \
    wordcountthread.h \
    outlinerspreadsheet.h \
    outlinerbase.h \
    orientationbutton.h \
    menubar.h \
    outlinerabstractmodel.h \
    outlineritemdelegate.h \
    outlinertreeitem.h \
    settingsdialog.h \
#    editmenu_.h
    editmenu.h \
    textstyles.h \
    findreplace.h \
    fullscreeneditor.h

RESOURCES += \
    pics.qrc \
    langs.qrc \
    readme.qrc

CODECFORTR = UTF-8

TRANSLATIONS = plume-creator_fr_FR.ts \
#    plume-creator_ro_RO.ts

include(./externals/qtsingleapplication/src/qtsingleapplication.pri)

FORMS += \
    settingsdialog.ui \
    editmenu.ui \
    findreplace.ui \
    fullscreeneditor.ui





