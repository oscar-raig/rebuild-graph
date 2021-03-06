//
//  ProgramOptions.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 20/7/15.
//  Copyright (c) 2015 Oscar Raig Colon. All rights reserved.
//

#include <string>
#include "ProgramOptions.h"
#include "boost/program_options.hpp"
#include <boost/format.hpp>
#include "SettingsSimulation.h"

using namespace std;
using namespace  boost::program_options;
namespace po = boost::program_options;


ProgramOptions::ProgramOptions(int argc, const char *argv[]){
    settingsSimulation = readConfiguration( argc,  argv );
}

SettingsSimulation * ProgramOptions::getSettings() {
    return settingsSimulation;
}


int ProgramOptions::GetAlgorithmFromArgument( po::variables_map argumentMap ){
    int Algorithm = COMMUNICABILITY_BETWEENESS_CENTRALITY;
    if (argumentMap.count("algorithm")){
        Algorithm = argumentMap["algorithm"].as<int>();
        switch (Algorithm){
            case BETWEENNESS_CENTRALITY:
                printf("Option BETWEENESS CENTRALITY\n");
                break;
                
            case COMMUNICABILITY_CENTRALITY:
                printf("Option COMMUNICABILITY CENTRALITY\n");
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



po::options_description ProgramOptions::GetOptionsDescription(){
    
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
	("graphFileToCompare", po::value< std::string >(),"graph file in python format to Compare")	
    ("outputFileSufix", po::value< std::string >(),"sufix for output")
    ("k",po::value<double>(),k_argument_description.c_str() )
    ("nIteration",po::value<int>(),nIteration_argument_description.c_str())
    ("nMax",po::value<int>(),nMax_argument_description.c_str())
    ("To",po::value<double>(),To_argument_description.c_str())
    
    ("seed_x",po::value<int>(),seed_x_argument_description.c_str())
    ("seed_y",po::value<int>(),seed_y_argument_description.c_str())
    ("seed_z",po::value<int>(),seed_z_argument_description.c_str())
    
    ("TMin",po::value<double>(),"TMin")
    ("output-format-adjlist","output format adjlist")
	("only-compare","compare two graphs")	
    ("threshold-accepting","Using threshold accepting")
    ("only-calculate-indicator","only calculates the BC, CB or CBC of the input file")
    ("reescale-cc","Reescale Communicability Centrality, BC and CBC are always reescale ")
    ("algorithm",po::value<int>(),"algorithm 1:BETWEENESS CENTRALITY, 2:COMMUNICABILITY BETWEENESS, 3: COMMUNICABILITY BETWEENESS CENTRALITY ");
    return argumentDescription;
}

SettingsSimulation * ProgramOptions::readConfiguration(int argc, const char * argv[] ){
    
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
    
    bool outputFormatDefaultAdjacencyList = false;
    if ( argumentMap.count("output-format-adjlist")  )
    {
        outputFormatDefaultAdjacencyList = true;
        std::cout << "Output Format Ajdacency list" << std::endl;
    }
    
    po::notify(argumentMap); // throws on error, so do after help in case
    
    int Algorithm = GetAlgorithmFromArgument(argumentMap);
    
    SettingsSimulation *settingsSimulation = new SettingsSimulation() ;
    settingsSimulation->inputFileName =argumentMap["graphFile"].as<std::string>();
    if (argumentMap.count("outputFileSufix"))
    {
        settingsSimulation->outputFileNameSuFix = argumentMap["outputFileSufix"].as<std::string>();
    }

    if ( argumentMap.count("only-calculate-indicator")  )
    {
        std::cout << "We only calculate  the algorithm" << std::endl;
        settingsSimulation->onlyCalculateIndicator = true;
    }   
	if ( argumentMap.count("only-compare")  )
    {
        std::cout << "We only Compare two graphs" << std::endl;
        only_compare = true;
		if ( !argumentMap.count("graphFileToCompare")){
        	std::cout << "argumentMap count" << argumentMap.count("graphFileToCompare");
        	std::cout << "graphFileToCompare" <<  argumentMap["graphFileToCompare"].as<std::string>() << std::endl;
        	return NULL;
    	} else {
			graphFileToCompare = argumentMap["graphFileToCompare"].as<std::string>();;
		}
		return settingsSimulation;
    }   
 
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
        settingsSimulation->To = argumentMap["To"].as<double>();
        std::cout << "To " << settingsSimulation->To  << std::endl;
        
    }
    if (argumentMap.count("TMin"))
    {
        settingsSimulation->tMin = argumentMap["TMin"].as<double>();
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
    if ( argumentMap.count("threshold-accepting")  )
    {
        settingsSimulation->thresholdAccepting = THRESHOLD_ACCEPTING_ALGORITHM;
        std::cout << "Threshold-accepting" << std::endl;
    }
    
    if ( argumentMap.count("reescale-cc")  )
    {
        reescale = true;
        std::cout << "reescale-cc" << std::endl;
    }
    
    
    // CSettingsSimulation *settingsSimulation = new CSettingsSimulation(argumentMap);
    settingsSimulation->graphProperty = Algorithm;
    if ( Algorithm == COMMUNICABILITY_CENTRALITY) {
        if ( reescale == true ) {
            settingsSimulation->reescale = true;
        } else {
            settingsSimulation->reescale = false;
        }
    }
    settingsSimulation->outputFormatGraphResultAdjList = outputFormatDefaultAdjacencyList;
    return settingsSimulation;
    
}
