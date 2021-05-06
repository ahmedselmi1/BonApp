QT       += core gui sql multimedia multimediawidgets printsupport charts serialport




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
    QCustomPlot.cpp \
    QrCode.cpp \
    QrCodeGeneratorDemo.cpp \
    QrCodeGeneratorWorker.cpp \
    arduino.cpp \
    category.cpp \
    clients.cpp \
    commandec.cpp \
    connection.cpp \
    coupons.cpp \
    database.cpp \
    dateformatdelegate.cpp \
    employee.cpp \
    gateauxc.cpp \
    main.cpp \
    mainwindow.cpp \
    materiel.cpp \
    notifications.cpp \
    produit.cpp \
    qrcodedisplayer.cpp \
    qrwidget.cpp \
    queue.cpp \
    recette.cpp \
    role.cpp \
    secdialog.cpp \
    shifts.cpp

HEADERS += \
    QCustomPlot.h \
    QrCode.hpp \
    arduino.h \
    category.h \
    clients.h \
    commandec.h \
    connection.h \
    coupons.h \
    database.h \
    dateformatdelegate.h \
    employee.h \
    gateauxc.h \
    mainwindow.h \
    materiel.h \
    notifications.h \
    produit.h \
    qrcodedisplayer.h \
    qrwidget.hpp \
    queue.h \
    recette.h \
    role.h \
    secdialog.h \
    shifts.h

FORMS += \
    mainwindow.ui \
    mainwindowIyed.ui \
    qrcodedisplayer.ui \
    secdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc
