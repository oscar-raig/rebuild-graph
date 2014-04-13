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


#define TO_DEFAULT 10
#define T_MIN_DEFAULT 0.0001
#define N_MAX_DEFAULT 1000
#define K_DEFAULT 0.9
#define TOL 0.0001
#define MAX_ITERATIONS 1000

class CSettingsSimulation{
	
public:
	double	k;
	int		nMax;
	double	tMin;
	double	To;
	std::string inputFileName;
	
	CSettingsSimulation():
	k(K_DEFAULT),nMax(N_MAX_DEFAULT),tMin(T_MIN_DEFAULT),To(TO_DEFAULT)
	{
	}
	
	CSettingsSimulation(po::variables_map argumentMap):
		k(K_DEFAULT),nMax(N_MAX_DEFAULT),tMin(T_MIN_DEFAULT),To(TO_DEFAULT)
	{
		if (argumentMap.count("graphFile")){
			inputFileName = argumentMap["graphFile"].as<std::string>();
		}
		
	};
};


#endif /* defined(__rebuild_graph__CSettingsSumulation__) */
