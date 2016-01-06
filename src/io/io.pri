message("pri file being processed: $$PWD")

HEADERS += \  
    $$PWD/streamdigraphreader.h \
    $$PWD/adjacencyliststringwriter.h \
    $$PWD/streamdigraphwriter.h \
    $$PWD/adjacencyliststringformat.h \
    $$PWD/adjacencyliststringreader.h

SOURCES += \     
    $$PWD/adjacencyliststringwriter.cpp \
    $$PWD/adjacencyliststringreader.cpp
