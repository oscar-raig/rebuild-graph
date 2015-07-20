//
//  main.c
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 22/03/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include <stdio.h>
#include <string>
#include "boost/program_options.hpp"
#include <boost/format.hpp>
#include "SettingsSimulation.h"
#include "rebuildgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include "FactoryMethodGraphIndicator.h"

#include "ProgramOptions.h"






int main(int argc, const char * argv[])
{
    ProgramOptions *programOptions = new ProgramOptions(argc,argv);
	SettingsSimulation * settingsSimulation=  programOptions->readConfiguration(argc,argv);
	if (!settingsSimulation){
		std::cout << "ERROR: Reading the configuration";
		return -1;
	}
	
	if (settingsSimulation->onlyCalculateIndicator ){
        gslGraph *gsl_Graph = ReadPythonGraphFile::readPythonGraphFile(settingsSimulation->inputFileName);
		double *arrayIndicator = NULL;
		

		 graphIndicator *graphIndicator = FactoryMethodGraphIndicator::createGraphIndicator(settingsSimulation->graphProperty, gsl_Graph);
		arrayIndicator = graphIndicator->calculateIndicatorWithReescale( settingsSimulation->reescale);
		for ( int i=0; i < gsl_Graph->getOrder(); i++){
			std::cout << " Result pos : " << i << " = " << arrayIndicator[i]<< std::endl;
		}
		return 0;
	}
	
	
	
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	double compareResult = 0.0;
	double *BestBC = NULL;
	int order = 0;
    rebuildGraph->regenerateGraph(settingsSimulation,BestBC,order,compareResult);

	std::cout << "RegenerateGraph " <<compareResult << std::endl;
	
	if (settingsSimulation){
		delete settingsSimulation;
		settingsSimulation= NULL;
	}
	if ( rebuildGraph ){
		delete rebuildGraph;
		rebuildGraph = NULL;
	}
	if ( BestBC )
		delete BestBC;
//	if ( TargetBC)
//		delete TargetBC;
			
	return 0;
}

