GRAPH_ANALYZE_DIR=../src/
SOURCES= $(GRAPH_ANALYZE_DIR)/graph_analyze.cpp  graph_analyze_main.cpp
OBJECTS=graph_analyze.o graph_analyze_main.o
INCLUDES=$(GRAPH_ANALYZE_DIR)graph_analyze.hpp

CC=g++  -c
LINKER=g++ -o

CFLAGS=-I$(GRAPH_ANALYZE_DIR)

TARGET=graph_analyze_main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINKER) $(TARGET) $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"	

$(OBJECTS): $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) $(SOURCES)

clean:
	rm -f $(TARGET) $(OBJECTS)
	@echo "Cleanup complete!"
