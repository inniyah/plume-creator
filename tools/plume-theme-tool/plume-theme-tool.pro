#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T14:02:54
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = plume-theme-tool
TEMPLATE = app

VERSION = 0.1
DEFINES += VERSIONSTR=\\\"$${VERSION}\\\"

INCLUDEPATH += ../../src/common


# dossier de zlib.h
INCLUDEPATH += ../../externals/zlib

LIBS += -L external/zlib
win32: LIBS += -lzdll
!win32: LIBS += -lz

# dossier des headers de quazip
INCLUDEPATH +=  ../../externals/quazip



SOURCES += main.cpp\
             mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
