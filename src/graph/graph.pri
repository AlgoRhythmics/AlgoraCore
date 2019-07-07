########################################################################
# Copyright (C) 2013 - 2018 : Kathrin Hanauer                          #
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

message("pri file being processed: $$PWD")

HEADERS += \
    $$PWD/graphartifact.h \
    $$PWD/digraph.h \
    $$PWD/vertex.h \
    $$PWD/arc.h \
    $$PWD/parallelarcsbundle.h \
    $$PWD/graph.h \
    $$PWD/subdigraph.h \
    $$PWD/superdigraph.h \
    $$PWD/graph_functional.h \
    $$PWD/multiarc.h \
    $$PWD/weightedarc.h \
    $$PWD/vertexpair.h \
    $$PWD/reversearc.h

SOURCES += \
    $$PWD/digraph.cpp \
    $$PWD/vertex.cpp \
    $$PWD/graphartifact.cpp \
    $$PWD/parallelarcsbundle.cpp \
    $$PWD/subdigraph.cpp \
    $$PWD/superdigraph.cpp \
    $$PWD/graph_functional.cpp \
    $$PWD/multiarc.cpp \
    $$PWD/weightedarc.cpp \
    $$PWD/vertexpair.cpp \
    $$PWD/graph.cpp \
    $$PWD/arc.cpp
