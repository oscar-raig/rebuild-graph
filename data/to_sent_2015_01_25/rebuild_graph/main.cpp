//
//  main.c
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 22/03/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include <stdio.h>
#include "boost/program_options.hpp"
#include <boost/format.hpp>
#include "CSettingsSumulation.h"
#include "rebuildgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include "FactoryGraphIndicator.h"
#include "graphIndicatorBetweennessCentrality.h"
#include "graphIndicatorCommunicabilityCentralityUsingMatrixExponential.h"
#include "graphIndicatorCommunicabilityBetweennessCentrality.h"


bool only_calculate = false;

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

/*
 #define DEFAULT_TO		10
 #define DEFAULT_T_MIN	0.0001
 #define DEFAULT_N_MAX	1000
 #define DEFAULT_K 0.9
 #define DEFAULT_MAX_ITERATIONS 1000
 #define DEFAULT_SEED_X 11
 #define DEFAULT_SEED_Y 92
 #define DEFAULT_SEED_Z 37
 
 */

po::options_description GetOptionsDescription(){

	string k_argument_description = "k description, default value: "
		+ boost::str(boost::format("%.2f") % DEFAULT_K);
	string nIteration_argument_description = "nIteration, default value: "
		+ boost::lexical_cast<string>(DEFAULT_MAX_ITERATIONS);
	string nMax_argument_description = "nMax, default value: "
		+ boost::lexical_cast<string>(DEFAULT_N_MAX);
	string To_argument_description = "To, default value: "
		+ boost::lexical_cast<string>(DEFAULT_TO);
	string seed_x_argument_description = "seed x, default value: "
	+ boost::lexical_cast<string>(DEFAULT_SEED_X);
	
	string seed_y_argument_description = "seed y, default value: "
	+ boost::lexical_cast<string>(DEFAULT_SEED_Y);
	
	string seed_z_argument_description = "seed z, default value: "
	+ boost::lexical_cast<string>(DEFAULT_SEED_Z);
	
	
	
	po::options_description argumentDescription("Options");
	argumentDescription.add_options()
	("help", "Print help messages" )
	("graphFile", po::value< std::string >(),"graph file in python format")
	("k",po::value<double>(),k_argument_description.c_str() )
	("nIteration",po::value<int>(),nIteration_argument_description.c_str())
	("nMax",po::value<int>(),nMax_argument_description.c_str())
	("To",po::value<int>(),To_argument_description.c_str())

	("seed_x",po::value<int>(),seed_x_argument_description.c_str())
	("seed_y",po::value<int>(),seed_y_argument_description.c_str())
	("seed_z",po::value<int>(),seed_z_argument_description.c_str())
	
	("TMin",po::value<double>(),"TMin")
	("output-format-adjlist","output format adjlist")
	("only-calculate-indicator","only calculates the BC, CB or CBC of the input file")
	("algorithm",po::value<int>(),"algorithm 1:BETWEENESS CENTRALITY, 2:COMMUNICABILITY BETWEENESS, 3: COMMUNICABILITY BETWEENESS CENTRALITY ");
	return argumentDescription;
}


CSettingsSimulation * readConfiguration(int argc, const char * argv[] ){
	
	po::options_description argumentDescription = GetOptionsDescription();
	
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
	if ( argumentMap.count("only-calculate-indicator")  )
	{
		std::cout << "We only calculate  the algorithm" << std::endl;
		only_calculate = true;
	}
	bool outputFormatDefaultAdjacencyList = false;
	if ( argumentMap.count("output-format-adjlist")  )
	{
		outputFormatDefaultAdjacencyList = true;
		std::cout << "Output Format Ajdacency list" << std::endl;
	}
	
	po::notify(argumentMap); // throws on error, so do after help in case
	
	int Algorithm = GetAlgorithmFromArgument(argumentMap);
	
	CSettingsSimulation *settingsSimulation = new CSettingsSimulation() ;
	settingsSimulation->inputFileName =argumentMap["graphFile"].as<std::string>();
	
	if (argumentMap.count("k"))
	{
		settingsSimulation->k = argumentMap["k"].as<double>();
		std::cout << "k " << settingsSimulation->k  << std::endl;
		
	}

	if (argumentMap.count("nIteration"))
	{
		settingsSimulation->maxIterations = argumentMap["nIteration"].as<int>();
		std::cout << "nIteration " << settingsSimulation->maxIterations  << std::endl;
		
	}
	if (argumentMap.count("nMax"))
	{
		settingsSimulation->nMax = argumentMap["nMax"].as<int>();
		std::cout << "nMax " << settingsSimulation->nMax  << std::endl;
		
	}
	if (argumentMap.count("To"))
	{
		settingsSimulation->nMax = argumentMap["To"].as<int>();
		std::cout << "To " << settingsSimulation->To  << std::endl;
		
	}
	if (argumentMap.count("TMin"))
	{
		settingsSimulation->nMax = argumentMap["TMin"].as<double>();
		std::cout << "TMin " << settingsSimulation->tMin  << std::endl;
		
	}
	
	
	if (argumentMap.count("seed_x"))
	{
		settingsSimulation->random_value_x = argumentMap["seed_x"].as<int>();
		std::cout << "seed_x " << settingsSimulation->random_value_x  << std::endl;
		
	}
	if (argumentMap.count("seed_y"))
	{
		settingsSimulation->random_value_y = argumentMap["seed_y"].as<int>();
		std::cout << "seed_y " << settingsSimulation->random_value_y  << std::endl;
		
	}
	if (argumentMap.count("seed_z"))
	{
		settingsSimulation->random_value_z = argumentMap["seed_z"].as<int>();
		std::cout << "seed_z " << settingsSimulation->random_value_z  << std::endl;
		
	}
	
	
	
	// CSettingsSimulation *settingsSimulation = new CSettingsSimulation(argumentMap);
	settingsSimulation->graphProperty = Algorithm;
	settingsSimulation->outputFormatGraphResultAdjList = outputFormatDefaultAdjacencyList;
	
	return settingsSimulation;
}

int main(int argc, const char * argv[])
{
	CSettingsSimulation * settingsSimulation=  readConfiguration(argc,argv);
	if (!settingsSimulation){
		std::cout << "ERROR: Reading the configuration";
		return -1;
	}
	
	if (only_calculate ){
		gslGraph *gsl_Graph = new gslGraph();
		gsl_Graph->readPythonGraphFile(settingsSimulation->inputFileName);
		double *arrayIndicator = NULL;
		

		 graphIndicator *graphIndicator = FactoryGraphIndicator::CreategraphIndicator(settingsSimulation->graphProperty, gsl_Graph);
		arrayIndicator = graphIndicator->calculateIndicator();
		for ( int i=0; i < gsl_Graph->getOrder(); i++){
			std::cout << " Result pos : " << i << " = " << arrayIndicator[i]<< std::endl;
		}
		return 0;
	}
	
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	double compareResult = 0.0;
	double *TargetBC = NULL;
	double *BestBC = NULL;
	int order = 0;
    rebuildGraph->regenerateGraph(settingsSimulation,TargetBC,BestBC,order,compareResult);

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
	if ( TargetBC)
		delete TargetBC;
			
	return 0;
}

