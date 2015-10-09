GSL_IO=../../../gsl_utillities/gsl_io/src/
GRAPH_GSL_GRAPH_DIR=../../../graph_gslgraph/src/
GSL_MATRIX_DIR=../../../../gsl-1.16/
CTRACE_DIR=../../../../../libcpp/


SOURCES=gslIO_test.cpp $(GSL_IO)/gslIO.cpp
OBJECTS=gslIO_test.o gslIO.o
TARGET=gslIO_test
COMPILER=gcc -c  
LINKER=gcc -o
GTEST_DIR=../../../graph_analyze/gtest
CFLAGS=-I$(GTEST_DIR)   -I $(GRAPH_GSL_GRAPH_DIR) -I $(GSL_MATRIX_DIR)/include -I $(CTRACE_DIR)
CFLAGS+=-I $(GSL_IO)
LINKFLAGS= -L $(GTEST_DIR)/libs -lgsl -L$(GSL_MATRIX_DIR)/lib  -lgtest -lgtest_main -lpthread 
DYNAMIC_LIBRARY_PATH_MAC=DYLD_LIBRARY_PATH
DYNAMIC_LIBRARY_PATH_LINUX=LD_LIBRARY_PATH
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
	$(DYNAMIC_LIBRARY_PATH)=$(GTEST_DIR)/libs/ ./$(TARGET)
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f *.gcov 
	rm -f *.gcda	
	rm -f *.gcno

