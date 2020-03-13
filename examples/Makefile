CC      := g++

TARGETS:= bfs

.PHONY: all clean

all: $(TARGETS)

clean:
	-	rm -f $(TARGETS)

% : %.cpp
	$(CC) -std=c++17 -Wall -o $@ -I../src/ -L../build/Release/ $^ -lAlgoraCore
