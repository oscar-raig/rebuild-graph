GRAPH_IO_DIR=../../../graph_io/src/
GRAPH_GSL_GRAPH_DIR=../../../graph_gslgraph/src/
CTRACE_DIR=../../../../../libcpp/
GSL_MATRIX_DIR=../../../../gsl-1.16/

GSL_IO=../../../gsl_utillities/gsl_io/src/



SOURCES=app_gsl_io_example.cpp 
SOURCES+=$(GSL_IO)gslIO.cpp
SOURCES+=$(GRAPH_IO_DIR)/readPythonGraphFile.cpp
SOURCES+=$(CTRACE_DIR)/CTrace.cpp
SOURCES+=$(GRAPH_GSL_GRAPH_DIR)/gslGraph.cpp


INCLUDES=$(GRAPH_GSL_GRAPH_DIR)gslGraph.h
INCLUDES+=$(GRAPH_IO_DIR)readPythonGraphFile.h
INCLUDES+=$(GSL_IO)gslIO.cpp


OBJECTS= gslGraph.o
OBJECTS+=readPythonGraphFile.o CTrace.o app_gsl_io_example.o gslIO.o


TARGET=app_gsl_io_example
COMPILER=g++ -c  
LINKER=g++ -o

BOOST_DIR=../../../../boost/



CFLAGS= -I$(GSL_MATRIX_DIR)/include  -I ../src/
CFLAGS+=-I$(CTRACE_DIR)
CFLAGS+=-I$(GRAPH_IO_DIR)
CFLAGS+=-I$(GRAPH_GSL_GRAPH_DIR)
CFLAGS+=-stdlib=libstdc++
 
LINKFLAGS+=-stdlib=libstdc++ -lgsl -L$(GSL_MATRIX_DIR)/lib 

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

run: $(TARGET)
	./$(TARGET)	 ../../../utils/example_graphs/circ-inipy.txt
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f *.gcov 
	rm -f *.gcda	
	rm -f *.gcno

