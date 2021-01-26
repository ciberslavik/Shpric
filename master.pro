QT      += core gui
QT      += serialbus
QT      += serialport

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
    Dialogs/editportionpressetdialog.cpp \
    Dialogs/editpressetdialog.cpp \
    Dialogs/editstuffingpressetdialog.cpp \
    Dialogs/editswiperpressetdialog.cpp \
    Dialogs/inputdigitdialog.cpp \
    Dialogs/inputtextdialog.cpp \
    Dialogs/iotestdialog.cpp \
    Dialogs/machinetestdialog.cpp \
    Dialogs/questiondialog.cpp \
    Dialogs/selectlogicdialog.cpp \
    Dialogs/selectpressetdialog.cpp \
    IO/PIRegulator.cpp \
    IO/iobase.cpp \
    IO/iofake.cpp \
    IO/iomodbus.cpp \
    IO/iomodbusworker.cpp \
    IO/iopin.cpp \
    can/MyMaster.c \
    can/controllerbase.cpp \
    can/fakeservo.cpp \
    can/servocontroller.cpp \
    frames/canconfigframe.cpp \
    frames/configframebase.cpp \
    frames/framebase.cpp \
    frames/machineconfigframe.cpp \
    frames/mainframe.cpp \
    frames/modbusconfigframe.cpp \
    frames/portionframe.cpp \
    frames/pressetmanagerframe.cpp \
    frames/pressetsmodel.cpp \
    frames/selectsettingframe.cpp \
    frames/serviceframe.cpp \
    frames/standbyframe.cpp \
    frames/stuffingframe.cpp \
    frames/swiperframe.cpp \
    logic/AbstractLogic.cpp \
    logic/PortionLogic.cpp \
    logic/StuffingLogic.cpp \
    logic/SwiperLogic.cpp \
    machine.cpp \
    machinecontroller.cpp \
    machineio_.cpp \
    main.cpp \
    mainwindow.cpp \
    master.cpp \
    request.cpp \
    setting/AbstractConfig.cpp \
    setting/ioserialconfig.cpp \
    setting/machineconfig.cpp \
    setting/pressetconfig.cpp \
    setting/settingstorage.cpp \
    utils/LoggingCategories.cpp \
    utils/qstorage.cpp \
    widgets/qclickablelineedit.cpp

HEADERS += \
    Dialogs/editportionpressetdialog.h \
    Dialogs/editpressetdialog.h \
    Dialogs/editstuffingpressetdialog.h \
    Dialogs/editswiperpressetdialog.h \
    Dialogs/inputdigitdialog.h \
    Dialogs/inputtextdialog.h \
    Dialogs/iotestdialog.h \
    Dialogs/machinetestdialog.h \
    Dialogs/questiondialog.h \
    Dialogs/selectlogicdialog.h \
    Dialogs/selectpressetdialog.h \
    IO/PIRegulator.h \
    IO/iobase.h \
    IO/iodevstate.h \
    IO/iofake.h \
    IO/iomodbus.h \
    IO/iomodbusworker.h \
    IO/iopin.h \
    IO/ioworkerconfig.h \
    can/MyMaster.h \
    can/controllerbase.h \
    can/fakeservo.h \
    can/servocontroller.h \
    frames/canconfigframe.h \
    frames/configframebase.h \
    frames/framebase.h \
    frames/machineconfigframe.h \
    frames/mainframe.h \
    frames/modbusconfigframe.h \
    frames/portionframe.h \
    frames/pressetmanagerframe.h \
    frames/pressetsmodel.h \
    frames/selectsettingframe.h \
    frames/serviceframe.h \
    frames/standbyframe.h \
    frames/stuffingframe.h \
    frames/swiperframe.h \
    logic/AbstractLogic.h \
    logic/PortionLogic.h \
    logic/StuffingLogic.h \
    logic/SwiperLogic.h \
    machine.h \
    machinecontroller.h \
    machineio_.h \
    mainwindow.h \
    master.h \
    request.h \
    setting/AbstractConfig.h \
    setting/ioserialconfig.h \
    setting/machineconfig.h \
    setting/pressetconfig.h \
    setting/settingstorage.h \
    utils/LoggingCategories.h \
    utils/qstorage.h \
    widgets/qclickablelineedit.h

FORMS += \
    Dialogs/editportionpressetdialog.ui \
    Dialogs/editstuffingpressetdialog.ui \
    Dialogs/editswiperpressetdialog.ui \
    Dialogs/inputdigitdialog.ui \
    Dialogs/inputtextdialog.ui \
    Dialogs/iotestdialog.ui \
    Dialogs/machinetestdialog.ui \
    Dialogs/questiondialog.ui \
    Dialogs/selectlogicdialog.ui \
    Dialogs/selectpressetdialog.ui \
    frames/canconfigframe.ui \
    frames/machineconfigframe.ui \
    frames/mainframe.ui \
    frames/modbusconfigframe.ui \
    frames/portionframe.ui \
    frames/pressetmanagerframe.ui \
    frames/selectsettingframe.ui \
    frames/serviceframe.ui \
    frames/standbyframe.ui \
    frames/stuffingframe.ui \
    frames/swiperframe.ui \
    mainwindow.ui \
    widgets/qclickablelineedit.ui

#LIBS += /home/cibers/LinuxProjects/can_test/build-CanFestival-3-de1fc3261f21-Desktop_Qt_5_9_9_GCC_64bit-Debug/libcanfestival.a
##LIBS += /home/cibers/LinuxProjects/can_test/build-CanFestival-3-de1fc3261f21-AGAVA40_ARM7_Qt_5_9_6-Debug/libcanfestival.a
#LIBS += /home/cibers/LinuxProjects/can_test/build-CanFestival-3-de1fc3261f21-AGAVA40_ARM7_Qt_5_9_6-RelWithDebInfo/libcanfestival.a
LIBS += /home/cibers/LinuxProjects/can_test/build-CanFestival-3-de1fc3261f21-AGAVA40_ARM7_Qt_5_9_6-Release/libcanfestival.a
LIBS += -ldl -lrt

INCLUDEPATH += /home/cibers/LinuxProjects/can_test/CanFestival-3-de1fc3261f21/include \
        /home/cibers/LinuxProjects/can_test/CanFestival-3-de1fc3261f21/include/unix \
        /home/cibers/LinuxProjects/can_test/CanFestival-3-de1fc3261f21/include/timers_unix \
        /mnt/_qtsdk/qt-5.9.6/sysroots/armv7ahf-neon-linux-gnueabi/usr/include/c++/7.2.1

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    can/MyMaster.od \
    images/backspace-arrow.png \
    images/cancel.png \
    images/circled-down.png \
    images/circled-up.png \
    images/delete.png \
    images/exclamation.png \
    images/gears.png \
    images/info.png \
    images/ok.png \
    images/power-button-off.png

RESOURCES += \
    resources.qrc
