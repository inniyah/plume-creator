
#-------------------------------------------------
#
# Project created by QtCreator 2011-07-25T11:13:12
#
#-------------------------------------------------
lessThan(QT_VERSION, 4.8.3) {
        error("Plume Creator requires Qt 4.8.3 or greater")
}


QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport multimedia


TEMPLATE = app

VERSION = 0.66.3
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

CONFIG(beta_release) {
TARGET = $$join(TARGET,,,_beta)
}

# dossier de zlib.h
INCLUDEPATH += ./externals/zlib

LIBS += -L external/zlib
win32: LIBS += -lzdll
!win32: LIBS += -lz

# dossier des headers de quazip
INCLUDEPATH +=  ./externals/quazip

# hunspell


#unix: !macx {

#LIBS += -lhunspell \
#-lhunspell-1.3

#}
#else {
include(./externals/hunspell/hunspell.pro)
#}






macx {

        lessThan(QT_MAJOR_VERSION, 5) {
                HEADERS += src/rtf/clipboard_mac.h
                SOURCES += src/rtf/clipboard_mac.cpp
        }

} else:win32 {

        lessThan(QT_MAJOR_VERSION, 5) {
                HEADERS += src/rtf/clipboard_windows.h
                SOURCES += src/rtf/clipboard_windows.cpp
        }
}


SOURCES += src/main.cpp\
src/mainwindow.cpp \
src/maintree.cpp \
src/statsbox.cpp \
src/itembox.cpp \
src/digitalclock.cpp \
src/notezone.cpp \
src/texttab.cpp \
src/textzone.cpp \
src/timer.cpp \
src/exporter/exporter.cpp \
src/orientationbutton.cpp \
src/menubar.cpp \
#src/outliner/outline.cpp \
#src/outliner/outlineitem.cpp \
src/outliner/outlinerbase.cpp \
src/outliner/outlinerspreadsheet.cpp \
src/settingsdialog.cpp \
src/editmenu.cpp \
src/textstyles.cpp \
src/findReplace/findreplace.cpp \
src/fullscreen/fulltextzone.cpp \
src/fullscreen/fullscreeneditor.cpp \
src/slimupdater.cpp \
src/outliner/outlinerspreadsheetheader.cpp \
src/outliner/outlinerspreadsheetheadersection.cpp \
src/wordgoalprogressbar.cpp \
src/fileupdater.cpp \
src/changestests.cpp \
src/zipper/zipper.cpp \
src/hub.cpp \
    src/attend/attendbase.cpp \
    src/attend/attendabstractmodel.cpp \
    src/attend/attendtreeitem.cpp \
    src/attend/attendsheettreeproxymodel.cpp \
    src/attend/attendglobaltreeproxymodel.cpp \
    src/attend/attendmanager.cpp \
    src/attend/attendmanagertreeproxymodel.cpp \
    src/attend/attendsettings.cpp \
    src/maintextdocument.cpp \
    src/wordcountengine.cpp \
    src/wordcountenginethread.cpp \
    src/common/utils.cpp \
    src/outliner/outlineritemnotedelegate.cpp \
    src/outliner/outlineritempovdelegate.cpp \
    src/slimfindreplace.cpp \
    src/texthighlighter.cpp \
    src/startcenter.cpp \
#src/sounds.cpp
    src/zipper/zipchecker.cpp \
    src/mainTree/maintreeabstractmodel.cpp \
    src/mainTree/maintreeitem.cpp \
    src/workbench.cpp \
    src/dockedTree/dockedtree.cpp \
    src/dockedTree/dockedtreeproxy.cpp \
    src/outliner/outlinerspreadsheetproxy.cpp \
    src/mainTree/maintreecontextmenu.cpp \
    src/dockedTree/dockedtrashtree.cpp \
    src/dockedTree/dockedtrashtreeproxy.cpp \
    src/outliner/outlineritemstatusdelegate.cpp \
    src/common/trees/checkabletreeproxy.cpp \
    src/common/trees/checkabletree.cpp \
    src/sizehandle.cpp \
    src/spellchecker.cpp \
    src/importuserdictdialog.cpp \
    src/attend/attendmanagertree.cpp \
    src/themes.cpp \
    src/theme.cpp \
    src/newProjectWizard/newprojectwizard.cpp \
    src/newProjectWizard/selectpage.cpp \
    src/newProjectWizard/structurepage.cpp \
    src/newProjectWizard/intropage.cpp \
    src/edittoolbar.cpp \
    src/overlay.cpp \
    src/updatechecker.cpp \
    src/rtf/writer.cpp \
    src/rtf/tokenizer.cpp \
    src/rtf/reader.cpp \
    src/minimap/minimap.cpp \
    src/overview/overviewbase.cpp \
    src/overview/textcell.cpp

