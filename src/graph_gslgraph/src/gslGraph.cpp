//
//  gslGraph.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 29/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include "gslGraph.hpp"
#include "CTrace.hpp"
#include <stdexcept>
#include <iostream>
#include <unistd.h>

using std::runtime_error;


#ifndef STRING_LENGTH
#define STRING_LENGTH 256
#endif


gslGraph::gslGraph():
        matrix(NULL),
        vertex_degree(NULL){
};

gslGraph::gslGraph(int sizeOfMatrix){

    matrix = gsl_matrix_calloc(sizeOfMatrix,sizeOfMatrix);
    //gsl_matrix_set_zero(matrix);
    vertex_degree = (int *)calloc(sizeOfMatrix , sizeof(int));
  
};

gslGraph::~gslGraph(){
    if( matrix )
        gsl_matrix_free(matrix);
};


gslGraph*   gslGraph::copyGraph()const{
    gslGraph *newgslGraph = new gslGraph(getOrder());

    if ( getOrder() !=  matrix->size1)
            throw runtime_error("Order and matrix size are different");
    if ( getOrder() !=  matrix->size2)
        throw runtime_error("Order and matrix size are different");


    gsl_matrix_memcpy ( newgslGraph->matrix, matrix );
    memcpy(newgslGraph->vertex_degree,vertex_degree,(sizeof(int)*getOrder()));
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
void gslGraph::addVertex(int newVertexId){

    if ( getOrder() == 0){
        if ( matrix)
            gsl_matrix_free( matrix);
        matrix = gsl_matrix_calloc  (newVertexId+1,newVertexId+1);
        this->vertex_degree = (int*)calloc(getOrder(),sizeof(int));
        return;
    }
    
    if(getOrder()<(newVertexId+1)){
        gsl_matrix *new_matrix = gsl_matrix_calloc(newVertexId+1,newVertexId+1);
        for (int i = 0 ; i < matrix->size1 ; i ++){
            for ( int j =0; j < matrix->size1;  j ++){
                gsl_matrix_set (new_matrix, i,j, gsl_matrix_get(matrix,i,j));
            }
        }
        gsl_matrix_free(matrix);
        matrix = new_matrix;
        int * new_vertex_degree = (int *) calloc((newVertexId+1), sizeof(int));
        memcpy(new_vertex_degree,vertex_degree,(newVertexId) * sizeof(int));
        free (vertex_degree);
        vertex_degree = new_vertex_degree;
    }
	
    return;
}



void gslGraph::addVertexNeighbour(int sourceVertex,int newNeighbour){
    bool sourceWasConnected = false;
    bool newNeighbourWasConnected = false;
    bool weHaveConnected = false;
    if(sourceVertex!=newNeighbour){
        if(getOrder()<(sourceVertex+1)){
            addVertex(sourceVertex);
        }
        if(getOrder()<(newNeighbour+1)){
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
        if (sourceWasConnected != newNeighbourWasConnected)
            throw "ERROR one is connected to other but not both";
    }
}


void gslGraph::removeVertexNeighbours(int vertexToRemoveNegighbours){
    int maxDegree = 0;
    if ( vertexToRemoveNegighbours < getOrder() ){
        for (int i = 0; i < getOrder() ; i++ ){
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
    }else{
        throw "ERROR: Neighbour to remove greater than order";
    }
    
}


void  gslGraph::addNewVertexNeighbour(int sourceVertex,int newNeighbour){
    CFuncTrace trace (false,"gslGraph::addNewVertexNeighbour");
    if(((getOrder()+1)>sourceVertex) &&
       ((getOrder()+1)>newNeighbour)) {
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
    for(i=0;i<getOrder();i++){
        if(getDegree(i)==0){
            result=true;
            *unconnectedVertex=i;
            //printf("NOT CONNECTED\n");//printGraph();
            break;
        }
    }
    return result;
}

int  gslGraph::graphToGsl( gsl_matrix* target){
    gsl_matrix_memcpy(target,matrix);
    return 1;
}



