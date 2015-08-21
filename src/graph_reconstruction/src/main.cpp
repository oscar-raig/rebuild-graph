//
//  main.c
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 22/03/14
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "SettingsSimulation.h"
#include "rebuildgraph.h"
#include "FactoryMethodGraphIndicator.h"
#include "ProgramOptions.h"

#define SUCCESS 0
#define ERROR   -1

int calculateIndicator(SettingsSimulation * settings ) {
    gslGraph *gsl_Graph =
    ReadPythonGraphFile::readPythonGraphFile(settings->inputFileName);
    double *arrayIndicator = NULL;

    graphIndicator *graphIndicator =
    FactoryMethodGraphIndicator::createGraphIndicator(settings->graphProperty,
                                                      gsl_Graph);
    arrayIndicator =
    graphIndicator->calculateIndicatorWithReescale(settings->reescale);
    for (int i=0; i< gsl_Graph->getOrder(); i++) {
        std::cout << " Result pos : " << i;
        std::cout<< " = " << arrayIndicator[i]<< std::endl;
    }
    return SUCCESS;
}

int main(int argc, const char * argv[]) {
    ProgramOptions *programOptions = new ProgramOptions(argc, argv);
    SettingsSimulation * settingsSimulation =
        programOptions->readConfiguration(argc, argv);
    if (!settingsSimulation) {
        std::cout << "ERROR: Reading the configuration";
        return ERROR;
    }

    if (settingsSimulation->onlyCalculateIndicator) {
        return calculateIndicator(settingsSimulation);
    }

    CRebuildGraph *rebuildGraph = new CRebuildGraph();
    double compareResult = 0.0;
    double *BestBC = NULL;
    int order = 0;
    rebuildGraph->regenerateGraph(settingsSimulation,
                                  BestBC, order, compareResult);

    std::cout << "RegenerateGraph " <<compareResult << std::endl;

    if (settingsSimulation) {
        delete settingsSimulation;
        settingsSimulation = NULL;
    }
    if (rebuildGraph) {
        delete rebuildGraph;
        rebuildGraph = NULL;
    }
    if (BestBC) {
        delete BestBC;
    }
    if (programOptions) {
        delete programOptions;
    }
    return SUCCESS;
}

