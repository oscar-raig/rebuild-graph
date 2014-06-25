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

int main(int argc, const char * argv[])
{

	namespace po = boost::program_options;
    po::options_description argumentDescription("Options");
    argumentDescription.add_options()
		("help", "Print help messages")
		("graphFile", po::value< std::string >(),"graph file in python format")
		("k","k description")
		("algorithm",po::value<int>(),"algorithm BETWEENESS CENTRALITY, ");
	
	
	po::variables_map argumentMap;
	po::store(po::parse_command_line(argc, argv, argumentDescription),
			  argumentMap);
	po::notify(argumentMap);
	if ( argumentMap.count("help")  )
	{
        std::cout << "Basic Command Line Parameter App" << std::endl
		<< argumentDescription << std::endl;
        return 1;
	}
	if ( !argumentMap.count("graphFile"))
	{
		std::cout << "graphFile" <<  argumentMap["graphFile"].as<std::string>() << std::endl;
	   
		return 1;
	}
	
	po::notify(argumentMap); // throws on error, so do after help in case
	// there are any problems
	
	
    // insert code here...
  
	double *TargetBC = NULL;
	double *BestBC = NULL;
	int order = 0;
	
	//
//	const char *largv[2]={"program_name","/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc"};

//	const char *largv[2]={"program_name","/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test.gpfc"};

	CSettingsSimulation *settingsSimulation = new CSettingsSimulation() ;
	settingsSimulation->inputFileName =argumentMap["graphFile"].as<std::string>();
	
	
	double compareResult = 0.0;

	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	// CSettingsSimulation *settingsSimulation = new CSettingsSimulation(argumentMap);
	int option = COMMUNICABILITY_BETWEENESS_CENTRALITY;
	if (argumentMap.count("algorithm")){
		option = atoi(argv[2]);	
		switch (option){
			case BETWEENNESS_CENTRALITY:
				printf("Option BETWEENESS CENTRALITY\n");
				break;

			case COMMUNICABILITY_BETWEENESS:
				printf("Option COMMUNICABILITY BETWEENESS\n");
				break;

			case COMMUNICABILITY_BETWEENESS_CENTRALITY:
			default:
				option = COMMUNICABILITY_BETWEENESS_CENTRALITY;
				printf("Option COMMUNICABILITY BETWEENESS CENTRALITY\n");
				break;
		}
	}	
	else
	{
		printf("No algorithm option>>COMMUNICABILITY_BETWEENESS_CENTRALITY\n");
		option =COMMUNICABILITY_BETWEENESS_CENTRALITY;
	}

	settingsSimulation->graphProperty = option;

	

    rebuildGraph->regenerateGraph(*settingsSimulation,TargetBC,BestBC,order,compareResult);
	
	
	if (settingsSimulation){
		free(settingsSimulation);
		settingsSimulation= NULL;
	}
	return 0;
}

