GRAPH_ANALYZE_DIR=../src

SOURCES=utestanalitza_main.cpp $(GRAPH_ANALYZE_DIR)/graph_analyze.cpp
OBJECTS=utestanalitza_main.o graph_analyze.o
TARGET=utestanalitza_main
COMPILER=gcc -c  
LINKER=gcc -o
GTEST_DIR=/usr/local/lib/
CFLAGS=-I$(GTEST_DIR)  -I $(GRAPH_ANALYZE_DIR)
LINKFLAGS=-lstdc++ -L $(GTEST_DIR)/libs -lgtest -lgtest_main -lpthread 
DYNAMIC_LIBRARY_PATH_MAC=DYLD_LIBRARY_PATH
DYNAMIC_LIBRARY_PATH_LINUX=LD_LIBRARY_PATH=LD_LIBRARY_PATH:$(GTEST_DIR)
DYNAMIC_LIBRARY_PATH=$(DYNAMIC_LIBRARY_PATH_LINUX)

OPERATING_SYSTEM=$(shell uname)
ifeq ($(OPERATING_SYSTEM),Darwin)
DYNAMIC_LIBRARY_PATH=$(DYNAMIC_LIBRARY_PATH_MAC)
endif

all: $(TARGET)

coverage:
	COMPILER_COVERAGEFLAGS="-fprofile-arcs  -ftest-coverage" LINKER_COVERAGEFLAGS=" -lgcov -coverage" make

$(TARGET): $(OBJECTS)
	$(LINKER) $(TARGET) $(LINKFLAGS)  $(LINKER_COVERAGEFLAGS) $(OBJECTS)

$(OBJECTS): $(SOURCES)
	$(COMPILER) $(CFLAGS) $(COMPILER_COVERAGEFLAGS)  $(SOURCES)

test:$(TARGET)
	$(DYNAMIC_LIBRARY_PATH) ./$(TARGET)
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f *.gcov 
	rm -f *.gcda	
	rm -f *.gcno

