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
	gslGraph * getGraph( ) {
		return graph;
	}
	
	double *calculateIndicatorWithReescale(bool Reescale) {
		
		double * arrayIndicator = calculateIndicator();
		if ( Reescale ) {
			rescale(arrayIndicator,this->graph->getOrder());
		}
		return arrayIndicator;
	}
	virtual ~graphIndicator(){};
private:
	virtual  double *calculateIndicator() = 0;

	void  rescale( double *arrayIndicator,int sizeArray) {
		if ( sizeArray <= 2)
			return;
		double scale = 1.0/((pow(sizeArray-1.0,2))-(sizeArray-1.0));
		
		for ( int posArray = 0; posArray < sizeArray; posArray++ ){
			arrayIndicator[posArray] *= scale;
		}
	}
};

#endif /* defined(__rebuild_graph__graphIndicator__) */
