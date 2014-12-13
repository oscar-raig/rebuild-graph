//
//  GraphFactory.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 29/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include "GraphFactory.h"



GeneralGraph * GraphFactory::createGraph(int graphClass){
	if (graphClass== GRAPH){
		return new graph();
	}else{
		return new gslGraph();
	}
}
GeneralGraph *  GraphFactory::createGraph(int graphClass,int sizeOfMatrix){
	if (graphClass== GRAPH){
		return new graph(sizeOfMatrix);
	}else{
		return new gslGraph(sizeOfMatrix);
	}
}