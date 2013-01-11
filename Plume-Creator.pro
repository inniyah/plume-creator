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
    exporter.cpp \
    attendbox.cpp \
    updater.cpp \
    wordcountthread.cpp \
    orientationbutton.cpp \
    menubar.cpp \
    outliner/outline.cpp \
    outliner/outlineitem.cpp \
    outliner/outlinerbase.cpp \
    outliner/outlinerspreadsheet.cpp \
    outliner/outlinerabstractmodel.cpp \
    outliner/outlineritemdelegate.cpp \
    outliner/outlinertreeitem.cpp \
    settingsdialog.cpp \
    editmenu.cpp \
    textstyles.cpp \
    findreplace.cpp \
    fullscreen/fulltextzone.cpp \
    fullscreen/fullscreeneditor.cpp \
    slimupdater.cpp

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
    exporter.h \
    attendbox.h \
    updater.h \
    wordcountthread.h \
    orientationbutton.h \
    menubar.h \
    outliner/outline.h \
    outliner/outlineitem.h \
    outliner/outlinerbase.h \
    outliner/outlinerspreadsheet.h \
    outliner/outlinerabstractmodel.h \
    outliner/outlineritemdelegate.h \
    outliner/outlinertreeitem.h \
    settingsdialog.h \
    editmenu.h \
    textstyles.h \
    findreplace.h \
    fullscreen/fulltextzone.h \
    fullscreen/fullscreeneditor.h \
    slimupdater.h

RESOURCES += \
    pics.qrc \
    langs.qrc \
    readme.qrc

CODECFORTR = UTF-8

TRANSLATIONS = plume-creator_fr_FR.ts \
#    plume-creator_ro_RO.ts \
    plume-creator_it_IT.ts \
    plume-creator_de_DE.ts

include(./externals/qtsingleapplication/src/qtsingleapplication.pri)

FORMS += \
    settingsdialog.ui \
    editmenu.ui \
    findreplace.ui \
    fullscreeneditor.ui \
    slimupdater.ui





