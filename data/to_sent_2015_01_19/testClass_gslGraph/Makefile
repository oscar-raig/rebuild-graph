

CPPFILES= graphIndicatorCommunicabilityCentralityUsingMatrixExponential.cpp graphIndicatorBetweennessCentrality.cpp rebuildgraph.cpp mainTestClass.cpp  CTrace.cpp 
CPPFILES+= gslGraph.cpp
CPPOPTIONS =-I gsl-1.16/  -I. -I../gsl_include/
LINKOPTIONS= -L. -lgsl -lgslcblas -stdlib=libstdc++ -lboost_program_options -lboost_unit_test_framework 

TestClassgslGraph: $(CPPFILES)
	g++ -stdlib=libstdc++ $(CPPFILES) $(LINKOPTIONS) $(CPPOPTIONS) -o TestClassgslGraph

clean:
	rm -f *.log
	rm -f TestClass
	rm -f *.o
