SOURCES=utestanalitza_main.cpp ../analitza/analitza.cpp
OBJECTS=utestanalitza_main.o analitza.o
TARGET=utestanalitza_main
COMPILER=gcc -c  
LINKER=gcc -o
GTEST_DIR=../gtest
CFLAGS=-I$(GTEST_DIR)  -I ../analitza/
LINKFLAGS=-lstdc++ -L $(GTEST_DIR)/libs -lgtest -lgtest_main -lpthread 
DYNAMIC_LIBRARY_PATH_MAC=DYLD_LIBRARY_PATH
DYNAMIC_LIBRARY_PATH_LINUX=LD_LIBRARY_PATH
DYNAMIC_LIBRARY_PATH=$(DYNAMIC_LIBRARY_PATH_MAC)

all: $(TARGET)

coverage:
	COMPILER_COVERAGEFLAGS="-fprofile-arcs  -ftest-coverage" LINKER_COVERAGEFLAGS=" -lgcov -coverage" make

$(TARGET): OBJECTS
	$(LINKER) $(TARGET) $(LINKFLAGS)  $(LINKER_COVERAGEFLAGS) $(OBJECTS)

OBJECTS: $(SOURCES)
	$(COMPILER) $(CFLAGS) $(COMPILER_COVERAGEFLAGS)  $(SOURCES)

run:
	$(DYNAMIC_LIBRARY_PATH)=$(GTEST_DIR)/libs/ ./utestanalitza_main	
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f *.gcov 
	rm -f *.gcda	
	rm -f *.gcno

