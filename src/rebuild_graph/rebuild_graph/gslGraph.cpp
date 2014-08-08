//
//  gslGraph.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 29/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include "gslGraph.h"
#include <stdexcept>
#include <iostream>
using std::runtime_error;



//-----------------------------------------------------------------------------
// PUBLIC class graph operation addVertex(newVertexId)
// This operation adds newVertexId to the graph
// RETURNS the updated value of de PRIVATE variable order
//-----------------------------------------------------------------------------
int gslGraph::addVertex(int newVertexId){
	//  int result=order;
	
	if ( order == 0){
		matrix = gsl_matrix_alloc	(newVertexId+1,newVertexId+1);
		order = newVertexId+1;
		return order;
	}
	
	if(order<(newVertexId+1)){
		gsl_matrix *new_matrix = gsl_matrix_alloc(newVertexId+1,newVertexId+1);
		for (int i = 0 ; i < matrix->size1 ; i ++){
			for ( int j =0; j < matrix->size1;  j ++){
				gsl_matrix_set (new_matrix, i,j, gsl_matrix_get(matrix,i,j));
			}
		}
		delete matrix;
		matrix = new_matrix;
		order = newVertexId+1;
	}
	return order;
}



int gslGraph::addVertexNeighbour(int sourceVertex,int newNeighbour){
	int result=-1;
	
	if(sourceVertex!=newNeighbour){
		if(order<(sourceVertex+1)){
			addVertex(sourceVertex);
		}
		if(order<(newNeighbour+1)){
			addVertex(newNeighbour);
		}
		gsl_matrix_set (matrix, sourceVertex,newNeighbour, 1);
		gsl_matrix_set (matrix, newNeighbour,sourceVertex, 1);

		//result=vertexArray[sourceVertex]->addNeighbour(newNeighbour);
		//result=vertexArray[newNeighbour]->addNeighbour(sourceVertex);
		if( getDegree(sourceVertex)>degree){
			degree++;
		}
	}
	return result;
}

GeneralGraph *gslGraph::readPythonGraphFile(char *fileName){
	
	FILE *input;
	int i=0;
	int vertex_identifier=0;
	int vertex_neighbour=0;
	char *line;
	char *endOfVertex,*newaux;
	try {
	if ( !fileName ){
		throw runtime_error("FileName : is NULL");
	}

	//	graph *result=new graph();
	line=(char *)malloc(sizeof(char)*STRING_LENGTH);
	if((input=fopen(fileName,"rt"))==NULL){
		char szPath[255];
		if (!getwd(szPath)){
			printf("Current path (%s)",szPath);
		}
		throw runtime_error("FileName : File Not Found");
	}
	line[0]='\0';
	line=fgets(line,STRING_LENGTH,input);
	while(line!=NULL){
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
		line=fgets(line,STRING_LENGTH,input);
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

void gslGraph::printGraph(){
	int i,j;
		
	//  printf("The graph has %d vertex:\n",order);
	for(i=0;i<order;i++){
		
		
		
		
		//    printf("Vertex %3d(%d neighbours):",i,auxNNeighbours);
		for(j=0;j<order;j++){
			gsl_matrix_get (matrix, i,j);
		}
		//    printf("\n");
	}
}