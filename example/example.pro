QT += core gui widgets

TARGET = NebExample
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += ../include

CONFIG += c++11

HEADERS += \
    mainwindow.h

SOURCES += \
    main.cpp \
    mainwindow.cpp

FORMS += mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS += resources/Neb.ico

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../lib/debug/ -lNeb
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/debug/Neb.lib
    else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/debug/libNeb.a
}else{
    LIBS += -L$$PWD/../lib/release/ -lNeb
    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/release/Neb.lib
    else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/release/libNeb.a
}

