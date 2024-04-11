QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cpsapplication.cpp \
    cpsbutton.cpp \
    cpshistorywindow.cpp \
    cpslabel.cpp \
    cpsmainwindow.cpp \
    cpstextfield.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    cpsapplication.h \
    cpsbutton.h \
    cpshistorywindow.h \
    cpslabel.h \
    cpsmainwindow.h \
    cpstextfield.h \
    cpswindowsapitools.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CMakeLists.txt \
    CMakeLists.txt.user \
    CPSMonitoringSystemGUI.pro.user \
    Ut.ico \
    icon.rc
