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
#include "gslGraph.hpp"

class graphIndicator {
public:
	graphIndicator( gslGraph *argGraph );
    virtual ~graphIndicator();
    void setGraph( gslGraph *graph);
    gslGraph * getGraph() ;
    double *calculateIndicatorWithReescale(bool Reescale);
   
private:
	virtual  double *calculateIndicator() = 0;
    void  rescale( double *arrayIndicator,int sizeArray);
private:
    gslGraph *graph;
};

#endif /* defined(__rebuild_graph__graphIndicator__) */
