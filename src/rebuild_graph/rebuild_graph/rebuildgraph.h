//
//  rebuildgraph.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 14/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef rebuild_graph_rebuildgraph_h
#define rebuild_graph_rebuildgraph_h
#include "graphs.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_linalg.h>

#include "CTrace.hpp"
#include "GraphFactory.h"

// Simulated Annealing

#ifndef STRING_LENGTH
#define STRING_LENGTH 256
#endif

#include "StrategyPatternAlgorithm.h"
#include "StrategyPatternAlgorithm.h"
class CRebuildGraph{
	
private:
	StrategyPatternAlgorithm *SrategyPatternAlgorithm;

public:
	// Input Output File Functions
	GeneralGraph *readPythonGraphFile(char *fileName);
	GeneralGraph *GetGraphfromFile(const char *graphFileName);

	
	// Printing Graphs and Results
	static int printGslMatrix(gsl_matrix* gslMatrix,const char *format="%.3f ");
	int printGslVector(gsl_vector* gslVector);
	
	int calculateCommunicability_cent_exp(const char *argv[]);
	void generateOutputFile(const  GeneralGraph *targetGraph,const char *inputFileName,double Tk,
							double costBest,double *targetBC,
							double *bestBC, time_t timeStart, time_t timeEnd,CSettingsSimulation settingSimulation);
	
	
	static void printingCompareMatrixResults(float delta,
									  gsl_matrix *F,
									  gsl_matrix* matrixA);
	
	static float compareMatrix(gsl_matrix* A, gsl_matrix*B);
		

	
	
	// Calculating properties Grpaphs
	int calculateCommunicability(const char *argv[]);
	int calculateBeterness(const char *argv[]);
	
	
		
	// Auxiliar Calculations
	gsl_vector_complex * calculateEgeinval (gsl_matrix *target);
	gsl_vector *calculateExp(const gsl_vector_complex *eval);
	
	// Others
		

	void CompareAndGenerateResults(CSettingsSimulation settingsSimulation,
												  GeneralGraph *targetGraph,
												  GeneralGraph *bestGraph,
												  char* inputFilename,
												  time_t timeStart,
												  double Tk,
												  double *&targetBC,
												  double *&bestBC,
												  double costBest,
												  double &compareResult,
												  char *outputGraphFilename
												  );
	void regenerateGraph(CSettingsSimulation *settingsSimulation,
						double *&targetBC,
						double *&bestBC,
						int &graphOrder,
						double &compareResult)
	{
		GeneralGraph *targetGraph= NULL;
		char inputFilename[250];
		time_t timeStart;
		GeneralGraph *bestGraph = NULL;
		double costBest=0.0;
			char outputGraphFilename[STRING_LENGTH];
		double Tk=TEMPER_INITIAL_DEFAULT;
		// Default value initialization
		timeStart=time(NULL);
		if ( settingsSimulation == NULL)
			throw std::runtime_error("settingsSimulation is NULL");
		strcpy(inputFilename,settingsSimulation->inputFileName.c_str());
		strcpy(outputGraphFilename,inputFilename);
		strcat(outputGraphFilename,".res");
		targetGraph = GraphFactory::createGraph( GSL_GRAPH );
		targetGraph->readPythonGraphFile(inputFilename);
		SrategyPatternAlgorithm->regenerateGraph(settingsSimulation,targetGraph,inputFilename, targetBC, bestBC, graphOrder, compareResult,&Tk,&costBest,&bestGraph);
		CompareAndGenerateResults(*settingsSimulation,targetGraph,bestGraph,inputFilename,timeStart,Tk,								  targetBC,bestBC,costBest,compareResult,outputGraphFilename);
		
	}
	
};



#endif
