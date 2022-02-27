QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fileparsercsv.cpp \
    flight.cpp \
    geneticalgorithm.cpp \
    graphicpainter.cpp \
    main.cpp \
    mainwindow.cpp \
    point.cpp \
    randomflights.cpp \
    scenariodialog.cpp \
    simpleflight.cpp

HEADERS += \
    Constants.h \
    fileparsercsv.h \
    flight.h \
    geneticalgorithm.h \
    graphicpainter.h \
    mainwindow.h \
    point.h \
    randomflights.h \
    scenariodialog.h \
    simpleflight.h

FORMS += \
    mainwindow.ui \
    scenario.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    quitIcon.qrc
