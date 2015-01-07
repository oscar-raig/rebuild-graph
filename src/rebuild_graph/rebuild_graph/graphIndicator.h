//
//  graphIndicator.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 22/12/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__graphIndicator__
#define __rebuild_graph__graphIndicator__

#include <stdio.h>
#include "gslGraph.h"

class graphIndicator {
public:
	gslGraph *graph;
public:
	graphIndicator( gslGraph *argGraph ):graph(argGraph){};
	void setGraph( gslGraph *graph){
		this->graph = graph;
	}
	gslGraph * getGraph( ){
		return graph;
	}
	virtual double *calculateIndicator() = 0;
};

#endif /* defined(__rebuild_graph__graphIndicator__) */
