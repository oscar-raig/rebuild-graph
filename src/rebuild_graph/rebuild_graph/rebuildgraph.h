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

// Simulated Annealing

#define TEMPER_INITIAL_DEFAULT 10
#define TEMPER_MIN_DEFAULT 0.0001
#define NUMBER_MAX_COMBINATIONS_DEFAULT 1000
#define K 0.9
#define TOL 0.0001
#define MAX_ITERATIONS 1000 
#define STRING_LENGTH 256

class CRebuildGraph{

public:
	// Input Output File Functions
	graph *readPythonGraphFile(char *fileName);
	graph*GetGraphfromFile(const char *argv[]);

	
	
	// Rebuilding Graph Main Functions
	int regenerateGraph(CSettingsSimulation &settingsSimulation,
						 double *&targetBC,
						 double *&bestBC,
						 int &graphOrder,
						 double &compareResult);
	void AnnealingAlgorithm(double &Tk, graph **pbestGraph,int graphOrder,
							double *bestBC,double *targetBC,
							FILE *logFile,double &costBest,
							CSettingsSimulation settingSimulation);

	// Copying And Modifying graphs
	graph*	copyGraph(graph *sourceGraph);
	void	copyGraph(graph *sourceGraph,graph *targetGraph);
	int graphToGsl(graph * source, gsl_matrix* target);
	
	
	graph *generateInitialGraph(int sourceGraphOrder,int &random_value_x,int &random_value_y,int &random_value_z);
	
	void modifyGraph(graph *sourceGraph,int &random_value_x,int &random_value_y,int &random_value_z);
	double cost(double *tarjet,double *current,int count);
	
	
	
	// Printing Graphs and Results
	int printGslMatrix(gsl_matrix* gslMatrix,const char *format="%.3f ");
	int printGslVector(gsl_vector* gslVector);
	int gslVectorToArray(gsl_vector* gslVector, double* arrayDoubles);
	int calculateCommunicability_cent_exp(const char *argv[]);
	void generateOutputFile(const  graph *targetGraph,const char *inputFileName,double Tk,
							double costBest,double *targetBC,
							double *bestBC, time_t timeStart, time_t timeEnd,CSettingsSimulation settingSimulation);
	
	
	float compareMatrix(gsl_matrix* A, gsl_matrix*B);
		

	
	
	// Calculating properties Grpaphs
	int calculateCommunicability(const char *argv[]);
	int calculateBeterness(const char *argv[]);
	
	void
	brandes_comunicability_centrality_exp(graph *targetGraph,double *myCExp);
		
	// Auxiliar Calculations
	gsl_vector_complex * calculateEgeinval (gsl_matrix *target);
	gsl_vector *calculateExp(const gsl_vector_complex *eval);
	
	// Others
	gsl_vector *getDiagonalFromGslMatrix(const gsl_matrix * gslMatrix);
	double generateRandomNumber(int &random_value_x,int &random_value_y, int &random_value_z);
	
};



#endif
