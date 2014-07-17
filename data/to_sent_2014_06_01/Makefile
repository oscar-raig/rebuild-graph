

CPPFILES= graphs.cpp main.cpp rebuildgraph.cpp CTrace.cpp
CPPOPTIONS =-I gsl-1.16/  -I.
LINKOPTIONS=-L. -lgsl -lgslcblas -stdlib=libstdc++ -lboost_program_options

comm_exp: $(CPPFILES)
	g++ -stdlib=libstdc++ $(CPPFILES) $(LINKOPTIONS) $(CPPOPTIONS) -o comm_exp

clean:
	rm -f *.log
	rm -f comm_exp
	rm -f *.o
