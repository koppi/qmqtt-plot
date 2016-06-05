TEMPLATE = app

TARGET = qmqtt-plot

DEFINES += QMQTT_EXAMPLE

INCLUDEPATH += libqmqtt

SOURCES = main.cpp \
    mainwindow.cpp ledindicator.cpp qcustomplot.cpp \
    \
libqmqtt/qmqtt_client.cpp     libqmqtt/qmqtt_network.cpp            libqmqtt/qmqtt_ssl_network.cpp \
libqmqtt/qmqtt_client_p.cpp   libqmqtt/qmqtt_routedmessage.cpp      libqmqtt/qmqtt_ssl_socket.cpp \
libqmqtt/qmqtt_frame.cpp      libqmqtt/qmqtt_router.cpp             libqmqtt/qmqtt_timer.cpp \
libqmqtt/qmqtt_message.cpp    libqmqtt/qmqtt_routesubscription.cpp \
libqmqtt/qmqtt_message_p.cpp  libqmqtt/qmqtt_socket.cpp \

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h ledindicator.h qcustomplot.h \
    \
libqmqtt/qmqtt_client.h    libqmqtt/qmqtt_message_p.h          libqmqtt/qmqtt_socket.h \
libqmqtt/qmqtt_client_p.h  libqmqtt/qmqtt_network.h            libqmqtt/qmqtt_socketinterface.h \
libqmqtt/qmqtt_frame.h     libqmqtt/qmqtt_networkinterface.h   libqmqtt/qmqtt_ssl_network.h \
libqmqtt/qmqtt_global.h    libqmqtt/qmqtt_routedmessage.h      libqmqtt/qmqtt_ssl_socket.h \
libqmqtt/qmqtt.h           libqmqtt/qmqtt_router.h             libqmqtt/qmqtt_timer.h \
libqmqtt/qmqtt_message.h   libqmqtt/qmqtt_routesubscription.h  libqmqtt/qmqtt_timerinterface.h \

win32 {

} else {
  MOC_DIR = .moc
  #OBJECTS_DIR = .obj
  UI_DIR = .ui
  RCC_DIR = .rcc

  CONFIG      += link_pkgconfig

  qmqtt-plot-binary.path = /usr/bin
  qmqtt-plot-binary.files = qmqtt-plot
  qmqtt-plot-deskop.path = /usr/share/applications
  qmqtt-plot-deskop.files = qmqtt-plot.desktop
  qmqtt-plot-icons.path = /usr/share/icons/hicolor/scalable/apps
  qmqtt-plot-icons.files = qmqtt-plot.svg

  INSTALLS    += qmqtt-plot-binary qmqtt-plot-deskop qmqtt-plot-icons

  RESOURCES   += res.qrc
}

win32 {
  DEFINES += BUILDTIME=\\\"HH:MM\\\"
  DEFINES += BUILDDATE=\\\"Y-m-d\\\"
} else {
  DEFINES += BUILDTIME=\\\"$$system(date '+%H:%M')\\\"
  DEFINES += BUILDDATE=\\\"$$system(date '+%Y-%m-%d')\\\"
}

QMAKE_CXXFLAGS_RELEASE += -O2
QMAKE_CXXFLAGS_DEBUG   += -O0

CONFIG *= debug_and_release
CONFIG *= qt
CONFIG += warn_on
CONFIG += thread
CONFIG += c++11

QT += core network gui printsupport widgets

ICON   = qmqtt-plot.svg

OTHER_FILES += README.md qmqtt-plot.svg qmqtt-plot.desktop

DIRS_DC = object_script.* .ui .moc .rcc .obj *.pro.user $$TARGET

unix:QMAKE_DISTCLEAN  += -r $$DIRS_DC
win32:QMAKE_DISTCLEAN += /s /f /q $$DIRS_DC && rd /s /q $$DIRS_DC
