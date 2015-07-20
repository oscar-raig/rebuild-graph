//
//  AbstractFactoryPatternRebuildGraph.h
//  TestRebuildGraph
//
//  Created by Oscar Raig Colon on 26/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __TestRebuildGraph__AbstractFactoryPatternRebuildGraph__
#define __TestRebuildGraph__AbstractFactoryPatternRebuildGraph__

#include <iostream>
#include "SettingsSimulation.h"
#include "rebuildgraph.h"
#include "StrategyPatternAlgorithm.h"

// Abstract Factory Pattern than handles the
// three classes involved, rebuildgrAPH, settingSimulation, strategyPatternAlgorithm

class AbstractFactoryPatternRebuildGraph {
	
private:
	CRebuildGraph *rebuildGraph;
	SettingsSimulation *settingSimulation;
	
public:
	AbstractFactoryPatternRebuildGraph (int algorithm, std::string GraphName,
										int nMax, double * compareResult, double To, bool thresholdAccepting ){
		int graphOrder = 0;
		this->settingSimulation= new SettingsSimulation() ;
		
		settingSimulation->inputFileName = "/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/" + GraphName;
		
		settingSimulation->setNMax(nMax);
		settingSimulation->setMaxIterations(100);
		*compareResult = 10.0;
		this->rebuildGraph = new CRebuildGraph();
		double *BestBC = NULL;
		if (algorithm)
			settingSimulation->graphProperty = algorithm;
		
		if (settingSimulation->graphProperty == COMMUNICABILITY_CENTRALITY )
			settingSimulation->reescale = false;
		
		settingSimulation->To = To;
		if ( thresholdAccepting )
			settingSimulation->thresholdAccepting = THRESHOLD_ACCEPTING_ALGORITHM;
		else
			settingSimulation->thresholdAccepting = ANNEALING_ALGORITHM;
		
		rebuildGraph->regenerateGraph(settingSimulation,BestBC,graphOrder,*compareResult);
	}
	
	~AbstractFactoryPatternRebuildGraph(){
		if (settingSimulation)
			delete settingSimulation;
		if (rebuildGraph)
			delete rebuildGraph;
	}
	
};

#endif /* defined(__TestRebuildGraph__AbstractFactoryPatternRebuildGraph__) */
