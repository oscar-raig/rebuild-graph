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
        matrix(NULL),
        vertex_degree(NULL){
};

gslGraph::gslGraph(int sizeOfMatrix):
        order(sizeOfMatrix),
        degree(0){

    matrix = gsl_matrix_calloc(sizeOfMatrix,sizeOfMatrix);
    //gsl_matrix_set_zero(matrix);
    vertex_degree = (int *)malloc(sizeOfMatrix * sizeof(int));
    memset(vertex_degree,0,sizeOfMatrix * sizeof(int));
};

gslGraph::~gslGraph(){
    if( matrix )
        gsl_matrix_free(matrix);
};


gslGraph*   gslGraph::copyGraph()const{
    gslGraph *newgslGraph = new gslGraph(order);

    if ( order !=  matrix->size1)
            throw runtime_error("Order and matrix size are different");
    if ( order !=  matrix->size2)
        throw runtime_error("Order and matrix size are different");

    newgslGraph->order = getOrder();
    newgslGraph->degree = getDegree();
    gsl_matrix_memcpy ( newgslGraph->matrix, matrix );
    memcpy(newgslGraph->vertex_degree,vertex_degree,(sizeof(int)*order));
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
                    fprintf(outputGraph,"%d ",(int)j    );
            }
            if ( gsl_matrix_get(matrix, i, j) )
                fprintf(outputGraph,"%d ",(int)j    );
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
        matrix = gsl_matrix_calloc  (newVertexId+1,newVertexId+1);
        order = newVertexId+1;
        degree = 0;
        this->vertex_degree = (int*)malloc(order*sizeof(int));
        memset(this->vertex_degree,0,order*sizeof(int));
        return order;
    }
    
    if(order<(newVertexId+1)){
        gsl_matrix *new_matrix = gsl_matrix_calloc(newVertexId+1,newVertexId+1);
        for (int i = 0 ; i < matrix->size1 ; i ++){
            for ( int j =0; j < matrix->size1;  j ++){
                gsl_matrix_set (new_matrix, i,j, gsl_matrix_get(matrix,i,j));
            }
        }
        gsl_matrix_free(matrix);
        matrix = new_matrix;
        int * new_vertex_degree = (int *) malloc((newVertexId+1) * sizeof(int));
        memset(new_vertex_degree,0,(newVertexId+1) * sizeof(int));
        memcpy(new_vertex_degree,vertex_degree,order * sizeof(int));
        order = newVertexId+1;
        free (vertex_degree);
        vertex_degree =new_vertex_degree;
        degree = getDegree();
    }
    return order;
}



int gslGraph::addVertexNeighbour(int sourceVertex,int newNeighbour){
    int result=1;
    bool sourceWasConnected = false;
    bool newNeighbourWasConnected = false;
    bool weHaveConnected = false;
    
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
        
        double *element = gsl_matrix_ptr(matrix,sourceVertex,newNeighbour);
        if ( !*element ){
            *element= 1;
            weHaveConnected = true;
            if ( this->vertex_degree )
                this->vertex_degree[sourceVertex] = this->vertex_degree[sourceVertex]+ 1;
            else
                throw "ERROR vertex_degree is null";
            
        }else{
            sourceWasConnected = true;  
            
        }
        
        element = gsl_matrix_ptr(matrix, newNeighbour,sourceVertex);
        if ( !*element ){
            *element = 1;
            weHaveConnected = true;
            this->vertex_degree[newNeighbour] = this->vertex_degree[newNeighbour] +1;
        }else{
            newNeighbourWasConnected = true;
        }
        if ( weHaveConnected ){
            if ( degree < this->vertex_degree[newNeighbour] )
                degree = this->vertex_degree[newNeighbour];
            if ( degree < this->vertex_degree[sourceVertex] )
                degree = this->vertex_degree[sourceVertex];
        }
        if (sourceWasConnected != newNeighbourWasConnected)
            throw "ERROR one is connected to other but not both";
    }
    return result;
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










void gslGraph::removeVertexNeighbours(int vertexToRemoveNegighbours){
    int maxDegree = 0;
    if ( vertexToRemoveNegighbours < order ){
        for (int i = 0; i < order ; i++ ){
            double *element = gsl_matrix_ptr(matrix,vertexToRemoveNegighbours,i );
            if ( *element){
                *element= 0;
                this->vertex_degree[vertexToRemoveNegighbours]--;
            }
            element = gsl_matrix_ptr(matrix,i,vertexToRemoveNegighbours);
            if (*element){
                *element = 0;
                this->vertex_degree[i]--;
            }
            if( vertexToRemoveNegighbours != i){
                if ( maxDegree < this->vertex_degree[i])
                    maxDegree = this->vertex_degree[i];
            }
        }
        degree = maxDegree;
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
        double *element = gsl_matrix_ptr(matrix,sourceVertex,newNeighbour );
        if ( !*element){
            *element = 1;
            vertex_degree[sourceVertex]++;
        }
        element = gsl_matrix_ptr(matrix,newNeighbour,sourceVertex );
        if ( !*element){
            *element = 1;
            vertex_degree[newNeighbour]++;
        }
        if(this->degree < vertex_degree[newNeighbour])
            degree = vertex_degree[newNeighbour];
        if(this->degree < vertex_degree[sourceVertex])
            degree = vertex_degree[sourceVertex];

            
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
