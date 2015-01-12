//
//  main.c
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 22/03/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include <stdio.h>
#include "boost/program_options.hpp"
#include "CSettingsSumulation.h"
#include "rebuildgraph.h"
#include <stdio.h>
#include <stdlib.h>
int
fregenerateGraph(CSettingsSimulation &settingsSimulation, double *&ptargetBC, double *&pbestBC,int *order);
int fCalculateBeterness(const char *argv[]);
int fCalculateCommunicability(const char *argv[]);
int fCalculateCommunicability_cent_exp(const char *argv[]);

int GetAlgorithmFromArgument( po::variables_map argumentMap ){
	int Algorithm = COMMUNICABILITY_BETWEENESS_CENTRALITY;
	if (argumentMap.count("algorithm")){
		Algorithm = argumentMap["algorithm"].as<int>();
		switch (Algorithm){
			case BETWEENNESS_CENTRALITY:
				printf("Option BETWEENESS CENTRALITY\n");
				break;
				
			case COMMUNICABILITY_BETWEENESS:
				printf("Option COMMUNICABILITY BETWEENESS\n");
				break;
				
			case COMMUNICABILITY_BETWEENESS_CENTRALITY:
			default:
				Algorithm = COMMUNICABILITY_BETWEENESS_CENTRALITY;
				printf("Option COMMUNICABILITY BETWEENESS CENTRALITY\n");
				break;
		}
	}
	else
	{
		printf("No algorithm option>>COMMUNICABILITY_BETWEENESS_CENTRALITY\n");
		Algorithm =COMMUNICABILITY_BETWEENESS_CENTRALITY;
	}
	return Algorithm;
}

CSettingsSimulation * readConfiguration(int argc, const char * argv[] ){
	
	namespace po = boost::program_options;
    po::options_description argumentDescription("Options");
    argumentDescription.add_options()
	("help", "Print help messages")
	("graphFile", po::value< std::string >(),"graph file in python format")
	("k","k description")
	("algorithm",po::value<int>(),"algorithm 1:BETWEENESS CENTRALITY, 2:COMMUNICABILITY BETWEENESS, 3: COMMUNICABILITY BETWEENESS CENTRALITY ");
	
	
	po::variables_map argumentMap;
	po::store(po::parse_command_line(argc, argv, argumentDescription),
			  argumentMap);
	po::notify(argumentMap);
	if ( argumentMap.count("help")  )
	{
        std::cout << "Basic Command Line Parameter App" << std::endl
		<< argumentDescription << std::endl;
        return NULL;
	}
	if ( !argumentMap.count("graphFile")){
		std::cout << "argumentMap count" << argumentMap.count("graphFile");
		std::cout << "graphFile" <<  argumentMap["graphFile"].as<std::string>() << std::endl;
		return NULL;
	}
	
	
	po::notify(argumentMap); // throws on error, so do after help in case
	
	int Algorithm = GetAlgorithmFromArgument(argumentMap);
	
	CSettingsSimulation *settingsSimulation = new CSettingsSimulation() ;
	settingsSimulation->inputFileName =argumentMap["graphFile"].as<std::string>();
	
	
	
	
	
	// CSettingsSimulation *settingsSimulation = new CSettingsSimulation(argumentMap);
	

	
	settingsSimulation->graphProperty = Algorithm;
	return settingsSimulation;
}

int main(int argc, const char * argv[])
{

	
	
	CSettingsSimulation * settingsSimulation=  readConfiguration(argc,argv);
	if (!settingsSimulation){
		std::cout << "ERROR: Reading the configuration";
		return -1;
	}
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	double compareResult = 0.0;
	double *TargetBC = NULL;
	double *BestBC = NULL;
	int order = 0;
    rebuildGraph->regenerateGraph(*settingsSimulation,TargetBC,BestBC,order,compareResult);

	
	
	if (settingsSimulation){
		free(settingsSimulation);
		settingsSimulation= NULL;
	}
	return 0;
}

