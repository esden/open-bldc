# qgovernor - QT based Open-BLDC PC interface tool
# Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# -------------------------------------------------
# Project created by QtCreator 2010-02-23T20:08:48
# -------------------------------------------------
QT += network \
    opengl \
    testlib
TARGET = qgovernor
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    connectdialog.cpp \
    governormaster.cpp \
    simulator.cpp \
    governorclient.cpp \
    registermodel.cpp \
    protocolmodel.cpp \
    governorsimulator.cpp \
    governorftdi.cpp \
    govconfig.cpp
HEADERS += mainwindow.h \
    connectdialog.h \
    governormaster.h \
    simulator.h \
    governorclient.h \
    registermodel.h \
    protocolmodel.h \
    governorsimulator.h \
    governorftdi.h \
    govconfig.h \
    targetwidgetfactory.h
FORMS += mainwindow.ui \
    connectdialog.ui \
    simulator.ui
macx { 
    LIBS += -L/opt/local/lib \
        -L/opt/mine/lib
    INCLUDEPATH += /opt/local/include \
        /opt/mine/include
    ICON = icons/qgovernor.icns
    FILETYPES.files = /opt/mine/lib//libgovernor.dylib
    FILETYPES.path = Contents/Frameworks
    QMAKE_BUNDLE_DATA += FILETYPES
}
INCLUDEPATH += ../utils/yamlgen/include \
    ../utils/olconf/include
LIBS += -lgovernor \
    -lftdi \
    -lusb \
    -lyaml \
    -lyamlgen \
    -lolconf
RESOURCES += qgovernor.qrc
