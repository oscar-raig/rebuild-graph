//
//  FactoryGraphIndicator.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 25/1/15.
//  Copyright (c) 2015 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__FactoryGraphIndicator__
#define __rebuild_graph__FactoryGraphIndicator__

#include <stdio.h>

#include "gslGraph.hpp"
#include "graphIndicatorBetweennessCentrality.h"
#include "graphIndicatorCommunicabilityCentralityUsingMatrixExponential.h"
#include "graphIndicatorCommunicabilityBetweennessCentrality.h"


#define BETWEENNESS_CENTRALITY     1
#define COMMUNICABILITY_CENTRALITY 2
#define COMMUNICABILITY_BETWEENESS_CENTRALITY 3

class FactoryMethodGraphIndicator{
public:
	static graphIndicator *createGraphIndicator( int indicator, gslGraph* gslGraph ){
		if ( indicator == BETWEENNESS_CENTRALITY)
			return new graphIndicatorBetweennessCentrality(gslGraph);
		else if ( indicator == COMMUNICABILITY_CENTRALITY)
			return new graphIndicatorCommunicabilityCentralityUsingMatrixExponential(gslGraph);
		else if ( indicator == COMMUNICABILITY_BETWEENESS_CENTRALITY ) {
			return new graphIndicatorCommunicabilityBetweennessCentrality(gslGraph);
		}
		else {
			return NULL;
		}
	};
	
};

#endif /* defined(__rebuild_graph__FactoryGraphIndicator__) */
