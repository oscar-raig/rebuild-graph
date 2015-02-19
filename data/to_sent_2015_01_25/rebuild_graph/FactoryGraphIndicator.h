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

#include "gslGraph.h"
#include "graphIndicatorBetweennessCentrality.h"
#include "graphIndicatorCommunicabilityCentralityUsingMatrixExponential.h"
#include "graphIndicatorCommunicabilityBetweennessCentrality.h"


#define BETWEENNESS_CENTRALITY     1
#define COMMUNICABILITY_BETWEENESS 2
#define COMMUNICABILITY_BETWEENESS_CENTRALITY 3

class FactoryGraphIndicator{
public:
	static graphIndicator *CreategraphIndicator( int Indicator, gslGraph* gslGraph ){
		if ( Indicator == BETWEENNESS_CENTRALITY)
			return new graphIndicatorBetweennessCentrality(gslGraph);
		else if ( Indicator == COMMUNICABILITY_BETWEENESS)
			return new graphIndicatorCommunicabilityCentralityUsingMatrixExponential(gslGraph);
		else
			return new graphIndicatorCommunicabilityBetweennessCentrality(gslGraph);
	};
	
};

#endif /* defined(__rebuild_graph__FactoryGraphIndicator__) */