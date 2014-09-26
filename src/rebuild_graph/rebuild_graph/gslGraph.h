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
#include "GeneralGraph.h"
#include <matrix/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_linalg.h>
#define RESULT_OK 1

class CRebuildGraph;

class gslGraph : public GeneralGraph {
private:
	int order;
	int degree;
	gsl_matrix * matrix;
	
public:
	gslGraph():order(0),degree(0),matrix(NULL)
	{
		nType = GSL_GRAPH;
	};
	gslGraph(int sizeOfMatrix):order(sizeOfMatrix),degree(0)
	{
		nType = GSL_GRAPH;
		matrix = gsl_matrix_alloc(sizeOfMatrix,sizeOfMatrix);
	};
	virtual GeneralGraph *readPythonGraphFile(char *fileName);
	// Get the number of vertex (order) of the graph
	virtual int getOrder() const { return order;};
	
	
	
    // Get the graph degree
	virtual int  getDegree() const {return degree;};
	virtual int getDegree(int vertex)const {
		int vertexDegree = 0;
		for ( int i =0;i < order; i++)
			if ( gsl_matrix_get(matrix,vertex,i)){
				vertexDegree++;
			}
		return vertexDegree;
	}
	
	// Add a vertex with the given newVertexId identifier to the graph
    int addVertex(int newVertexId);
	int addVertexNeighbour(int sourceVertex,int newNeighbour);
	void printGraph();
	virtual int graphToGsl( gsl_matrix* target){
		gsl_matrix_memcpy(target,matrix);
		return 1;
	}
	virtual void setAllVertexNeighbours(){};
	virtual ~gslGraph(){};
	virtual void brandes_comunicability_centrality_exp(double *myCExp)
	{
		
		int graphOrder=getOrder();
		// Get Numpy Matrix // Matriu d'adjacencia
				//	lFuncTrace.trace("\nPrinting Home made Matrix\n");
		//	printGslMatrix(A1," %g");
		gsl_matrix *A1expm=gsl_matrix_alloc(graphOrder,graphOrder);
		
		gsl_linalg_exponential_ss(matrix
								, A1expm, .01);
		//	lFuncTrace.trace("Printing ExpmMatrix");
		//	printGslMatrix(A1expm);
		
		gsl_vector * gslvDiagonal = getDiagonalFromGslMatrix(A1expm);
		
		//	lFuncTrace.trace("Printing Diagonal From ExpmMatrix");
		//	printGslVector(gslvDiagonal);
		
		gslVectorToArray(gslvDiagonal,myCExp);
		
	};
	gsl_vector *
	getDiagonalFromGslMatrix(const gsl_matrix * gslMatrix){
		
		int nMatrixOrder = (int) gslMatrix->size1;
		gsl_vector * gslvDiagonal = gsl_vector_alloc(nMatrixOrder);
		
		for (int i=0; i < nMatrixOrder;i++){
			gsl_vector_set(gslvDiagonal,i,gsl_matrix_get(gslMatrix,i,i));
		}
		
		return gslvDiagonal;
	}
	int gslVectorToArray(gsl_vector* gslVector, double* arrayDoubles)
	{
		
		for (size_t i = 0; i < gslVector->size; i++) {
			arrayDoubles[i]=  gsl_vector_get(gslVector, i);
			
		}
		return RESULT_OK;
	}
	virtual gslGraph*	copyGraph(){
		gslGraph *newgslGraph = new gslGraph(order);
		newgslGraph->order = order;
		newgslGraph->degree = degree;
		gsl_matrix_memcpy ( newgslGraph->matrix, matrix );
		return newgslGraph;
	}
	
	virtual void copyGraph(gslGraph * newgslGraph)
	{
		newgslGraph = copyGraph();
		return;
		newgslGraph->order = order;
		newgslGraph->degree = degree;
		if ( newgslGraph->matrix)
			delete newgslGraph->matrix;
		gsl_matrix_memcpy ( newgslGraph->matrix, matrix );
	}
	
