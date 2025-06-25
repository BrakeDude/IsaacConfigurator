QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/about.cpp \
    src/conifg.cpp \
    src/directoryFuncs.cpp \
    src/iniconfig.cpp \
    src/isaaclogreader.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mods.cpp \
    src/translations.cpp

HEADERS += \
    headers/about.h \
    headers/customerrorhandler.h \
    headers/isaaclogreader.h \
    headers/mainwindow.h \
    headers/qtablewidgetcheckbox.h

FORMS += \
    ui/about.ui \
    ui/mainwindow.ui

TRANSLATIONS += \
    translations/imc_ru_RU.ts \
    translations/imc_es_ES.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
        icons/icons.qrc

RC_ICONS = icons/wrench.ico

TARGET = IsaacConfigurator
VERSION = 1.4.0

contains(QMAKE_HOST.os, "win32"): QMAKE_LFLAGS += -static
else: QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
