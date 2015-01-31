
CPPFILES+=compara_main.cpp CTrace.cpp
CPPFILES+=compara.cpp gslGraph.cpp
CPPOPTIONS = -I/Users/oscarraigcolon/Downloads/gsl-1.16/   -I. 
CPPOPTIONS+=  -I. -I ../gsl_include/ -I/opt/local/include
LINKOPTIONS= -L. -lgsl   -lboost_program_options  
LINKOPTIONS+=-lgslcblas
CPP_HOME_OSCAR_OPTIONS=-stdlib=libstdc++

PROG_NAME= compara
${PROG_NAME}: $(CPPFILES)
	g++  $(CPP_HOME_OSCAR_OPTIONS)  $(CPPFILES) $(LINKOPTIONS) $(CPPOPTIONS) -o ${PROG_NAME}

clean:
	rm -f *.log
	rm -f ${PROG_NAME} 
	rm -f *.o