	virtual void removeVertexNeighbours(int vertexToRemoveNegighbours){
		if ( vertexToRemoveNegighbours < order )
		{
			for (int i = 0; i < order ; i++ ){
				
				gsl_matrix_set(matrix,vertexToRemoveNegighbours,i,0);
				gsl_matrix_set(matrix,i,vertexToRemoveNegighbours,0);
				

				
			}
			/*
				gsl_matrix *new_matrix=gsl_matrix_alloc(order -1,order -1);
				for ( int i = 0; i < order ; i ++)
				{
					if (i < vertexToRemoveNegighbours ) {
						for (int j = 0; j < order ; j++ ){
							if ( j < vertexToRemoveNegighbours){
								if (gsl_matrix_get(matrix, i, j)){
									gsl_matrix_set(new_matrix,i,j,1);
								}
							}else if (j > vertexToRemoveNegighbours ){
								if (gsl_matrix_get(matrix, i, j)){
									gsl_matrix_set(new_matrix,i,j-1,1);
								}
							}
						}
								
					}else if (i > vertexToRemoveNegighbours){
						for (int j = 0; j < order ; j++ ){
							if ( j < vertexToRemoveNegighbours){
								if (gsl_matrix_get(matrix, i, j)){
									gsl_matrix_set(new_matrix,i-1,j,1);
								}
							}else if (j > vertexToRemoveNegighbours ){
								if (gsl_matrix_get(matrix, i, j)){
									gsl_matrix_set(new_matrix,i-1,j-1,1);
								}
							}
						}
						
					}
				}
			delete matrix;
			matrix = new_matrix;
			*/
		}
	}
	virtual int vertexAreNeighbours(int vertexBegining,int vertexEnding){
		return gsl_matrix_get(matrix, vertexBegining, vertexEnding);
	}
	virtual int graphNotConnected (int *unconnectedVertex){
		int i,result=false;
		for(i=0;i<order;i++){
			if(getDegree(i)==0){
				result=true;
				*unconnectedVertex=i;
				//printf("NOT CONNECTED\n");//printGraph();
				break;
			}
		}
		return result;
	}
	virtual void addNewVertexNeighbour(int sourceVertex,int newNeighbour){
		if(((order+1)>sourceVertex) &&
		   ((order+1)>newNeighbour)) {
			gsl_matrix_set(matrix,sourceVertex,newNeighbour,1);
		}
		else{
			printf("Error source ore newNeigbour out of bounds");
		}
			
	
	}
	virtual void brandes_betweenness_centrality(double *myBBC){
		
		gsl_vector * v =betweenness_bin(matrix);
		gslVectorToArray(v,myBBC);
		for(int i=0;i< order;i++){
			myBBC[i]=(myBBC[i])/((order-1.0)*(order-2.0));
			std::cout << " BC " << myBBC[i];
		}
	}

   
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <limits>

	


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
gsl_vector* betweenness_bin(const gsl_matrix* sourceGraph) {
	gsl_vector* betweenness = gsl_vector_alloc(sourceGraph->size1);
	gsl_matrix* betweeness_edge = gsl_matrix_calloc(sourceGraph->size1, sourceGraph->size2);
//	node_and_edge_betweenness_bin(sourceGraph, NULL ,betweeness_edge);
//	printGslMatrix(betweeness_edge);
	
	node_and_edge_betweenness_bin(sourceGraph, betweenness, NULL );
	
	
	
	return betweenness;
}

	gsl_matrix * copy ( const gsl_matrix *orig){
		gsl_matrix * result = gsl_matrix_alloc(orig->size1,orig->size2);
		gsl_matrix_memcpy ( result, orig );
		return result;
	}

double  epsilon = std::numeric_limits<double>::epsilon();
	
bool fp_nonzero(double x) { return abs(x) > epsilon; }
	
int nnz(const gsl_vector* v) {
	int nnz = 0;
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i))) {
			nnz++;
		}
	}
	return nnz;
}

	
int any(const gsl_vector* v) {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i))) {
			return 1;
		}
	}
	return 0;
}
	
