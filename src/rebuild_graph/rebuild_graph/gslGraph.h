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
#include "CTrace.hpp"
#include "GeneralGraph.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <limits>
#include <matrix/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_linalg.h>
#define RESULT_OK 1



//#define DEBUG_GSL_GRAPH CTrace::level::TRACE_INFO
#define DEBUG_GSL_GRAPH CTrace::level::TRACE_DEBUG

#define INFO_GSL_GRAPH CTrace::level::TRACE_INFO
//#define INFO_GSL_GRAPH CTrace::level::TRACE_DEBUG
#define ERROR_GSL_GRAPH CTrace::level::TRACE_ERROR


class CRebuildGraph;

class gslGraph : public GeneralGraph {
private:
	int order;
	int degree;
	gsl_matrix * matrix;
	
public:
	
	gslGraph();
	gslGraph(int sizeOfMatrix);
	
	virtual GeneralGraph *readPythonGraphFile(char *fileName);
	// Get the number of vertex (order) of the graph
	virtual int getOrder() const { return order;};
	virtual void printMyGraph(const char * outputGraphFilename) const;
	
	
    // Get the graph degree
	virtual int getDegree() const;
	virtual int getDegree(int vertex)const;
	
	// Add a vertex with the given newVertexId identifier to the graph
    int addVertex(int newVertexId);
	int addVertexNeighbour(int sourceVertex,int newNeighbour);
	void printGraph(int TRACE_LEVEL = CTrace::level::TRACE_DEBUG);
	virtual int graphToGsl( gsl_matrix* target){
		gsl_matrix_memcpy(target,matrix);
		return 1;
	}
	virtual void setAllVertexNeighbours(){};
	virtual ~gslGraph(){};
	virtual void
		communicability_betweenness_centrality(double *myCExp){
			CFuncTrace trace (true,"communicability_betweenness_centrality");
			trace.trace(CTrace::level::TRACE_ERROR,"Not implemented");
	}
	
	virtual void brandes_comunicability_centrality_exp(double *myCExp);
	gsl_vector *
	getDiagonalFromGslMatrix(const gsl_matrix * gslMatrix);
	int gslVectorToArray(gsl_vector* gslVector, double* arrayDoubles);
	virtual gslGraph*	copyGraph() const ;
	virtual void copyGraph(gslGraph * newgslGraph) const;
	
	virtual void removeVertexNeighbours(int vertexToRemoveNegighbours);
	virtual int vertexAreNeighbours(int vertexBegining,int vertexEnding);
	virtual int graphNotConnected (int *unconnectedVertex);
	virtual void addNewVertexNeighbour(int sourceVertex,int newNeighbour);
	virtual void brandes_betweenness_centrality(double *myBBC){
		CFuncTrace trace (false,"brandes_betweenness_centrality");
		gsl_vector * v =betweenness_bin(matrix);
		gslVectorToArray(v,myBBC);
		for(int i=0;i< order;i++){
			myBBC[i]=(myBBC[i])/((order-1.0)*(order-2.0));
			trace.trace(CTrace::level::TRACE_DEBUG," BC ",  myBBC[i]);
		}
	}
int printGslMatrix(const gsl_matrix* gslMatrix,const char *format ="%.3f "){
	printf("\n");
	for (size_t i = 0; i < gslMatrix->size1; i++) {
		for (size_t j = 0; j < gslMatrix->size2; j++) {
			printf(format, gsl_matrix_get(gslMatrix, i, j));
		}
		
		printf("\n");
	}
	return RESULT_OK;
}


	/*
 * Computes node betweenness centrality for a binary graph.  Results are
 * returned in a vector where each element is the betweenness centrality of the
 * corresponding node.
 */
	gsl_vector* betweenness_bin(const gsl_matrix* sourceGraph)const;
	gsl_matrix * copy ( const gsl_matrix *orig)const{
		gsl_matrix * result = gsl_matrix_alloc(orig->size1,orig->size2);
		gsl_matrix_memcpy ( result, orig );
		return result;
	}

double  epsilon = std::numeric_limits<double>::epsilon();
	
bool fp_nonzero(double x) const { return abs(x) > epsilon; }
	
int nnz(const gsl_vector* v) const {
	int nnz = 0;
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i))) {
			nnz++;
		}
	}
	return nnz;
}

	
int any(const gsl_vector* v) const {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i))) {
			return 1;
		}
	}
	return 0;
}
	
