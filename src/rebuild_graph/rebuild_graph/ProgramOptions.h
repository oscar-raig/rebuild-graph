//
//  ProgramOptions.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 20/7/15.
//  Copyright (c) 2015 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__ProgramOptions__
#define __rebuild_graph__ProgramOptions__



#include <stdio.h>
#include "SettingsSimulation.h"

class ProgramOptions {
   
public:
    ProgramOptions(int argc, const char *argv[]);
    SettingsSimulation * readConfiguration(int argc, const char * argv[] );
    
private:
    SettingsSimulation *  settingsSimulation;
    int GetAlgorithmFromArgument( po::variables_map argumentMap );
    po::options_description GetOptionsDescription();
    
private:
    bool only_calculate = false;
    bool reescale = false;
    SettingsSimulation *getSettings();
    
};

#endif /* defined(__rebuild_graph__ProgramOptions__) */
