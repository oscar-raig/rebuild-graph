//
//  gslGraph.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 29/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include "gslGraph.h"
#include "CTrace.hpp"
#include <stdexcept>
#include <iostream>
#include <unistd.h>

using std::runtime_error;


#ifndef STRING_LENGTH
#define STRING_LENGTH 256
#endif


gslGraph::gslGraph():
		order(0),
		degree(0),
		matrix(NULL){
};

gslGraph::gslGraph(int sizeOfMatrix):
		order(sizeOfMatrix),
		degree(0){

	matrix = gsl_matrix_alloc(sizeOfMatrix,sizeOfMatrix);
	gsl_matrix_set_zero(matrix);
};

gslGraph::~gslGraph(){
	if( matrix )
		gsl_matrix_free(matrix);
};


gslGraph*	gslGraph::copyGraph()const{
	gslGraph *newgslGraph = new gslGraph(order);

	if ( order !=  matrix->size1)
			throw runtime_error("Order and matrix size are different");
	if ( order !=  matrix->size2)
		throw runtime_error("Order and matrix size are different");

	newgslGraph->order = getOrder();
	newgslGraph->degree = getDegree();
	gsl_matrix_memcpy ( newgslGraph->matrix, matrix );
	return newgslGraph;
}



void gslGraph::printMyGraph(const char * outputGraphFilename,
							bool outputAsAdjacencyList
							)const{
	CFuncTrace trace (false,"printMyGraph");
	FILE *outputGraph = NULL;
	outputGraph=fopen(outputGraphFilename,"w");
	if(outputGraph==NULL){
		printf("Cannot open out graph file %s for writting\n",outputGraphFilename);
		exit(-1);
	}
	if ( outputAsAdjacencyList ){
		for (size_t i = 0; i < matrix->size1; i++) {
			size_t j = 0;
			fprintf(outputGraph,"%d ",(int)i);
			for (j = i; j < matrix->size2-1; j++) {
				if ( gsl_matrix_get(matrix, i, j) )
					fprintf(outputGraph,"%d ",(int)j	);
			}
			if ( gsl_matrix_get(matrix, i, j) )
				fprintf(outputGraph,"%d ",(int)j	);
			fprintf(outputGraph,"\n");
		}
	}
	else{
		for (size_t i = 0; i < matrix->size1; i++) {
			size_t j = 0;
			for (j = 0; j < matrix->size2-1; j++) {
				fprintf(outputGraph,"%d ",(int)
						gsl_matrix_get(matrix, i, j));
			}
			fprintf(outputGraph,"%d\n",(int)gsl_matrix_get(matrix, i, j));
		}
	}
	fclose(outputGraph);
}


//-----------------------------------------------------------------------------
// PUBLIC class graph operation addVertex(newVertexId)
// This operation adds newVertexId to the graph
// RETURNS the updated value of de PRIVATE variable order
//-----------------------------------------------------------------------------
int gslGraph::addVertex(int newVertexId){
	
	if ( order == 0){
		if ( matrix)
			gsl_matrix_free( matrix);
		matrix = gsl_matrix_alloc	(newVertexId+1,newVertexId+1);
		order = newVertexId+1;
		degree = 0;
		return order;
	}
	
	if(order<(newVertexId+1)){
		gsl_matrix *new_matrix = gsl_matrix_alloc(newVertexId+1,newVertexId+1);
		for (int i = 0 ; i < matrix->size1 ; i ++){
			for ( int j =0; j < matrix->size1;  j ++){
				gsl_matrix_set (new_matrix, i,j, gsl_matrix_get(matrix,i,j));
			}
		}
		gsl_matrix_free(matrix);
		matrix = new_matrix;
		order = newVertexId+1;
		degree = getDegree();
	}
	return order;
}



int gslGraph::addVertexNeighbour(int sourceVertex,int newNeighbour){
	int result=1;
	
	if(sourceVertex!=newNeighbour){
		if(order<(sourceVertex+1)){
			addVertex(sourceVertex);
		}
		if(order<(newNeighbour+1)){
			addVertex(newNeighbour);
		}
		if ( sourceVertex> getOrder())
			throw "ERROR:SourceVertex is greate than order";
		if ( newNeighbour> getOrder())
			throw "ERROR: newNeighbour is greate than order";
		gsl_matrix_set (matrix, sourceVertex,newNeighbour, 1);
		gsl_matrix_set (matrix, newNeighbour,sourceVertex, 1);
		degree = getDegree();
		
	}
	return result;
}

gslGraph *gslGraph::readPythonGraphFile( std::string fileName){
	
	FILE *input;
	int i=0;
	int vertex_identifier=0;
	int vertex_neighbour=0;
	char *line;
	char *endOfVertex,*newaux;
	try {
	if ( !fileName.c_str() ){
		throw runtime_error("FileName : is NULL");
	}

	//	graph *result=new graph();
	line=(char *)malloc(sizeof(char)*STRING_LENGTH);
	if((input=fopen(fileName.c_str(),"rt"))==NULL){
		char szPath[255];
		if (!getwd(szPath)){
			printf("Current path (%s)",szPath);
		}
		throw runtime_error("FileName : File Not Found" + fileName);
	}
	line[0]='\0';
	char * lineReadResult = NULL;
	lineReadResult=fgets(line,STRING_LENGTH,input);
	while(lineReadResult!=NULL){
		if(line!=NULL && line[0]!='#'){
			vertex_identifier=(int)strtol(&line[i],&endOfVertex,10);
			addVertex(vertex_identifier);
			newaux=endOfVertex;
			do{
				endOfVertex=newaux;
				vertex_neighbour=(int)strtol(endOfVertex,&newaux,10);
				if(newaux-endOfVertex!=0){
					addVertexNeighbour(vertex_identifier,vertex_neighbour);
				}
			} while(endOfVertex!=newaux);
		}
		line[0]='\0';
		lineReadResult=fgets(line,STRING_LENGTH,input);
	}
	fclose(input);
	free(line);
	
	return this;
	}
	catch ( std::exception &e){
		std::cout << e.what() <<std::endl;
		throw;
	}
}

