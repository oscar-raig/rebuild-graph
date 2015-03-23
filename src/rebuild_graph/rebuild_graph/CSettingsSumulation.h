//
//  CSettingsSumulation.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 10/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__CSettingsSumulation__
#define __rebuild_graph__CSettingsSumulation__

#include <iostream>
#include "boost/program_options.hpp"
namespace po = boost::program_options;


#define DEFAULT_TO		10
#define DEFAULT_T_MIN	0.0001
#define DEFAULT_N_MAX	1000
#define DEFAULT_K 0.9
#define DEFAULT_MAX_ITERATIONS 1000

// Park-Miller pseudo random  number generator
#define DEFAULT_SEED_X 11
#define DEFAULT_SEED_Y 92
#define DEFAULT_SEED_Z 37



#define BETWEENNESS_CENTRALITY     1
#define COMMUNICABILITY_CENTRALITY 2
#define COMMUNICABILITY_BETWEENESS_CENTRALITY 3

#define ANNEALING_ALGORITHM				1
#define THRESHOLD_ACCEPTING_ALGORITHM	2


#define OUTPUT_BINARY false
#define OUTPUT_ADJACENCY_LIST true
class CSettingsSimulation{
	
public:
	double	k;
	int		nMax;
	double	tMin;
	double	To;
	std::string inputFileName;
	int random_value_x;
	int random_value_y;
	int random_value_z;
	int graphProperty;
	int maxIterations;
	int outputFormatGraphResultAdjList;
	int thresholdAccepting;
	
	CSettingsSimulation():
	k(DEFAULT_K),nMax(DEFAULT_N_MAX),
	tMin(DEFAULT_T_MIN),To(DEFAULT_TO),
	random_value_x(DEFAULT_SEED_X),
	random_value_y(DEFAULT_SEED_Y),
	random_value_z(DEFAULT_SEED_Z),
	graphProperty(BETWEENNESS_CENTRALITY),
	maxIterations(DEFAULT_MAX_ITERATIONS),
	outputFormatGraphResultAdjList(OUTPUT_ADJACENCY_LIST),
	thresholdAccepting(ANNEALING_ALGORITHM)
	{
	}
	
	CSettingsSimulation(po::variables_map argumentMap):
		k(DEFAULT_K),nMax(DEFAULT_N_MAX),tMin(DEFAULT_T_MIN),
		To(DEFAULT_TO),
		random_value_x(DEFAULT_SEED_X),
		random_value_y(DEFAULT_SEED_Y),
		random_value_z(DEFAULT_SEED_Z),
		graphProperty(BETWEENNESS_CENTRALITY),
		maxIterations(DEFAULT_MAX_ITERATIONS),
		outputFormatGraphResultAdjList(OUTPUT_ADJACENCY_LIST),
		thresholdAccepting(ANNEALING_ALGORITHM)
	{
		if (argumentMap.count("graphFile")){
			inputFileName = argumentMap["graphFile"].as<std::string>();
		}
		
	};
	
	CSettingsSimulation(CSettingsSimulation *sourceSettings) {
		k = sourceSettings->k;
		nMax = sourceSettings->nMax ;
		tMin = sourceSettings->tMin;
		To = sourceSettings->To;
		inputFileName = sourceSettings->inputFileName;
		random_value_x = sourceSettings->random_value_x;
		random_value_y = sourceSettings->random_value_y;
		random_value_z = sourceSettings->random_value_z;
		graphProperty = sourceSettings->graphProperty;
		maxIterations = sourceSettings->maxIterations;
		outputFormatGraphResultAdjList = sourceSettings->outputFormatGraphResultAdjList;
		thresholdAccepting = sourceSettings->thresholdAccepting;
	}
	
	void setNMax( int newNmax)
	{
		nMax = newNmax;
	}
	void setMaxIterations (int newMaxIterations)
	{
		maxIterations = newMaxIterations;
	}
	
	void Reset()
	{
		k = DEFAULT_K;
		nMax = DEFAULT_MAX_ITERATIONS;
		tMin = DEFAULT_T_MIN;
		To= DEFAULT_TO;
		random_value_x = DEFAULT_SEED_X;
		random_value_y = DEFAULT_SEED_Y;
		random_value_z = DEFAULT_SEED_Z;
		
		
	}
	
};


#endif /* defined(__rebuild_graph__CSettingsSumulation__) */
