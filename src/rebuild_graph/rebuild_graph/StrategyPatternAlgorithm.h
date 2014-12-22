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
	
public:
	StrategyPatternAlgorithm(CSettingsSimulation *argummentSettingsSimulation)
	{
		CFuncTrace trace(true,"StrategyPatternAlgorithm");
		if ( argummentSettingsSimulation)
			settingsSimulation = argummentSettingsSimulation;
		else
			settingsSimulation = new CSettingsSimulation();
	};
	
	
	static double cost(double *tarjet,double *current,int count);

	void modifyGraph(gslGraph *sourceGraph);
	double generateRandomNumber();
	gslGraph *generateInitialGraph(int sourceGraphOrder);
	
	int
	regenerateGraph(gslGraph *targetGraph,
					char *inputFilename,
					double *&targetBC,
					double *&bestBC,
					int &graphOrder,
					double &compareResult,
					double *Tk,
					double *costBest,
					gslGraph **bestGraph);
	// Rebuilding Graph Main Functions

	void AnnealingAlgorithm(double &Tk, gslGraph **pbestGraph,int graphOrder,
							double *bestBC,double *targetBC,
							FILE *logFile,double &costBest,
							CSettingsSimulation settingSimulation);
};

#endif /* defined(__rebuild_graph__SrategyPatternAlgorithm__) */