HEADERS += src/mainwindow.h \
src/maintree.h \
src/statsbox.h \
src/itembox.h \
src/digitalclock.h \
src/notezone.h \
src/texttab.h \
src/textzone.h \
src/timer.h \
src/exporter/exporter.h \
src/orientationbutton.h \
src/menubar.h \
#src/outliner/outline.h \
#src/outliner/outlineitem.h \
src/outliner/outlinerbase.h \
src/outliner/outlinerspreadsheet.h \
src/settingsdialog.h \
src/editmenu.h \
src/textstyles.h \
src/findReplace/findreplace.h \
src/fullscreen/fulltextzone.h \
src/fullscreen/fullscreeneditor.h \
src/slimupdater.h \
src/outliner/outlinerspreadsheetheader.h \
src/outliner/outlinerspreadsheetheadersection.h \
src/wordgoalprogressbar.h \
src/fileupdater.h \
src/zipper/zipper.h \
src/changestests.h \
src/hub.h \
    src/attend/attendbase.h \
    src/attend/attendabstractmodel.h \
    src/attend/attendtreeitem.h \
    src/attend/attendsheettreeproxymodel.h \
    src/attend/attendglobaltreeproxymodel.h \
    src/attend/attendmanager.h \
    src/attend/attendmanagertreeproxymodel.h \
    src/attend/attendsettings.h \
    src/maintextdocument.h \
    src/wordcountengine.h \
    src/wordcountenginethread.h \
    src/common/utils.h \
    src/outliner/outlineritemnotedelegate.h \
    src/outliner/outlineritempovdelegate.h \
    src/slimfindreplace.h \
    src/texthighlighter.h \
    src/startcenter.h \
#src/sounds.h
    src/zipper/zipchecker.h \
    src/mainTree/maintreeabstractmodel.h \
    src/mainTree/maintreeitem.h \
    src/workbench.h \
    src/dockedTree/dockedtree.h \
    src/dockedTree/dockedtreeproxy.h \
    src/outliner/outlinerspreadsheetproxy.h \
    src/mainTree/maintreecontextmenu.h \
    src/dockedTree/dockedtrashtree.h \
    src/dockedTree/dockedtrashtreeproxy.h \
    src/outliner/outlineritemstatusdelegate.h \
    src/common/trees/checkabletreeproxy.h \
    src/common/trees/checkabletree.h \
    src/sizehandle.h \
    src/spellchecker.h \
    src/importuserdictdialog.h \
    src/attend/attendmanagertree.h \
    src/themes.h \
    src/theme.h \
    src/newProjectWizard/newprojectwizard.h \
    src/newProjectWizard/selectpage.h \
    src/newProjectWizard/structurepage.h \
    src/newProjectWizard/intropage.h \
    src/edittoolbar.h \
    src/overlay.h \
    src/updatechecker.h \
    src/rtf/writer.h \
    src/rtf/tokenizer.h \
    src/rtf/reader.h \
    src/minimap/minimap.h \
    src/overview/overviewbase.h \
    src/overview/textcell.h

CODECFORTR = UTF-8

TRANSLATIONS = translations/plume-creator_fr_FR.ts \
translations/plume-creator_it_IT.ts \
translations/plume-creator_de_DE.ts \
translations/plume-creator_sp_SP.ts \
translations/plume-creator_ru_RU.ts \
translations/plume-creator_pt_BR.ts

