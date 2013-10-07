#-------------------------------------------------
#
# Project created by QtCreator 2013-08-27T16:32:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    stdsoap2.cpp \
    soapProxy.cpp \
    soapC.cpp \
    spi.cpp \
    motro.cpp \
    temp.cpp \
    adc.cpp \
    led.cpp \
    globalvar.cpp

HEADERS  += widget.h \
    ns.nsmap \
    stdsoap2.h \
    soapStub.h \
    soapProxy.h \
    soapH.h \
    spi.h \
    motro.h \
    temp.h \
    adc.h \
    led.h \
    globalvar.h

FORMS    += widget.ui \
    spi.ui \
    motro.ui \
    temp.ui \
    adc.ui \
    led.ui

OTHER_FILES += \
    icon.rc \
    web_service.ico

RC_FILE = icon.rc
