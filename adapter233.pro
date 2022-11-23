QT       += core gui serialport sql
QT       += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Devices/FTDIAdapter/ftdiadapterdevice.cpp \
    Devices/GDM78341/gdm78341.cpp \
    Devices/GDS71072/gds71072.cpp \
    Devices/GPD72303/gpd72303.cpp \
    Devices/PCI1727/pci1727.cpp \
    Devices/PSP405/psp405.cpp \
    Devices/SDGWaveGenerator/sdgwavegenerator.cpp \
    Devices/advantechcarddevice.cpp \
    Devices/comportdevice.cpp \
    Devices/device.cpp \
    Devices/visadevice.cpp \
    Methods/Adapters/adapter6b5429233.cpp \
    Methods/TestProcedure/testprocedure6b5429233.cpp \
    Methods/TestProcedure/testprocedure6b5429426.cpp \
    Methods/TestProcedure/testprocedure6b5429503.cpp \
    Methods/commonmethod.cpp \
    common/commondata.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Devices/FTDIAdapter/ftdiadapterdevice.h \
    Devices/GDM78341/gdm78341.h \
    Devices/GDS71072/gds71072.h \
    Devices/GPD72303/gpd72303.h \
    Devices/PCI1727/pci1727.h \
    Devices/PSP405/psp405.h \
    Devices/SDGWaveGenerator/sdgwavegenerator.h \
    Devices/SDGWaveGenerator/visa.h \
    Devices/SDGWaveGenerator/visatype.h \
    Devices/SDGWaveGenerator/vpptype.h \
    Devices/advantechcarddevice.h \
    Devices/bdaqctrl.h \
    Devices/comportdevice.h \
    Devices/device.h \
    Devices/visadevice.h \
    Methods/Adapters/adapter6b5429233.h \
    Methods/TestProcedure/testprocedure6b5429233.h \
    Methods/TestProcedure/testprocedure6b5429426.h \
    Methods/TestProcedure/testprocedure6b5429503.h \
    Methods/commonmethod.h \
    common/commondata.h \
    common/enumTypeData.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += "c:/Program Files/IVI Foundation/VISA/WinNT/Include"
DEPENDPATH += "c:/Program Files/IVI Foundation/VISA/WinNT/Include"

# Library for VISA functions
win32: LIBS += "c:/Program Files/IVI Foundation/VISA/WinNT/lib/msc/visa32.lib"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Devices/Automation.BDaq.jar
