//
//  readPythonGraphFile.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 20/7/15.
//  Copyright (c) 2015 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__readPythonGraphFile__
#define __rebuild_graph__readPythonGraphFile__

#include <stdio.h>
#include "gslGraph.h"

#ifndef STRING_LENGTH
#define STRING_LENGTH 256
#endif

class ReadPythonGraphFile {

public:
    static gslGraph *readPythonGraphFile(std::string fileName);
    
};

#endif /* defined(__rebuild_graph__readPythonGraphFile__) */
