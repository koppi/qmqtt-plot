QT += core network gui webkitwidgets printsupport widgets

CONFIG += c++11

TEMPLATE = app

TARGET = qmqtt-plot

DEFINES += QMQTT_EXAMPLE

INCLUDEPATH += libqmqtt
LIBS += -lqmqtt

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

