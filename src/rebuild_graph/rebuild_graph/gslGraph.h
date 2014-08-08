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
		gslGraph *newgslGraph = new gslGraph();
		newgslGraph->order = order;
		newgslGraph->degree = degree;
		gsl_matrix_memcpy ( newgslGraph->matrix, matrix );
		return newgslGraph;
	}
	
	virtual void copyGraph(gslGraph * newgslGraph)
	{
	//	*newgslGraph = copyGraph();
		
		newgslGraph->order = order;
		newgslGraph->degree = degree;
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
		
	}
};
   

#endif /* defined(__rebuild_graph__gslGraph__) */
