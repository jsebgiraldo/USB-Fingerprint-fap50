QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    IMD/FAP50/fap50reader.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    IMD/FAP50/InterfaceImdFap50Method.h \
    IMD/FAP50/fap50reader.h \
    InterfaceImdFap50Method.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Nfiq2Api.dll \
    lib_imd_fap50_method.dll \
    lib_imd_sensor_api.dll \
    lib_imd_test_method.dll \
    lib_nbis.dll \
    nist_plain_tir-ink.txt \
    nist_plain_tir-ink.yaml \
    opencv_world480.dll

win32: LIBS += $$PWD/Nfiq2Api.dll
win32: LIBS += $$PWD/lib_imd_sensor_api.dll
win32: LIBS += $$PWD/lib_imd_test_method.dll
win32: LIBS += $$PWD/lib_nbis.dll
win32: LIBS += $$PWD/lib_imd_fap50_method.dll
win32: LIBS += $$PWD/opencv_world480.dll
