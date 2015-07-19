//
//  graphIndicator.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 22/12/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include "graphIndicator.h"


graphIndicator::graphIndicator( gslGraph *argGraph ):graph(argGraph) {

}

graphIndicator::~graphIndicator() {

};


void graphIndicator::setGraph( gslGraph *graph){
    this->graph = graph;
}

gslGraph *graphIndicator::getGraph( ) {
    return graph;
}

void  graphIndicator::rescale( double *arrayIndicator,int sizeArray) {
    if ( sizeArray <= 2)
        return;
    double scale = 1.0/((pow(sizeArray-1.0,2))-(sizeArray-1.0));
    
    for ( int posArray = 0; posArray < sizeArray; posArray++ ){
        arrayIndicator[posArray] *= scale;
    }
}

double *graphIndicator::calculateIndicatorWithReescale(bool Reescale) {
    
    double * arrayIndicator = calculateIndicator();
    if ( Reescale ) {
        rescale(arrayIndicator,this->graph->getOrder());
    }
    return arrayIndicator;
}