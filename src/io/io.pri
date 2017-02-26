message("pri file being processed: $$PWD")

HEADERS += \  
    $$PWD/streamdigraphreader.h \
    $$PWD/adjacencyliststringwriter.h \
    $$PWD/streamdigraphwriter.h \
    $$PWD/adjacencyliststringformat.h \
    $$PWD/adjacencyliststringreader.h \
    $$PWD/sparsesixgraphrw.h \
    $$PWD/sparsesixformat.h \
    $$PWD/adjacencymatrixrw.h \
    $$PWD/linearvertexsequencetikzwriter.h

SOURCES += \     
    $$PWD/adjacencyliststringwriter.cpp \
    $$PWD/adjacencyliststringreader.cpp \
    $$PWD/sparsesixgraphrw.cpp \
    $$PWD/sparsesixformat.cpp \
    $$PWD/adjacencymatrixrw.cpp \
    $$PWD/linearvertexsequencetikzwriter.cpp
