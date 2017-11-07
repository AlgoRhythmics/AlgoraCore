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

message("pri file being processed: $$PWD")

HEADERS += \ 
    $$PWD/finddipathalgorithm.h \
    $$PWD/breadthfirstsearch.h \
    $$PWD/basic_algorithms.h \
    $$PWD/tarjansccalgorithm.h \
    $$PWD/topsortalgorithm.h \
    $$PWD/depthfirstsearch.h \
    $$PWD/biconnectedcomponentsalgorithm.h \
    $$PWD/accessibilityalgorithm.h

SOURCES += \     
    $$PWD/finddipathalgorithm.cpp \
    $$PWD/breadthfirstsearch.cpp \
    $$PWD/tarjansccalgorithm.cpp \
    $$PWD/topsortalgorithm.cpp \
    $$PWD/basic_algorithms.cpp \
    $$PWD/depthfirstsearch.cpp \
    $$PWD/biconnectedcomponentsalgorithm.cpp \
    $$PWD/accessibilityalgorithm.cpp
