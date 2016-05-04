TARGET = twodgame
HEADERS += glwidget.h
SOURCES += glwidget.cpp

macx {
	QMAKE_MAC_SDK = macosx10.11
}

QT += opengl
CONFIG -= app_bundle
CONFIG += console
CONFIG += c++11
INCLUDEPATH += ../engine
LIBS += -L../engine -lengine
INCLUDEPATH += ../../lib
LIBS += -L../../lib -llib

RESOURCES += \
    shaders.qrc
