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
    $$PWD/weightedarc.h

SOURCES += \
    $$PWD/digraph.cpp \
    $$PWD/vertex.cpp \
    $$PWD/arc.cpp \
    $$PWD/graphartifact.cpp \
    $$PWD/parallelarcsbundle.cpp \
    $$PWD/subdigraph.cpp \
    $$PWD/graph.cpp \
    $$PWD/superdigraph.cpp \
    $$PWD/graph_functional.cpp \
    $$PWD/multiarc.cpp \
    $$PWD/weightedarc.cpp
