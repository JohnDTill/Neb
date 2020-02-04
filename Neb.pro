QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Neb
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    mainwindow.h \
    node.h \
    nodetype.h \
    parser.h \
    scanner.h \
    tokens.h \
    tokentype.h

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        node.cpp \
        parser.cpp \
        scanner.cpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS += Neb.ico
