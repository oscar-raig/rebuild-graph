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
#include "CSettingsSumulation.h"
#include "rebuildgraph.h"

class AbstractFactoryPatternRebuildGraph {
	
private:
	CRebuildGraph *rebuildGraph;
	CSettingsSimulation *settingSimulation;
public:
	AbstractFactoryPatternRebuildGraph (int algorithm, std::string GraphName, int nMax, double * compareResult){
		int graphOrder = 0;
		settingSimulation= new CSettingsSimulation() ;
		
		settingSimulation->inputFileName = "/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/" + GraphName;
		
		settingSimulation->setNMax(nMax);
		settingSimulation->setMaxIterations(100);
		*compareResult = 10.0;
		this->rebuildGraph = new CRebuildGraph();
		double *TargetBC = NULL;
		double *BestBC = NULL;
		settingSimulation->graphProperty = COMMUNICABILITY_BETWEENESS;
		rebuildGraph->regenerateGraph(settingSimulation,TargetBC,BestBC,graphOrder,*compareResult);
	}
	
	~AbstractFactoryPatternRebuildGraph(){
		delete settingSimulation;
		delete rebuildGraph;
	}
	
};
#endif /* defined(__TestRebuildGraph__AbstractFactoryPatternRebuildGraph__) */
