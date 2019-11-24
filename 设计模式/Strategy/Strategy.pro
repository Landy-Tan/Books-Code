TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    csort.cpp \
    cbubble.cpp \
    cquicksort.cpp \
    csortalgorithm.cpp \
    calgorithmfactory.cpp

HEADERS += \
    csort.h \
    cbubble.h \
    cquicksort.h \
    csortalgorithm.h \
    calgorithmfactory.h
