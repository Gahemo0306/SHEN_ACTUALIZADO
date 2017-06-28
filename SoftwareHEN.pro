#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24T16:13:10
#
#-------------------------------------------------

QT+= core gui

QMAKE_CXXFLAGS += -std=gnu++14

greaterThan(QT_MAJOR_VERSION,4): QT += widgets printsupport

TARGET = SoftwareHEN
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        ventanaprincipal.cpp \
    unidadesentrada.cpp \
    tabladatos.cpp \
    duvaloraction.cpp \
    analisispersonalizado.cpp \
    plots.cpp \
    summary.cpp \
    qcustomplot.cpp \
    contenido_plots.cpp \
    graficos.cpp \
    contenido_plots_area.cpp \
    contenido_plots_cc.cpp \
    contenido_plots_costos.cpp \
    plotter.cpp \
    problemtable.cpp \
    help.cpp \
    areatargeting.cpp \
    costtargeting.cpp \
    networkdesign.cpp \
    jscene.cpp \
    plotterdn.cpp

HEADERS  += ventanaprincipal.h \
    unidadesentrada.h \
    tabladatos.h \
    valoraction.h \
    duvaloraction.h \
    analisispersonalizado.h \
    plots.h \
    summary.h \
    qcustomplot.h \
    contenido_plots.h \
    graficos.h \
    contenido_plots_area.h \
    contenido_plots_cc.h \
    contenido_plots_costos.h \
    plotter.h \
    problemtable.h \
    help.h \
    areatargeting.h \
    costtargeting.h \
    networkdesign.h \
    jscene.h \
    plotterdn.h

FORMS    += ventanaprincipal.ui \
    unidadesentrada.ui \
    tabladatos.ui \
    analisispersonalizado.ui \
    plots.ui \
    summary.ui \
    contenido_plots.ui \
    contenido_plots_area.ui \
    contenido_plots_cc.ui \
    contenido_plots_costos.ui \
    plotter.ui \
    problemtable.ui \
    help.ui \
    areatargeting.ui \
    costtargeting.ui \
    networkdesign.ui \
    plotterdn.ui

RESOURCES += resources.qrc

DISTFILES += \
    help_K.html \
    Resources/arrow.png \
    Resources/intercambiador.png
