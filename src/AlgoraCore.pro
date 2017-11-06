#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       -= core gui

TARGET = AlgoraCore
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS_DEBUG += -std=c++11 -O0
QMAKE_CXXFLAGS_RELEASE += -std=c++11

general {
  QMAKE_CXXFLAGS_RELEASE += -O2 -march=x86-64
} else {
  QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -mtune=native
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(graph/graph.pri)
include(graph.incidencelist/graph.incidencelist.pri)
include(graph.visitor/graph.visitor.pri)
include(property/property.pri)
include(pipe/pipe.pri)
include(io/io.pri)
include(algorithm/algorithm.pri)
include(algorithm.basic/algorithm.basic.pri)
