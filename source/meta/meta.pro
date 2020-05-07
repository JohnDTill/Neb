QT += core

TARGET = meta
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += main.cpp

HEADERS += ../neb_tokentype.h \
           ../neb_nodetype.h \
    script_nodes.h \
    script_tokens.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += TokenTable.csv NodeTable.csv
RESOURCES += TokenTable.csv NodeTable.csv