void gslGraph::printGraph(int TRACE_LEVEL){
	int i,j;
	CFuncTrace trace(false,"gslGraph::printGraph");
	
	trace.trace(TRACE_LEVEL,"The graph has %d order %d degree:\n",order,degree);
	for(i=0;i<order;i++){
		trace.trace(TRACE_LEVEL,"Vertex %d",i);
		for(j=0;j<order;j++){
			if (gsl_matrix_get (matrix, i,j)) {
				trace.trace(TRACE_LEVEL," %d ",j);
			}
		}
		  	trace.trace(TRACE_LEVEL,"\n");
	}
}





// Get the graph degree
int  gslGraph::getDegree() const {
	int local_degree = -1;
	for (size_t i = 0; i < matrix->size1; i++) {
		int degree_j = 0;
		for (size_t j = 0; j < matrix->size2; j++) {
			
			
			if (gsl_matrix_get(matrix, i, j) )
				degree_j++;
		}
		if (degree_j> local_degree)
			local_degree = degree_j;
	}
	return local_degree;
};

int gslGraph::getDegree(int vertex)const {
	int vertexDegree = 0;
	for ( int i =0;i < order; i++)
		if ( gsl_matrix_get(matrix,vertex,i)){
			vertexDegree++;
		}
	return vertexDegree;
}


void gslGraph::removeVertexNeighbours(int vertexToRemoveNegighbours){
	if ( vertexToRemoveNegighbours < order ){
		for (int i = 0; i < order ; i++ ){
			
			gsl_matrix_set(matrix,vertexToRemoveNegighbours,i,0);
			gsl_matrix_set(matrix,i,vertexToRemoveNegighbours,0);
			
			
			
		}
		this->degree = getDegree();
	}else{
		throw "ERROR: Neighbour to remove greater than order";
	}
	
}





gsl_vector *
 gslGraph::getDiagonalFromGslMatrix(const gsl_matrix * gslMatrix){
	
	int nMatrixOrder = (int) gslMatrix->size1;
	gsl_vector * gslvDiagonal = gsl_vector_alloc(nMatrixOrder);
	
	for (int i=0; i < nMatrixOrder;i++){
		gsl_vector_set(gslvDiagonal,i,gsl_matrix_get(gslMatrix,i,i));
	}
	
	return gslvDiagonal;
}

int  gslGraph::gslVectorToArray(gsl_vector* gslVector, double* arrayDoubles)
{
	for (size_t i = 0; i < gslVector->size; i++) {
		arrayDoubles[i]=  gsl_vector_get(gslVector, i);
	}
	return RESULT_OK;
}



void  gslGraph::addNewVertexNeighbour(int sourceVertex,int newNeighbour){
	CFuncTrace trace (false,"gslGraph::addNewVertexNeighbour");
	if(((order+1)>sourceVertex) &&
	   ((order+1)>newNeighbour)) {
		gsl_matrix_set(matrix,sourceVertex,newNeighbour,1);
		gsl_matrix_set(matrix,newNeighbour,sourceVertex,1);
		this->degree = getDegree();
	}
	else{
		trace.trace(ERROR_GSL_GRAPH,"Error: source ore newNeigbour out of bounds");
	}
}


int gslGraph::vertexAreNeighbours(int vertexBegining,int vertexEnding){
	if (vertexBegining == vertexEnding)
		return 1;
	return gsl_matrix_get(matrix, vertexBegining, vertexEnding);
}

int gslGraph::graphNotConnected (int *unconnectedVertex){
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





int gslGraph::printGslMatrix(const gsl_matrix* gslMatrix,const char *format){
	printf("\n");
	for (size_t i = 0; i < gslMatrix->size1; i++) {
		for (size_t j = 0; j < gslMatrix->size2; j++) {
			printf(format, gsl_matrix_get(gslMatrix, i, j));
		}
		
		printf("\n");
	}
	return RESULT_OK;
}








int  gslGraph::graphToGsl( gsl_matrix* target){
	gsl_matrix_memcpy(target,matrix);
	return 1;
}


int gslGraph::compare(gslGraph * graph1, gslGraph * graph2){
	
	if ( graph1 == NULL) {
		throw runtime_error("gsl_graph:compare: bad first graph for comparing");
	}
	
	if ( graph1->getOrder() !=graph2->getOrder())
		return 0;
	if ( graph1->getDegree() !=graph2->getDegree())
		return 0;
	
	if ( graph1->matrix->size1 != graph2->matrix->size1)
		return 0;
	if ( graph1->matrix->size2 != graph2->matrix->size2)
		return 0;
	
	for ( int i =0; i < graph1->matrix->size1;i++)
		for ( int j =0; j < graph1->matrix->size2;j++)
				if ( gsl_matrix_get(graph1->matrix,i,j) != gsl_matrix_get(graph2->matrix,i,j))
					return 0;
	
	return 1;
}
