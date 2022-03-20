msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basetable.cpp \
    config.cpp \
    debug.cpp \
    engine.cpp \
    entrusttable.cpp \
    fundtable.cpp \
    ledlabel.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    mdapi.cpp \
    postable.cpp \
    quotetable.cpp \
    tdapi.cpp \
    trade.cpp \
    util.cpp

HEADERS += \
    api/ThostFtdcMdApi.h \
    api/ThostFtdcTraderApi.h \
    api/ThostFtdcUserApiDataType.h \
    api/ThostFtdcUserApiStruct.h \
    basetable.h \
    command.h \
    config.h \
    debug.h \
    engine.h \
    entrusttable.h \
    fundtable.h \
    global.h \
    ledlabel.h \
    login.h \
    mainwindow.h \
    mdapi.h \
    postable.h \
    quotetable.h \
    tdapi.h \
    trade.h \
    util.h

FORMS += \
    login.ui \
    mainwindow.ui \
    trade.ui

TRANSLATIONS += \
    ProgramTrading_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/api/ -lthostmduserapi_se

INCLUDEPATH += $$PWD/api
DEPENDPATH += $$PWD/api

win32: LIBS += -L$$PWD/api/ -lthosttraderapi_se

INCLUDEPATH += $$PWD/api
DEPENDPATH += $$PWD/api

RESOURCES += \
    qss.qrc
