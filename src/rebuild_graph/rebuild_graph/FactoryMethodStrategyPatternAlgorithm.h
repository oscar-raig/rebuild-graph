//
//  FactoryStrategyPatternAlgorithm.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 9/2/15.
//  Copyright (c) 2015 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__FactoryStrategyPatternAlgorithm__
#define __rebuild_graph__FactoryStrategyPatternAlgorithm__

#include <stdio.h>
#include "StrategyPatternAlgorithm.h"
#include "CSettingsSumulation.h"


// Desing Patterns: Factory Method
// Parameterized factory methods

class FactoryMethodStrategyPatternAlgorithm{
public:
	static StrategyPatternAlgorithm *createStrategyPatternAlgorithm( int algorithm,CSettingsSimulation *settings){
		CSettingsSimulation settingsCopy = new CSettingsSimulation(settings);
		
		if ( algorithm == ANNEALING_ALGORITHM)
			return new StrategyPatternAlgorithm(&settingsCopy);
		else
			return new StrategyPatternAlgorithmThresholdAccepting(&settingsCopy);
		};
	
};

#endif /* defined(__rebuild_graph__FactoryStrategyPatternAlgorithm__) */
