#CPPFILES+=AbstractFactoryPatternRebuildGraph.cpp
CPPFILES+=CSettingsSumulation.cpp
CPPFILES+=CTrace.cpp
CPPFILES+=StrategyPatternAlgorithm.cpp
#CPPFILES+=TestRebuildGraph.cpp
#CPPFILES+=analitza.cpp
CPPFILES+=graphIndicatorBetweennessCentrality.cpp
CPPFILES+=gslGraph.cpp
CPPFILES+=rebuildgraph.cpp
CPPFILES+=main.cpp

#CPPOPTIONS = -I/Users/oscarraigcolon/Downloads/gsl-1.16/   -I. 
CPPOPTIONS= -g -I../ -I. -I ../gsl_include/ -I/opt/local/include
LINKOPTIONS= -L. -lgsl   -lboost_program_options  
LINKOPTIONS+=-lgslcblas
#CPP_HOME_OSCAR_OPTIONS=-stdlib=libstdc++

rebuild_graph: $(CPPFILES)
	g++  $(CPP_HOME_OSCAR_OPTIONS)  $(CPPFILES) $(LINKOPTIONS) $(CPPOPTIONS) -o rebuild_graph

clean:
	rm -f *.log
	rm -f rebuild_graph 
	rm -f *.o
