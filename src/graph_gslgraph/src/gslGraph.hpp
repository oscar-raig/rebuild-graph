//
//  gslGraph.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 29/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__gslGraph__
#define __rebuild_graph__gslGraph__

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <limits>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_linalg.h>
#include "CTrace.hpp"
#define RESULT_OK 1



//#define DEBUG_GSL_GRAPH CTrace::TRACE_INFO
#define DEBUG_GSL_GRAPH CTrace::TRACE_DEBUG

#define INFO_GSL_GRAPH CTrace::TRACE_INFO
//#define INFO_GSL_GRAPH CTrace::TRACE_DEBUG
#define ERROR_GSL_GRAPH CTrace::TRACE_ERROR


class CRebuildGraph;

class gslGraph  {
public:
	enum COMPARE_GRAPH
	{
		GRAPH_DIFFERENTS,
		GRAPH_EQUALS,
	};
	enum VERTEX_CONNECTED{
		VERTEX_DISCONNECTED,
		VERTEX_CONNCTED,
	};
private:
	int order;
	int degree;
	gsl_matrix * matrix;
	int *vertex_degree;
	
public:
	
	gslGraph();
	gslGraph(int sizeOfMatrix);
	virtual ~gslGraph();
	
	virtual gslGraph*	copyGraph() const ;
	gsl_matrix* getGslMatrix(){return matrix;}
	
	// Get the number of vertex (order) of the graph
	inline int getOrder() const { return order;};
	
	// Get the graph degree
	inline int  getDegree() const {
		return this->degree;
	};
	
	inline int getDegree(int vertex)const {
		
		if ( vertex > order )
			throw "ERROR: Asking for vertex greater than order";
		
		
		return vertex_degree[vertex];
	}
	virtual void printMyGraph(const char * outputGraphFilename,bool outputAsAdjacencyList) const;
	void printGraph(int TRACE_LEVEL = CTrace::TRACE_DEBUG);
	
	
	// Add a vertex with the given newVertexId identifier to the graph
    int addVertex(int newVertexId);
	int addVertexNeighbour(int sourceVertex,int newNeighbour);
	virtual void addNewVertexNeighbour(int sourceVertex,int newNeighbour);
	virtual void removeVertexNeighbours(int vertexToRemoveNegighbours);

	virtual int graphToGsl( gsl_matrix* target);
	gsl_vector * getDiagonalFromGslMatrix(const gsl_matrix * gslMatrix);
	static int gslVectorToArray(gsl_vector* gslVector, double* arrayDoubles);
	virtual void  communicability_betweenness_centrality(double *myCExp){
					CFuncTrace trace (true,"communicability_betweenness_centrality");
		            trace.trace(CTrace::TRACE_ERROR,"Not implemented");
	}

	virtual int vertexAreNeighbours(int vertexBegining,int vertexEnding);
	virtual int graphNotConnected (int *unconnectedVertex);
	
	
	
};

#endif /* defined(__rebuild_graph__gslGraph__) */
