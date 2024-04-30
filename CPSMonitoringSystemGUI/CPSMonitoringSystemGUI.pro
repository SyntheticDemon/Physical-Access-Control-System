# Basic project setup
QT       += core gui network

# Add 'widgets' for Qt 5 and above
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
                                  QT += websockets


# Enable C++17 support
CONFIG += c++17
LIBS += -ldwmapi
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# Source files
SOURCES += \
    bidirectionalconnection.cpp \
    cpsapplication.cpp \
    cpsbutton.cpp \
    cpshistorywindow.cpp \
    cpslabel.cpp \
    cpsmainwindow.cpp \
    cpstextfield.cpp \
    httprequest.cpp \
    httpresponse.cpp \
    main.cpp \
    mainwindow.cpp

# Header files
HEADERS += \
    bidirectionalconnection.h \
    cpsapplication.h \
    cpsbutton.h \
    cpshistorywindow.h \
    cpslabel.h \
    cpsmainwindow.h \
    cpstextfield.h \
    cpswindowsapitools.h \
    httprequest.h \
    httpresponse.h \
    mainwindow.h

# UI files
FORMS += \
    mainwindow.ui

# Default rules for deployment.
# Adjust target paths based on platform
win32 {
    target.path = $$[QT_INSTALL_EXAMPLES]/websockets/CPSMonitoringSystemGUI
    INSTALLS += target
}
else {
    target.path = /opt/$$TARGET/bin
    INSTALLS += target
}

# Additional files to distribute
DISTFILES += \
    CMakeLists.txt \
    CMakeLists.txt.user \
    CMakeLists.txt.user.efd4768 \
    CPSMonitoringSystemGUI.pro.user \
    CPSMonitoringSystemGUI.pro.user.efd4768 \
    Ut.ico \
    icon.rc
