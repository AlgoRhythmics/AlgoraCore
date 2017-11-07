########################################################################
# Copyright (C) 2013 - 2017 : Kathrin Hanauer                          #
#                                                                      #
# This file is part of Algora.                                         #
#                                                                      #
# Algora is free software: you can redistribute it and/or modify       #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Algora is distributed in the hope that it will be useful,            #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Algora.  If not, see <http://www.gnu.org/licenses/>.      #
#                                                                      #
# Contact information:                                                 #
#   http://algora.xaikal.org                                           #
########################################################################

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