gsl_vector* any(const gsl_matrix* m, int dim =1) const {
	if (dim == 1) {
		gsl_vector* any_v = gsl_vector_alloc(m->size2);
		for (int i = 0; i < (int)m->size2; i++) {
			gsl_vector_const_view m_col_i = gsl_matrix_const_column(m, i);
			gsl_vector_set(any_v, i, any(&m_col_i.vector));
		}
		return any_v;
	} else if (dim == 2) {
		gsl_vector* any_v = gsl_vector_alloc(m->size1);
		for (int i = 0; i < (int)m->size1; i++) {
			gsl_vector_const_view m_row_i = gsl_matrix_const_row(m, i);
			gsl_vector_set(any_v, i, any(&m_row_i.vector));
		}
		return any_v;
	} else {
		return NULL;
	}
}

	
bool fp_zero(double x) const { return abs(x) < epsilon; }
	
int all(const gsl_vector* v) const {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_zero(gsl_vector_get(v, i))) {
			return 0;
		}
	}
	return 1;
}
	
	
gsl_vector* find(const gsl_vector* v, int n = std::numeric_limits<int>::max(),
				 const std::string& direction = "first") const {
	int n_find = nnz(v);
	if (n_find == 0 || n < 1) {
		return NULL;
	}
	gsl_vector* find_v = gsl_vector_alloc((n < n_find) ? n : n_find);
	if (direction == "first") {
		int position = 0;
		for (int i = 0; i < (int)v->size && position < (int)find_v->size; i++) {
			if (fp_nonzero(gsl_vector_get(v, i))) {
				gsl_vector_set(find_v, position, i);
				position++;
			}
		}
		return find_v;
	} else if (direction == "last") {
		int position = ((int)find_v->size) - 1;
		for (int i = ((int)v->size) - 1; i >= 0 && position >= 0; i--) {
			if (fp_nonzero(gsl_vector_get(v, i))) {
				gsl_vector_set(find_v, position, i);
				position--;
			}
		}
		return find_v;
	} else {
		gsl_vector_free(find_v);
		std::cout << "find::Returning NULL" << std::endl;
		return NULL;
	}
}

gsl_vector* sequence(int start, int end) const; 	
/*
 * Emulates (~v).
 */
gsl_vector * logical_not(const gsl_vector* v) const {
	gsl_vector* not_v = gsl_vector_alloc(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		bool z = fp_zero(gsl_vector_get(v, i));
		gsl_vector_set(not_v, i, (double)z);
	}
	return not_v;
}
	
	// Matrix-by-two-vectors indexing (non-mixed)
	
gsl_matrix* ordinal_index(const gsl_matrix* m, const gsl_vector* rows,
						  const gsl_vector* columns) const {
	gsl_matrix* index_m = gsl_matrix_alloc(rows->size, columns->size);
	for (int i = 0; i < (int)rows->size; i++) {
		int row = (int)gsl_vector_get(rows, i);
		for (int j = 0; j < (int)columns->size; j++) {
			int column = (int)gsl_vector_get(columns, j);
			double value = gsl_matrix_get(m, row, column);
			gsl_matrix_set(index_m, i, j, value);
		}
	}
	return index_m;
}
	
	
/*
 * Emulates (start:step:end).
 */
	gsl_vector* sequence(int start, int step, int end) const;
/*
 * Computes node and edge betweenness centrality for a binary graph.  Results
 * are stored in a vector (node betweenness) and a matrix (edge betweenness)
 * that are provided by the caller.
 */
int safe_mode =0 ;
	
	void node_and_edge_betweenness_bin(const gsl_matrix* sourceGraph, gsl_vector* node_betweenness,gsl_matrix* edge_betweenness) const;
};

#endif /* defined(__rebuild_graph__gslGraph__) */
