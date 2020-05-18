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


# copies the given files to the destination directory
defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # replace slashes in source path for Windows
        win32:file ~= s,/,\\,g

        QMAKE_PRE_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    export(QMAKE_PRE_LINK)
}

copyToDestDir($$PWD/"neb_node.h", $$PWD/../include)
copyToDestDir($$PWD/"neb_nodetype.h", $$PWD/../include)
copyToDestDir($$PWD/"neb_parser.h", $$PWD/../include)
copyToDestDir($$PWD/"neb_token.h", $$PWD/../include)
copyToDestDir($$PWD/"neb_tokentype.h", $$PWD/../include)
