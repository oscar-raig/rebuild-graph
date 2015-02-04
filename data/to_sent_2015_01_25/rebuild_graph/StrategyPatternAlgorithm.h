//
//  SrategyPatternAlgorithm.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 27/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__SrategyPatternAlgorithm__
#define __rebuild_graph__SrategyPatternAlgorithm__

#include <iostream>
#include "CSettingsSumulation.h"
#include "CTrace.hpp"
#include "gslGraph.h"

#define TEMPER_INITIAL_DEFAULT 10
#define TEMPER_MIN_DEFAULT 0.0001
#define NUMBER_MAX_COMBINATIONS_DEFAULT 1000
#define K 0.9
#define TOL 0.0001
#define MAX_ITERATIONS 1000

#ifndef STRING_LENGTH
#define STRING_LENGTH 256
#endif

class StrategyPatternAlgorithm {
	
private:
	CSettingsSimulation *settingsSimulation;
	bool settingsSimulationAllocated;
	gslGraph *sourceGraph;
	FILE *logFile;
	
public:
	StrategyPatternAlgorithm(CSettingsSimulation *argummentSettingsSimulation)
	{
		CFuncTrace trace(false,"StrategyPatternAlgorithm");
		
		if ( argummentSettingsSimulation){
			settingsSimulationAllocated= false;
			settingsSimulation = argummentSettingsSimulation;
		}
		else{
			settingsSimulationAllocated = true;
			settingsSimulation = new CSettingsSimulation();
		}
	};

	~StrategyPatternAlgorithm(){
		if (sourceGraph)
			delete sourceGraph;
		if (settingsSimulation && settingsSimulationAllocated )
			delete settingsSimulation;
	}
	
	gslGraph * getGraph(){
		return sourceGraph;
	}
	
	void setGraph(gslGraph * graph){
		if ( sourceGraph ){
			delete sourceGraph;
		}
		sourceGraph = graph;
	}
	
	
	static double cost(double *tarjet,double *current,int count);

	void modifyGraph(gslGraph *sourceGraph);
	double generateRandomNumber();
	void generateInitialGraph(int sourceGraphOrder);
	
	int
	regenerateGraph(gslGraph *targetGraph,
					double *&bestBC,
					int &graphOrder,
					double &compareResult,
					double *Tk,
					double *costBest);
	// Rebuilding Graph Main Functions

	void AnnealingAlgorithm(int graphOrder,
							double *&bestBC,double * targetBC,
							double &costBest);
};

#endif /* defined(__rebuild_graph__SrategyPatternAlgorithm__) */
