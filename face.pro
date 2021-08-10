#-------------------------------------------------
#
# Project created by QtCreator 2021-07-06T23:07:10
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        detectThread/detect.cpp \
        main.cpp \
        mainwindow.cpp \
    datadb/facedb.cpp \
    communThread/communication.cpp \
    comFunc/comfunc.cpp \
    setPage/setparameter.cpp \
    devicescan/devicescan.cpp \
    devicescan/NetworkConf.cpp \
    comFunc/common_input.cpp \
    keyboard/qtkeyboard.cpp


HEADERS += \
        detectThread/detect.h \
        include.h \
        mainwindow.h \
        $$PWD/include/ExternalApi.h \
    datadb/facedb.h \
    include/defineExt.h \
    communThread/communication.h \
    include/lib_mqtt.h \
    comFunc/comfunc.h \
    setPage/customtabstyle.h \
    setPage/setparameter.h \
    devicescan/devicescan.h \
    include/netapi.h \
    include/xosal.h \
    devicescan/NetworkConf.h \
    comFunc/common_input.h \
    keyboard/qtkeyboard.h


FORMS += \
        mainwindow.ui \
    setPage/setparameter.ui \
    comFunc/common_input.ui \
    keyboard/qtkeyboard.ui



OPENCV_LIB += -lopencv_features2d -lopencv_flann -lopencv_imgcodecs -lopencv_imgproc -lopencv_calib3d -lopencv_core
LIBS +=-lrga  -L$$PWD/lib/ -lExternalApi  -lfacesdk_rv1109 $$OPENCV_LIB -lif_face_sdk -larctern-base
LIBS += $$PWD/lib/libpaho-mqtt3a.a $$PWD/lib/libpaho-mqtt3c.a $$PWD/lib/lib_mqtt.a
LIBS += -L$$PWD/lib/ -lnetapi  -lxosal

INCLUDEPATH += $$PWD/.
DEPENDPATH  += $$PWD/.



target.path += /oem/Face/
INSTALLS += target

RESOURCES += \
    res/pic.qrc
include("$$PWD/input2019/input2019.pri")
