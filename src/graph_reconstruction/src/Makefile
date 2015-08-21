GSL_GRAPH_DIR=../../graph_gslgraph/src/
GRAPH_IO_DIR=../../graph_io/src/
GRAPH_INDICATOR_DIR=../../graph_indicators/src/

SOURCES=main.cpp ProgramOptions.cpp SettingsSimulation.cpp
SOURCES+=rebuildgraph.cpp
SOURCES+=$(GRAPH_INDICATOR_DIR)graphIndicator.cpp 
SOURCES+=$(GRAPH_INDICATOR_DIR)graphIndicatorBetweennessCentrality.cpp
SOURCES+=$(GRAPH_INDICATOR_DIR)graphIndicatorCommunicabilityBetweennessCentrality.cpp 
SOURCES+=$(GRAPH_INDICATOR_DIR)graphIndicatorCommunicabilityCentralityUsingMatrixExponential.cpp
SOURCES+=../../graph_gslgraph/src/gslGraph.cpp 
SOURCES+=$(GRAPH_IO_DIR)readPythonGraphFile.cpp
SOURCES+=StrategyPatternAlgorithm.cpp
SOURCES+=../../../../libcpp/CTrace.cpp


OBJECTS=main.o   ProgramOptions.o   SettingsSimulation.o  
OBJECTS+=graphIndicator.o   
OBJECTS+=graphIndicatorBetweennessCentrality.o  
OBJECTS+=graphIndicatorCommunicabilityBetweennessCentrality.o   
OBJECTS+=graphIndicatorCommunicabilityCentralityUsingMatrixExponential.o  
OBJECTS+=rebuildgraph.o  
OBJECTS+=gslGraph.o   
OBJECTS+=readPythonGraphFile.o  
OBJECTS+=StrategyPatternAlgorithm.o  
OBJECTS+=CTrace.o

INCLUDES+=$(GRAPH_INDICATOR_DIR)graphIndicator.h 
INCLUDES+=$(GRAPH_INDICATOR_DIR)graphIndicatorBetweennessCentrality.h  
INCLUDES+=$(GRAPH_INDICATOR_DIR)graphIndicatorCommunicabilityBetweennessCentrality.h
INCLUDES+=$(GRAPH_INDICATOR_DIR)graphIndicatorCommunicabilityCentralityUsingMatrixExponential.h

INCLUDES+=rebuildgraph.h
INCLUDES+=FactoryMethodGraphIndicator.h FactoryMethodStrategyPatternAlgorithm.h 
INCLUDES+=ProgramOptions.h SettingsSimulation.h 

INCLUDES+=../../graph_gslgraph/src/gslGraph.h 
INCLUDES+=$(GRAPH_IO_DIR)readPythonGraphFile.h
INCLUDES+= StrategyPatternAlgorithm.h  
INCLUDES+=../../../../libcpp/CTrace.hpp

CC=g++  -c
LINKER=g++ -o

CFLAGS=-I ../../../gsl-1.16/include -I ../../../boost/include
CFLAGS+=-I../../../../libcpp/ -stdlib=libstdc++
CFLAGS+=-I$(GSL_GRAPH_DIR)
CFLAGS+=-I$(GRAPH_IO_DIR)
CFLAGS+=-I$(GRAPH_INDICATOR_DIR)

BOOST_DIR=../../../boost/

LFLAGS=-stdlib=libstdc++ -lgslcblas  -lgsl -L../../../gsl-1.16/lib -L../../../boost/lib -lboost_program_options

DYNAMIC_LIBRARY_PATH_MAC=DYLD_LIBRARY_PATH
DYNAMIC_LIBRARY_PATH_LINUX=LD_LIBRARY_PATH
DYNAMIC_LIBRARY_PATH=$(DYNAMIC_LIBRARY_PATH_LINUX)

OPERATING_SYSTEM=$(shell uname)
ifeq ($(OPERATING_SYSTEM),Darwin)
DYNAMIC_LIBRARY_PATH=$(DYNAMIC_LIBRARY_PATH_MAC)
endif

TARGET= rebuild_graph

all: $(TARGET)

$(TARGET): OBJECTS
	$(LINKER) $(TARGET) $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"	

OBJECTS: $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) $(SOURCES)

run:
	$(DYNAMIC_LIBRARY_PATH)=$(BOOST_DIR)/lib/ ./$(TARGET)	

clean:
	rm -f $(TARGET) $(OBJECTS)
	@echo "Cleanup complete!"
