//
//  GeneralGraph.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 29/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__GeneralGraph__
#define __rebuild_graph__GeneralGraph__

#include <iostream>
#include <matrix/gsl_matrix.h>
#ifndef STRING_LENGTH
#define STRING_LENGTH 256
#endif
#define GSL_GRAPH 0
#define GRAPH 1
class GeneralGraph{
public:
	int nType;
public:
	int GetType() { return nType;}
	GeneralGraph(){};
	virtual GeneralGraph *readPythonGraphFile(char *fileName) = 0;
	// Get the number of vertex (order) of the graph
    virtual int getOrder()const =0 ;
	
	
	
    // Get the graph degree
	virtual int getDegree()const =0;
	virtual int getDegree( int Vertex)const = 0;
	virtual int graphToGsl( gsl_matrix* target)=0;
	virtual void printGraph()=0;
	
	// Get a vertex list of neighbours
	virtual void setAllVertexNeighbours()=0;
	virtual void brandes_betweenness_centrality(double *myBBC){};
	virtual void brandes_comunicability_centrality_exp(double *myCExp){};
	
	virtual void communicability_betweenness_centrality(double *myCExp){};
	virtual void printMyGraph(const char * outputGraphFilename){};
	virtual GeneralGraph* copyGraph()=0;
//	void copyGraph(GeneralGraph *targetGraph)=0;
	virtual void removeVertexNeighbours(int vertexToRemoveNegighbours)=0;
	virtual int  vertexAreNeighbours(int vertexBegining,int vertexEnding)=0;
	virtual int  graphNotConnected(int *unconnectedVertex)=0;
	virtual void addNewVertexNeighbour(int sourceVertex,int newNeighbour)=0;
};

#endif /* defined(__rebuild_graph__GeneralGraph__) */
