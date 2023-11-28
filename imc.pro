QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    conifg.cpp \
    directoryFuncs.cpp \
    iniconfig.cpp \
    main.cpp \
    mainwindow.cpp \
    mods.cpp \
    translations.cpp

HEADERS += \
    about.h \
    mainwindow.h

FORMS += \
    about.ui \
    mainwindow.ui

TRANSLATIONS += \
    imc_ru_RU.ts \
    imc_es_ES.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
        icons.qrc

RC_ICONS = wrench.ico

TARGET = IsaacConfigurator

QMAKE_LFLAGS += -static
