TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    openFoamSystemFiles.cpp \
    smokeTransportRun.cpp \
    readConfigFile.cpp \
    smokeTransportOptions.cpp \
    configOption.cpp \
    usefulFunctions.cpp

HEADERS += \
    openFoamSystemFiles.h \
    smokeTransportRun.h \
    readConfigFile.h \
    smokeTransportOptions.h \
    configOption.h \
    useCmd.h \
    usefulFunctions.h

