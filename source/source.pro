QT += core gui

TARGET = Neb
TEMPLATE = lib
DEFINES += QT_DEPRECATED_WARNINGS
DESTDIR = $$shell_path($$_PRO_FILE_PWD_)/../lib
CONFIG += c++11
CONFIG += staticlib

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    neb_node.h \
    neb_nodetype.h \
    neb_parser.h \
    neb_scanner.h \
    neb_token.h \
    neb_tokentype.h

SOURCES += \
    neb_node.cpp \
    neb_parser.cpp \
    neb_scanner.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
