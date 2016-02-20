message("pri file being processed: $$PWD")

HEADERS += \
    $$PWD/vertexvisitor.h \
    $$PWD/arcvisitor.h \
    $$PWD/collectverticesvisitor.h \
    $$PWD/collectarcsvisitor.h \
    $$PWD/printgraphvisitor.h \
    $$PWD/graphvisitor.h \
    $$PWD/grimreaper.h

SOURCES += \
    $$PWD/collectverticesvisitor.cpp \
    $$PWD/collectarcsvisitor.cpp \
    $$PWD/printgraphvisitor.cpp \
    $$PWD/graphvisitor.cpp

