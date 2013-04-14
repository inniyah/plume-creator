
#-------------------------------------------------
#
# Project created by QtCreator 2011-07-25T11:13:12
#
#-------------------------------------------------
QT += core gui xml

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
#to compile with QT5, replace <QtGui> with <QtWidgets>

TEMPLATE = app

VERSION = 0.58.4
DEFINES += VERSIONSTR=\\\"$${VERSION}\\\"

#DESTDIR = bin
#MOC_DIR = build
#OBJECTS_DIR = build
#RCC_DIR = build
#UI_DIR = build
INCLUDEPATH += .\
src
DEPENDPATH +=  .

unix: !macx {
TARGET = plume-creator
} else {
TARGET = Plume-Creator
}


# dossier de zlib.h
INCLUDEPATH += ./externals/zlib

LIBS += -L external/zlib
win32: LIBS += -lzdll
!win32: LIBS += -lz

# dossier des headers de quazip
INCLUDEPATH +=  ./externals/quazip



SOURCES += src/main.cpp\
src/mainwindow.cpp \
src/newprojectwizard.cpp \
src/prjmanager.cpp \
src/maintree.cpp \
src/statsbox.cpp \
src/itembox.cpp \
src/digitalclock.cpp \
src/notezone.cpp \
src/texttab.cpp \
src/textzone.cpp \
src/timer.cpp \
src/wordcount.cpp \
src/exporter.cpp \
src/wordcountthread.cpp \
src/orientationbutton.cpp \
src/menubar.cpp \
#src/outliner/outline.cpp \
#src/outliner/outlineitem.cpp \
src/outliner/outlinerbase.cpp \
src/outliner/outlinerspreadsheet.cpp \
src/outliner/outlinerabstractmodel.cpp \
src/outliner/outlineritemdelegate.cpp \
src/outliner/outlinertreeitem.cpp \
src/settingsdialog.cpp \
src/editmenu.cpp \
src/textstyles.cpp \
src/findreplace.cpp \
src/fullscreen/fulltextzone.cpp \
src/fullscreen/fullscreeneditor.cpp \
src/slimupdater.cpp \
src/outliner/outlinerspreadsheetheader.cpp \
src/outliner/outlinerspreadsheetheadersection.cpp \
src/wordgoalprogressbar.cpp \
src/fileupdater.cpp \
src/changestests.cpp \
src/zipper.cpp \
src/hub.cpp \
    src/fileutils.cpp \
    src/attend/attendbase.cpp \
    src/attend/attendabstractmodel.cpp \
    src/attend/attendtreeitem.cpp \
    src/attend/attendsheettreeproxymodel.cpp \
    src/attend/attendglobaltreeproxymodel.cpp \
    src/attend/attendmanager.cpp \
    src/attend/attendmanagertreeproxymodel.cpp \
    src/attend/attendsettings.cpp \
    src/maintextdocument.cpp \
    src/wordcountengine.cpp


HEADERS += src/mainwindow.h \
src/newprojectwizard.h \
src/prjmanager.h \
src/maintree.h \
src/statsbox.h \
src/itembox.h \
src/digitalclock.h \
src/notezone.h \
src/texttab.h \
src/textzone.h \
src/timer.h \
src/wordcount.h \
src/exporter.h \
src/wordcountthread.h \
src/orientationbutton.h \
src/menubar.h \
#src/outliner/outline.h \
#src/outliner/outlineitem.h \
src/outliner/outlinerbase.h \
src/outliner/outlinerspreadsheet.h \
src/outliner/outlinerabstractmodel.h \
src/outliner/outlineritemdelegate.h \
src/outliner/outlinertreeitem.h \
src/settingsdialog.h \
src/editmenu.h \
src/textstyles.h \
src/findreplace.h \
src/fullscreen/fulltextzone.h \
src/fullscreen/fullscreeneditor.h \
src/slimupdater.h \
src/outliner/outlinerspreadsheetheader.h \
src/outliner/outlinerspreadsheetheadersection.h \
src/wordgoalprogressbar.h \
src/fileupdater.h \
src/zipper.h \
src/changestests.h \
src/hub.h \
    src/fileutils.h \
    src/attend/attendbase.h \
    src/attend/attendabstractmodel.h \
    src/attend/attendtreeitem.h \
    src/attend/attendsheettreeproxymodel.h \
    src/attend/attendglobaltreeproxymodel.h \
    src/attend/attendmanager.h \
    src/attend/attendmanagertreeproxymodel.h \
    src/attend/attendsettings.h \
    src/maintextdocument.h \
    src/wordcountengine.h

CODECFORTR = UTF-8

TRANSLATIONS = translations/plume-creator_fr_FR.ts \
# translations/plume-creator_ro_RO.ts \
translations/plume-creator_it_IT.ts \
translations/plume-creator_de_DE.ts

include(./externals/qtsingleapplication/src/qtsingleapplication.pri)
include(./externals/quazip/quazip.pro)

FORMS += \
src/settingsdialog.ui \
src/editmenu.ui \
src/findreplace.ui \
src/fullscreen/fullscreeneditor.ui \
src/slimupdater.ui \
src/mainwindow.ui \
src/wordgoalprogressbar.ui \
    src/attend/attendbase.ui \
    src/attend/attendmanager.ui \
    src/attend/attendsettings.ui

RESOURCES += \
src/pics.qrc \
translations/langs.qrc \
readme.qrc


macx {
# ICON = resources/mac/plume-creator.icns
}



win32 {
RC_FILE = resources/windows/icon.rc
}


macx {
ICONS.files = resources/images/icons/oxygen/hicolor
ICONS.path = Contents/Resources/icons
QMAKE_BUNDLE_DATA += ICONS
}


unix: !macx {
isEmpty(PREFIX) {
PREFIX = /usr
}
isEmpty(BINDIR) {
BINDIR = $$PREFIX/bin
}
isEmpty(DATADIR) {
DATADIR = $$PREFIX/share
}
DEFINES += DATADIR=\\\"$${DATADIR}/plume-creator\\\"
target.path = $$BINDIR
icon.files = resources/images/icons/hicolor/*
icon.path = $$DATADIR/icons/hicolor
pixmap.files = resources/unix/plume-creator.png
pixmap.path = $$DATADIR/pixmaps
desktop.files = resources/unix/plume-creator.desktop
desktop.path = $$DATADIR/applications/
mime.files = resources/unix/plume-creator.xml
mime.path = $$DATADIR/mime/packages/
mimeInk.files += resources/unix/x-plume.desktop resources/unix/x-plumebackup.desktop
mimeInk.path = $$DATADIR/mimeInk/application/
docs.files += README COPYING License INSTALL
docs.path = $$DATADIR/plume-creator/
#useless for now :
qm.files = translations/*.qm
qm.path = $$DATADIR/plume-creator/translations
# sounds.files = resources/sounds/*
# sounds.path = $$DATADIR/plume-creator/sounds
# symbols.files = resources/symbols/symbols.dat
# symbols.path = $$DATADIR/plume-creator
INSTALLS += target icon pixmap desktop mime mimeInk docs qm
}
