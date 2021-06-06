QT       += core gui\
            charts \
            multimedia\
            serialport\
            printsupport\
            bluetooth\

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS\


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#LIBS += -L$$_PRO_FILE_PWD_/  -lhidapi


SOURCES += \
    appthreadhjw.cpp \
    callback.cpp \
    enterwindow.cpp \
    fileprotocolparsehjw.cpp \
    genericcallback.cpp \
    jwblue.cpp \
    jwserialthread.cpp \
    licensehjw.cpp \
    main.cpp \
    maintable.cpp \
    mainwindow.cpp \
    plothjw.cpp \
    xyseriesiodevice.cpp\
    jwserial.cpp \
    jwplot/jwplot.cpp\
    qcustomplot.cpp\




HEADERS += \
    appthreadhjw.h \
    callback.h \
    enterwindow.h \
    fileprotocolparsehjw.h \
    genericcallback.h \
    jwblue.h \
    jwserialthread.h \
    licensehjw.h \
    maintable.h \
    mainwindow.h\
    hidapi.h \
    plothjw.h \
    xyseriesiodevice.h\
    jwserial.h \
    jwplot/jwplot.h\
    qcustomplot.h\




FORMS += \
    enterwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    BP.qrc


DEFINES += QT_DEPRECATED_WARNINGS

win32: LIBS += -lSetupAPI -luser32
