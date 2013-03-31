CONFIG += qt warn_on
DEPENDPATH += .
INCLUDEPATH += .
!win32:VERSION = 1.0.0

#DEFINES += QUAZIP_BUILD
#CONFIG(staticlib):
DEFINES += QUAZIP_STATIC

# Input
HEADERS += \
$$PWD/crypt.h \
$$PWD/ioapi.h \
$$PWD/JlCompress.h \
$$PWD/quaadler32.h \
$$PWD/quachecksum32.h \
$$PWD/quacrc32.h \
$$PWD/quagzipfile.h \
$$PWD/quaziodevice.h \
$$PWD/quazipdir.h \
$$PWD/quazipfile.h \
$$PWD/quazipfileinfo.h \
$$PWD/quazip_global.h \
$$PWD/quazip.h \
$$PWD/quazipnewinfo.h \
$$PWD/unzip.h \
$$PWD/zip.h

SOURCES +=  \
$$PWD/qioapi.cpp \
$$PWD/JlCompress.cpp \
$$PWD/quaadler32.cpp \
$$PWD/quacrc32.cpp \
$$PWD/quagzipfile.cpp \
$$PWD/quaziodevice.cpp \
$$PWD/quazipdir.cpp \
$$PWD/quazipfile.cpp \
$$PWD/quazipfileinfo.cpp \
$$PWD/quazip.cpp \
$$PWD/quazipnewinfo.cpp \
$$PWD/unzip.c \
$$PWD/zip.c

unix:!symbian {
    headers.path=$$PREFIX/include/quazip
    headers.files=$$HEADERS
    target.path=$$PREFIX/lib
    INSTALLS += headers target

	OBJECTS_DIR=.obj
	MOC_DIR=.moc
	
}

#win32 {
#    headers.path=$$PREFIX/include/quazip
#    headers.files=$$HEADERS
#    target.path=$$PREFIX/lib
#    INSTALLS += headers target
#    # workaround for qdatetime.h macro bug
#    DEFINES += NOMINMAX
#}


symbian {

    # Note, on Symbian you may run into troubles with LGPL.
    # The point is, if your application uses some version of QuaZip,
    # and a newer binary compatible version of QuaZip is released, then
    # the users of your application must be able to relink it with the
    # new QuaZip version. For example, to take advantage of some QuaZip
    # bug fixes.

    # This is probably best achieved by building QuaZip as a static
    # library and providing linkable object files of your application,
    # so users can relink it.

    CONFIG += staticlib
    CONFIG += debug_and_release

    LIBS += -lezip

    #Export headers to SDK Epoc32/include directory
    exportheaders.sources = $$HEADERS
    exportheaders.path = quazip
    for(header, exportheaders.sources) {
        BLD_INF_RULES.prj_exports += "$$header $$exportheaders.path/$$basename(header)"
    }
}
