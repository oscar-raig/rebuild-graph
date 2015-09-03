//
//  readPythonGraphFile.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 20/7/15.
//  Copyright (c) 2015 Oscar Raig Colon. All rights reserved.
//

#include <unistd.h>
#include "readPythonGraphFile.h"
#include <stdexcept>


gslGraph *ReadPythonGraphFile::readPythonGraphFile( std::string fileName){
    
    FILE *input;
    int i=0;
    int vertex_identifier=0;
    int vertex_neighbour=0;
    char *line;
    char *endOfVertex,*newaux;
    gslGraph * graph = new gslGraph();
    try {
        if ( !fileName.c_str() ){
            throw runtime_error("FileName : is NULL");
        }
        
        //  graph *result=new graph();
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
                graph->addVertex(vertex_identifier);
                newaux=endOfVertex;
                do{
                    endOfVertex=newaux;
                    vertex_neighbour=(int)strtol(endOfVertex,&newaux,10);
                    if(newaux-endOfVertex!=0){
                        graph->addVertexNeighbour(vertex_identifier,vertex_neighbour);
                    }
                } while(endOfVertex!=newaux);
            }
            line[0]='\0';
            lineReadResult=fgets(line,STRING_LENGTH,input);
        }
        fclose(input);
        free(line);
        
        return graph;
    }
    catch ( std::exception &e){
        std::cout << e.what() <<std::endl;
        throw;
    }
}

