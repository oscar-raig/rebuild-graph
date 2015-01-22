CPPFILES+=AbstractFactoryPatternRebuildGraph.cpp
CPPFILES+=CSettingsSumulation.cpp
CPPFILES+=CTrace.cpp
CPPFILES+=StrategyPatternAlgorithm.cpp
CPPFILES+=TestRebuildGraph.cpp
CPPFILES+=analitza.cpp
CPPFILES+=graphIndicatorBetweennessCentrality.cpp
CPPFILES+=gslGraph.cpp
CPPFILES+=rebuildgraph.cpp
#CPPOPTIONS = -I/Users/oscarraigcolon/Downloads/gsl-1.16/   -I. 
CPPOPTIONS=-I. -I ../gsl_include/
LINKOPTIONS= -L. -lgsl -lgslcblas -stdlib=libstdc++ -lboost_program_options -lboost_unit_test_framework 

TestClassgslGraph: $(CPPFILES)
	g++ -stdlib=libstdc++ $(CPPFILES) $(LINKOPTIONS) $(CPPOPTIONS) -o TestRebuildGraph

clean:
	rm -f *.log
	rm -f TestRebuildGraph 
	rm -f *.o