include(./externals/qtsingleapplication/src/qtsingleapplication.pri)
include(./externals/quazip/quazip.pro)

FORMS += \
src/settingsdialog.ui \
src/editmenu.ui \
src/findReplace/findreplace.ui \
src/fullscreen/fullscreeneditor.ui \
src/slimupdater.ui \
src/mainwindow.ui \
src/wordgoalprogressbar.ui \
    src/attend/attendbase.ui \
    src/attend/attendmanager.ui \
    src/attend/attendsettings.ui \
    src/slimfindreplace.ui \
    src/exporter/exporter.ui \
    src/startcenter.ui \
    src/texttab.ui \
    src/dockedTree/dockedtreebase.ui \
    src/importuserdictdialog.ui \
    src/newProjectWizard/selectpage.ui \
    src/newProjectWizard/structurepage.ui \
    src/newProjectWizard/intropage.ui \
    src/edittoolbar.ui \
    src/notedock.ui \
    src/minimap/minimap.ui \
    src/overview/overviewbase.ui \
    src/overview/textcell.ui

RESOURCES += \
translations/langs.qrc \
readme.qrc \
    src/pics.qrc \
    src/sounds.qrc




win32 {
RC_FILE = resources/windows/icon.rc
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
pixmap.files += resources/unix/pixmaps/plume-creator.png \
	      resources/unix/pixmaps/plume-creator-backup.png
pixmap.path = $$DATADIR/pixmaps
desktop.files = resources/unix/applications/plume-creator.desktop
desktop.path = $$DATADIR/applications/
mime.files = resources/unix/mime/packages/plume-creator.xml
mime.path = $$DATADIR/mime/packages/
mimeInk.files += resources/unix/mimeInk/application/x-plume.desktop \
	      resources/unix/mimeInk/application/x-plume-backup.desktop
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
dicts.files = resources/dicts/*
dicts.path = $$DATADIR/plume-creator/dicts
themes.files = resources/themes/*
themes.path = $$DATADIR/plume-creator/themes



INSTALLS += target icon pixmap desktop mime mimeInk docs qm dicts themes
}


macx {
ICON = resources/mac/plume-creator.icns

icons.files = resources/images/icons
icons.path = Contents/Resources/
dicts.files = resources/dicts
dicts.path = Contents/Resources/
themes.files = resources/themes
themes.path = Contents/Resources/



QMAKE_BUNDLE_DATA += icons dicts themes
QMAKE_INFO_PLIST = resources/mac/Info.plist

#QMAKE_INFO_PLIST_OUT = $${TARGET}.app/Contents/Info.plist
#PRE_TARGETDEPS +=	$${TARGET}.app/Contents/Info.plist

#QMAKE_POST_LINK += ;/usr/libexec/PlistBuddy -c \"Set :CFBundleShortVersionString $${VERSION}\" $${OUT_PWD}/$${TARGET}.app/Contents/Info.plist

#Info_plist.target = Info.plist
#Info_plist.depends = Info.plist.template $${TARGET}.app/Contents/Info.plist
#Info_plist.commands = @$(DEL_FILE) $${TARGET}.app/Contents/Info.plist$$escape_expand(\n\t) \
#@$(SED) -e “s,@EXECUTABLE@,$$TARGET,g” -e “s,@VERSION@,$$MODULE_VERSION,g” -e “s,@TYPEINFO@,$$HiView_SIGNATURE,g” -e “s,@ICON@,$$basename(ICON),g” Info.plist.template > $${TARGET}.app/Contents/Info.plist
#QMAKE_EXTRA_TARGETS += Info_plist
#PRE_TARGETDEPS += $$Info_plist.target
#PkgInfo.target = PkgInfo
#PkgInfo.depends = $${TARGET}.app/Contents/PkgInfo
#PkgInfo.commands = @$(DEL_FILE) $$PkgInfo.depends$$escape_expand(\n\t) \ @echo “APPL$$HiView_SIGNATURE” > $$PkgInfo.depends
#QMAKE_EXTRA_TARGETS += PkgInfo
#PRE_TARGETDEPS += $$PkgInfo.target



}

