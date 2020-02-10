QT += core gui

TARGET = Neb
TEMPLATE = lib
DEFINES += QT_DEPRECATED_WARNINGS
DESTDIR = $$shell_path($$_PRO_FILE_PWD_)/../lib
CONFIG += c++11
CONFIG += staticlib

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    nebparser.h \
    node.h \
    nodetype.h \
    scanner.h \
    tokens.h \
    tokentype.h

SOURCES += \
        nebparser.cpp \
        node.cpp \
        scanner.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
