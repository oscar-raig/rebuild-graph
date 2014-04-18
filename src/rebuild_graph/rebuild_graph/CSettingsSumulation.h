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
#define DEFAULT_TOL 0.0001
#define DEFAULT_MAX_ITERATIONS 1000

// Park-Miller pseudo random  number generator
#define DEFAULT_SEED_X 11
#define DEFAULT_SEED_Y 92
#define DEFAULT_SEED_Z 37


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
	
	
	CSettingsSimulation():
	k(DEFAULT_K),nMax(DEFAULT_N_MAX),
	tMin(DEFAULT_T_MIN),To(DEFAULT_TO),
	random_value_x(DEFAULT_SEED_X),
	random_value_y(DEFAULT_SEED_Y),
	random_value_z(DEFAULT_SEED_Z)
	{
	}
	
	CSettingsSimulation(po::variables_map argumentMap):
		k(DEFAULT_K),nMax(DEFAULT_N_MAX),tMin(DEFAULT_T_MIN),
		To(DEFAULT_TO),
		random_value_x(DEFAULT_SEED_X),
		random_value_y(DEFAULT_SEED_Y),
		random_value_z(DEFAULT_SEED_Z)
	{
		if (argumentMap.count("graphFile")){
			inputFileName = argumentMap["graphFile"].as<std::string>();
		}
		
	};
};


#endif /* defined(__rebuild_graph__CSettingsSumulation__) */
