#-------------------------------------------------
#
# Project created by QtCreator 2015-10-06T15:22:26
#
#-------------------------------------------------

#Some Note

QT       += core gui network xml testlib\
       multimedia multimediawidgets

QT       += serialport

#QT       += core dbus
#CONFIG   -= app_bundle*/

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cameleon
TEMPLATE = app



QMAKE_LIBDIR += C:/Qt/Projects/Cameleon_QT_BY/Cameleon/SDL/SDL-1.2.15/lib/x86

Release:QMAKE_LIBDIR += C:/Qt/Projects/Cameleon/OgreSDK/lib/release



# Edit below for custom library location

SOURCES += main.cpp\
        mainwindow.cpp \
    Dialogs/dgAudio.cpp \
    Dialogs/dgLight.cpp \
    Dialogs/dgLightSetting.cpp \
    Joystick/v_joystick_adapter.cpp \
    Thread/ControllerMsgSendThread.cpp \
    Thread/ControllerMsgRcvThread.cpp \
    Thread/SendingTimer.cpp \
    Thread/ThreadPrototype.cpp \
    Utils/ControllerState.cpp \
    Utils/FuncUtils.cpp \
    Utils/ShowMessage.cpp \
    Thread/videothread.cpp \
    Utils/RobotState.cpp \
    OgreView/OgreView.cpp \
    OgreView/modelloader.cpp \
    Dialogs/dgCameraMode.cpp \
    Thread/TabletBatteryLevelRequest.cpp \
    Dialogs/CameraSelection.cpp \
    Dialogs/dgArm.cpp \
    Dialogs/dgVLC.cpp \
    Utils/PingModel.cpp

HEADERS  += mainwindow.h \
    Dialogs/dgAudio.h \
    Dialogs/dgLight.h \
    Dialogs/dgLightSetting.h \
    Dialogs/dgVideo.h \
    Joystick/v_joystick_adapter.h \
    Thread/ControllerMsgSendThread.h \
    Thread/ControllerMsgRcvThread.h \
    Thread/SendingTimer.h \
    Thread/ThreadPrototype.h \
    Utils/ConstInfo.h \
    Utils/ControllerState.h \
    Utils/FuncUtils.h \
    Utils/ShowMessage.h \
    Utils/RobotState.h \
    Thread/videothread.h \
    OgreView/OgreView.h \
    OgreView/modelloader.h \
    libvlc-qt/include/SharedExportCore.h \
    libvlc-qt/include/SharedExportWidgets.h \
    libvlc-qt/include/VideoDelegate.h \
    Dialogs/dgCameraMode.h \
    Utils/ModeTypes.h \
    Thread/TabletBatteryLevelRequest.h \
    Dialogs/CameraSelection.h \
    Dialogs/dgArm.h \
    Dialogs/dgVLC.h \
    Utils/PingModel.h

FORMS    += mainwindow.ui \
    Dialogs/dgAudio.ui \
    Dialogs/dglight.ui \
    Dialogs/dgLightSetting.ui \
    Dialogs/dgCameraMode.ui \
    Dialogs/dgCameraSelection.ui \
    Dialogs/dgArm.ui \
    Dialogs/dgVLC.ui

RESOURCES += \
    resources.qrc

win32: LIBS += -L$$PWD/../../../libvlc/lib/ -llibVLCQtCore.dll

INCLUDEPATH += $$PWD/../../../libvlc/include
DEPENDPATH += $$PWD/../../../libvlc/include

win32: LIBS += -L$$PWD/../../../libvlc/lib/ -llibVLCQtWidgets.dll

INCLUDEPATH += $$PWD/../../../libvlc/include
DEPENDPATH += $$PWD/../../../libvlc/include

win32: LIBS += -L$$PWD/SDL/SDL-1.2.15/lib/x86/ -lSDL

INCLUDEPATH += $$PWD/SDL/SDL-1.2.15/lib/x86
DEPENDPATH += $$PWD/SDL/SDL-1.2.15/lib/x86

win32: LIBS += -L$$PWD/SDL/SDL-1.2.15/lib/x86/ -lSDLmain

INCLUDEPATH += $$PWD/SDL/SDL-1.2.15/lib/x86
DEPENDPATH += $$PWD/SDL/SDL-1.2.15/lib/x86


INCLUDEPATH += $$PWD/SDL/SDL-1.2.15/include
DEPENDPATH += $$PWD/SDL/SDL-1.2.15/include


win32: LIBS += -L$$PWD/OgreSDK/lib/release/ -llibOgreMain.dll

INCLUDEPATH += $$PWD/OgreSDK/lib/release
DEPENDPATH += $$PWD/OgreSDK/lib/release

INCLUDEPATH += $$PWD/OgreSDK/include
DEPENDPATH += $$PWD/OgreSDK/include