gsl_vector*any(const gsl_matrix* m, int dim =1) {
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

	
bool fp_zero(double x) { return abs(x) < epsilon; }
	
int all(const gsl_vector* v) {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_zero(gsl_vector_get(v, i))) {
			return 0;
		}
	}
	return 1;
}
	
	
gsl_vector* find(const gsl_vector* v, int n = std::numeric_limits<int>::max(), const std::string& direction = "first") {
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
			int position = find_v->size - 1;
			for (int i = v->size - 1; i >= 0 && position >= 0; i--) {
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

	gsl_vector* sequence(int start, int end) {
		return sequence(start, 1, end);
	}

	
/*
 * Emulates (~v).
 */
gsl_vector * logical_not(const gsl_vector* v) {
	gsl_vector* not_v = gsl_vector_alloc(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		bool z = fp_zero(gsl_vector_get(v, i));
		gsl_vector_set(not_v, i, (double)z);
	}
	return not_v;
}
	
	// Matrix-by-two-vectors indexing (non-mixed)
	
gsl_matrix* ordinal_index(const gsl_matrix* m, const gsl_vector* rows, const gsl_vector* columns) {
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
gsl_vector* sequence(int start, int step, int end) {
	int n_seq = (end - start) / step + 1;
	if (n_seq <= 0) {
		return NULL;
	}
	gsl_vector* seq_v = gsl_vector_alloc(n_seq);
	for (int i = 0, value = start; i < n_seq; i++, value += step) {
		gsl_vector_set(seq_v, i, value);
	}
	return seq_v;
}

/*
 * Computes node and edge betweenness centrality for a binary graph.  Results
 * are stored in a vector (node betweenness) and a matrix (edge betweenness)
 * that are provided by the caller.
 */
int safe_mode =0 ;
void node_and_edge_betweenness_bin(const gsl_matrix* sourceGraph, gsl_vector* node_betweenness,gsl_matrix* edge_betweenness) {
//	if (safe_mode) check_status(m, BINARY, "node_and_edge_betweenness_bin");
	if (sourceGraph->size1 != sourceGraph->size2) {
		//throw size_exception();
	}
	bool free_node_betweenness = false;
	bool free_edge_betweenness = false;
	
	// BC=zeros(n,1);
	if (node_betweenness == NULL) {
		free_node_betweenness = true;
		node_betweenness = gsl_vector_calloc(sourceGraph->size1);
	} else {
		gsl_vector_set_zero(node_betweenness);
	}
	if (edge_betweenness == NULL) {
		free_edge_betweenness = true;
		edge_betweenness = gsl_matrix_calloc(sourceGraph->size1, sourceGraph->size2);
	} else {
		gsl_matrix_set_zero(edge_betweenness);
	}
	
	
	// for u=1:n
	for (int u = 0; u < (int)sourceGraph->size1; u++) {
		
		
		gsl_vector* d = gsl_vector_calloc(sourceGraph->size1);
		gsl_vector_set(d, u, 1.0);
		
		// NP=zeros(1,n); NP(u)=1;
		gsl_vector* sigma = gsl_vector_calloc(sourceGraph->size1);
		gsl_vector_set(sigma, u, 1.0);
		
		// P=false(n);
		gsl_matrix* p = gsl_matrix_calloc(sourceGraph->size1, sourceGraph->size2);
		
		// Q=zeros(1,n); q=n;
		gsl_vector* Q = gsl_vector_calloc(sourceGraph->size1);
		int q = (int)sourceGraph->size1 - 1;
		
		gsl_matrix* copy_sourceGraph = copy(sourceGraph);
		
		// V=u;
		gsl_vector* V = gsl_vector_alloc(1);
		gsl_vector_set(V, 0, u);
		
		// while V
		while (V != NULL) {
			
			// Gu(:,V)=0;
			for (int V_index = 0; V_index < (int)V->size; V_index++) {
				int v = (int)gsl_vector_get(V, V_index);
				gsl_vector_view copy_m_column = gsl_matrix_column(copy_sourceGraph, v);
				gsl_vector_set_zero(&copy_m_column.vector);
			}
			
			// for v=V
			for (int V_index = 0; V_index < (int)V->size; V_index++) {
				int v = (int)gsl_vector_get(V, V_index);
				gsl_vector_set(Q, q--, v);
				
				// W=find(Gu(v,:));
				gsl_vector_view copy_m_row = gsl_matrix_row(copy_sourceGraph, v);
				gsl_vector* W = find(&copy_m_row.vector);
				
				// For all the neighbours of v
				for (int W_index = 0; W != NULL && W_index < (int)W->size; W_index++) {
					int w = (int)gsl_vector_get(W, W_index);
					// shortest path to w via v?
					if (fp_nonzero(gsl_vector_get(d, w))) {
						
						double sigma_w = gsl_vector_get(sigma, w);
						double sigma_v = gsl_vector_get(sigma, v);
						gsl_vector_set(sigma, w, sigma_w + sigma_v);
						
						gsl_matrix_set(p, w, v, 1.0);
						
						// else
					} else {
						
						gsl_vector_set(d, w, 1.0);
						gsl_vector_set(sigma, w, gsl_vector_get(sigma, v));
						gsl_matrix_set(p, w, v, 1.0);
					}
				}
				if (W != NULL) {
					gsl_vector_free(W);
				}
			}
			
			// V=find(any(Gu(V,:),1));
			gsl_vector* column_indices = sequence(0, sourceGraph->size2 - 1);
			gsl_matrix* copy_m_rows = ordinal_index(copy_sourceGraph, V, column_indices);
			gsl_vector* any_copy_m_rows = any(copy_m_rows);
			gsl_vector_free(V);
			V = find(any_copy_m_rows);
			gsl_vector_free(any_copy_m_rows);
			gsl_matrix_free(copy_m_rows);
			gsl_vector_free(column_indices);
		}
		
		// if ~all(D)
		if (all(d) == 0) {
			std::cout << "All the distances are 0" << std::endl;
			// Q(1:q)=find(~D);
			gsl_vector* not_d = logical_not(d);
			gsl_vector* not_d_indices = find(not_d);
			gsl_vector_view Q_upto_q = gsl_vector_subvector(Q, 0, q + 1);
			gsl_vector_memcpy(&Q_upto_q.vector, not_d_indices);
			gsl_vector_free(not_d_indices);
			gsl_vector_free(not_d);
		}
		
		// S returns vertices in order of non-increasing distance from s
		gsl_vector* delta = gsl_vector_calloc(sourceGraph->size1);
		
		// for w=Q(1:n-1);
		for (int Q_index = 0; Q_index < (int)sourceGraph->size1 - 1; Q_index++) {
		
			int w = (int)gsl_vector_get(Q, Q_index);
			// BC(w)=BC(w)+DP(w)
			double bcw = gsl_vector_get(node_betweenness, w);
			double delta_w = gsl_vector_get(delta, w);
			gsl_vector_set(node_betweenness, w, bcw + delta_w);
			
			// for v=find(P(w,:))
			gsl_vector_view p_row = gsl_matrix_row(p, w);
			gsl_vector* found_p_row = find(&p_row.vector);
//			printGslMatrix(p);
			for (int p_index = 0; found_p_row != NULL && p_index < (int)found_p_row->size; p_index++) {
				int v = (int)gsl_vector_get(found_p_row, p_index);
//				std::cout << " v " << v << "Is connected with w" << w << std::endl;
				// DPvw=(1+DP(w)).*NP(v)./NP(w);
				double sigma_v = gsl_vector_get(sigma, v);
				double sigma_w = gsl_vector_get(sigma, w);
				double dpvw = (1 + delta_w) * sigma_v / sigma_w;
//				std::cout << " sigma_v " << sigma_v << "sigma_w" << sigma_w << std::endl;
				// DP(v)=DP(v)+DPvw;
				double dpv = gsl_vector_get(delta, v);
				gsl_vector_set(delta, v, dpv + dpvw);
				double ebcvw = gsl_matrix_get(edge_betweenness, v, w);
				
				gsl_matrix_set(edge_betweenness, v, w, ebcvw + dpvw);
				
			}
			if (found_p_row != NULL) {
				gsl_vector_free(found_p_row);
			}
		}
		
		gsl_vector_free(delta);
		gsl_matrix_free(copy_sourceGraph);
		gsl_vector_free(Q);
		gsl_matrix_free(p);
		gsl_vector_free(sigma);
		gsl_vector_free(d);
	}
	if (free_node_betweenness) {
		gsl_vector_free(node_betweenness);
	}
	if (free_edge_betweenness) {
		gsl_matrix_free(edge_betweenness);
	}
	
}

};

#endif /* defined(__rebuild_graph__gslGraph__) */
