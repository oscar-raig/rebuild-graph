//
//  GraphFactory.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 29/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__GraphFactory__
#define __rebuild_graph__GraphFactory__

#include <iostream>
#include "gslGraph.h"

#include "gslGraph.h"


class GraphFactory {
	
public:
	static gslGraph * createGraph(int graphClass);
	static gslGraph * createGraph(int graphClass,int sizeOfMatrix);
	
};
#endif /* defined(__rebuild_graph__GraphFactory__) */
