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
	graph *readPythonGraphFile(char *fileName);
	double generateRandomNumber(int &random_value_x,int &random_value_y, int &random_value_z);
	graph *generateInitialGraph(int sourceGraphOrder,int &random_value_x,int &random_value_y,int &random_value_z);
	graph *copyGraph(graph *sourceGraph);
	void copyGraph(graph *sourceGraph,graph *targetGraph);
	void modifyGraph(graph *sourceGraph,int &random_value_x,int &random_value_y,int &random_value_z);
	double cost(double *tarjet,double *current,int count);
	void generateOutputFile(const  graph *targetGraph,const char *inputFileName,
										   int lx, int ly, int lz, double To, double Tk,double Tmin,
										   double k,long int Nmax,double costBest,double *targetBC,
							double *bestBC, time_t timeStart, time_t timeEnd);
	void AnnealingAlgorithm(double &Tk, int temperInitial,long int numberMaxCombination,graph **pbestGraph,int graphOrder,
										   double *bestBC,double *targetBC,
										   FILE *logFile,double &costBest,
										   CSettingsSimulation settingSimulation);
	int fregenerateGraph(CSettingsSimulation &settingsSimulation, double *&targetBC, double *&bestBC,int *order);
	graph*GetGraphfromFile(const char *argv[]);
	
	int fCalculateBeterness(const char *argv[]);
	gsl_vector_complex * calculateEgeinval (gsl_matrix *target);
	int graphToGsl(graph * source, gsl_matrix* target);
	int printGslMatrix(gsl_matrix* gslMatrix,const char *format="%.3f ");
	int printGslVector(gsl_vector* gslVector);
	gsl_vector *calculateExp(const gsl_vector_complex *eval);
	int fCalculateCommunicability(const char *argv[]);
	gsl_vector *GetDiagonalFromGslMatrix(const gsl_matrix * gslMatrix);
	int fCalculateCommunicability_cent_exp(const char *argv[]);
	
};



#endif
