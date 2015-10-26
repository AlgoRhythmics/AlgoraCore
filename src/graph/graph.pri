message("pri file being processed: $$PWD")

HEADERS += \
    $$PWD/graphartifact.h \
    $$PWD/graph.h \
    $$PWD/vertex.h \
    $$PWD/arc.h \
    $$PWD/incidencelistgraph.h \
    $$PWD/parallelarcsbundle.h

SOURCES += \
    $$PWD/graph.cpp \
    $$PWD/vertex.cpp \
    $$PWD/arc.cpp \
    $$PWD/graphartifact.cpp \
    $$PWD/incidencelistgraph.cpp \
    $$PWD/parallelarcsbundle.cpp
