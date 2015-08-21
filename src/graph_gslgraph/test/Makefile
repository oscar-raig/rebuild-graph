GRAPH_IO_DIR=../../graph_io/src/
GRAPH_TRACE=../../../../libcpp/

SOURCES=graph_gslgraph_test.cpp ../src/gslGraph.cpp
SOURCES+=$(GRAPH_TRACE)/CTrace.cpp
SOURCES+=$(GRAPH_IO_DIR)readPythonGraphFile.cpp

INCLUDES=../src/gslGraph.h
INCLUDES+=$(GRAPH_IO_DIR)readPythonGraphFile.h

OBJECTS=graph_gslgraph_test.o gslGraph.o
OBJECTS+=readPythonGraphFile.o CTrace.o

TARGET=graph_gslgraph_test
COMPILER=g++ -c  
LINKER=g++ -o

BOOST_DIR=../../../boost/
GSL_MATRIX_DIR=../../../gsl-1.16/
CTRACE_DIR=../../../../libcpp/

CFLAGS=-I$(BOOST_DIR)/include -I$(GSL_MATRIX_DIR)/include  -I ../src/
CFLAGS+=-I$(CTRACE_DIR)
CFLAGS+=-I$(GRAPH_IO_DIR)
CFLAGS+=-stdlib=libstdc++

LINKFLAGS=-stdlib=libstdc++ -L $(BOOST_DIR)/lib -lboost_unit_test_framework  
LINKFLAGS+=-lgsl -L$(GSL_MATRIX_DIR)/lib 

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

$(TARGET): OBJECTS
	$(LINKER) $(TARGET) $(LINKFLAGS)  $(LINKER_COVERAGEFLAGS) $(OBJECTS)

OBJECTS: $(SOURCES)
	$(COMPILER) $(CFLAGS) $(COMPILER_COVERAGEFLAGS)  $(SOURCES)

run:
	$(DYNAMIC_LIBRARY_PATH)=$(BOOST_DIR)/lib/ ./$(TARGET)	
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f *.gcov 
	rm -f *.gcda	
	rm -f *.gcno

