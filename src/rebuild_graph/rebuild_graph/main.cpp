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
		("k","k description");
	
	
	po::variables_map argumentMap;
	po::store(po::parse_command_line(argc, argv, argumentDescription),
			  argumentMap);
	if ( argumentMap.count("help")  )
	{
        std::cout << "Basic Command Line Parameter App" << std::endl
		<< argumentDescription << std::endl;
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

	const char *largv[2]={"program_name","/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test.gpfc"};

	CSettingsSimulation *settingsSimulation = new CSettingsSimulation() ;
	settingsSimulation->inputFileName =largv[1];
	
	
	//
	
	// CSettingsSimulation *settingsSimulation = new CSettingsSimulation(argumentMap);
    fregenerateGraph(*settingsSimulation,TargetBC,BestBC,&order);
	
	//fCalculateBeterness(argv);
    //fCalculateCommunicability(argv);
	
//	fCalculateCommunicability_cent_exp(argv);
	if (settingsSimulation){
		free(settingsSimulation);
		settingsSimulation= NULL;
	}
	return 0;
}

