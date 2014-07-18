////////////////////////////////////////////////////////////////////////////////
//////////                         GRAPHS.CPP                         //////////
////////////////////////////////////////////////////////////////////////////////

// Including standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "CTrace.hpp"
#include <gsl/gsl_matrix.h>  
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_linalg.h>
#include "graphs.h"
#include "CSettingsSumulation.h"

// Defining constants

// Just for convenience


#define PYTHON  1
#define ADJLIST 2
#define BETWEENNESS 3







////////////////////////////////////////////////////////////////////////////////
//////////                     VERTEX OPERATIONS                      //////////
////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// PUBLIC class vertex operation addNeighbour(newNeighbourId)
// This operation:
//  -adds newNeighbourId to the list of neighbours(if needed)
//  -updates the value of the PRIVATE variable degree
// RETURNS the updated value of de PRIVATE variable degree
//-----------------------------------------------------------------------------
int graph::vertex::addNeighbour(int newNeighbourId){
  int i;
	
  int found=false;

  if(vertexId!=newNeighbourId){
    if(degree==0){
      if(neighbours==NULL){
        neighbours=(int *)malloc(sizeof(int));
      } else {
        neighbours=(int *)realloc(neighbours,sizeof(int));
      }
      neighbours[0]=newNeighbourId;
      degree=1;
    } else {
      for(i=0;i<degree;i++){
        if(neighbours[i]==newNeighbourId){
          found=true;
          break;
        }
      }
      if(found==false){
        if(neighbours==NULL){
          neighbours=(int *)malloc(sizeof(int)*(degree+1));
        } else {
          neighbours=(int *)realloc(neighbours,sizeof(int)*(degree+1));
        }
        neighbours[degree]=newNeighbourId;
        degree++;
      }
    }
  }
  return degree;
}



//-----------------------------------------------------------------------------
// PUBLIC class vertex operation removeNeighbour(oldNeighbourId)
// This operation:
//  -removes oldNeighbourId from the list of neighbours(if found)
//  -updates the value of the PRIVATE variable degree
// RETURNS the updated value of de PRIVATE variable degree
//-----------------------------------------------------------------------------
int graph::vertex::removeNeighbour(int oldNeighbourId){
  int i;
  int found=false;

  if((vertexId!=oldNeighbourId)&&(degree>0)){
    for(i=0;i<degree;i++){
      if(neighbours[i]==oldNeighbourId){
        found=true;
      }
      if(found &&((i+1)<degree)){
        neighbours[i]=neighbours[i+1];
      }
    }
    if(found){
      degree--;
    }
  }
  return degree;
}


//-----------------------------------------------------------------------------
// PUBLIC class vertex operation getNeighbours(numberOfMyNeighbours)
// This operation:
// -sets the value of the given parameter numberOfMyNeighbours to the value
//    of the PRIVATE variable degree
// RETURNS a copy of the array of vertex identifiers with vertex's neighbours
//-----------------------------------------------------------------------------
int *graph::vertex::getNeighbours(int * numberOfMyNeighbours){
  *numberOfMyNeighbours=degree;
  return getNeighbours();
}

//-----------------------------------------------------------------------------
// PUBLIC class vertex operation getNeighbours(myNeighbours)
// This operation:
// -makes a copy of the array of vertex identifiers with vertex's neighbours
//    into the given parameter myNeighbours
// RETURNS the value of de PRIVATE variable degree
//-----------------------------------------------------------------------------
int graph::vertex::getNeighbours(int ** myNeighbours){
  *myNeighbours=getNeighbours();
  return degree;
}

//-----------------------------------------------------------------------------
// PUBLIC class vertex operation getNeighbours()
// RETURNS a copy of the array of vertex identifiers with vertex's neighbours
//-----------------------------------------------------------------------------
int *graph::vertex::getNeighbours(){
  int *result=NULL;

  if(degree!=0){
    result=(int *)malloc(sizeof(int)*degree);
    for(int i=0;i<degree;i++){
      result[i]=neighbours[i];
    }
  }
  return result;
}

//-----------------------------------------------------------------------------
// PUBLIC class vertex operation getNeighbours(myNeighbours)
// This operation:
// -makes a copy of the array of vertex identifiers with vertex's neighbours
//    into the given parameter myNeighbours
// RETURNS the value of de PRIVATE variable degree
//-----------------------------------------------------------------------------
void graph::vertex::getAllNeighbours(int *myListOfNeighbours[]){
  (*myListOfNeighbours)[0]=degree;
  for(int i=0;i<degree;i++){
    (*myListOfNeighbours)[i+1]=neighbours[i];
  }
}

//-----------------------------------------------------------------------------
// PUBLIC class vertex operation getNeighbours()
// RETURNS a copy of the array of vertex identifiers with vertex's neighbours
//-----------------------------------------------------------------------------
void graph::vertex::setAllNeighbours(int *myListOfNeighbours){

  if(myListOfNeighbours==NULL){
    degree=0;
  } else {
    degree=myListOfNeighbours[0];
    for(int i=0;i<degree;i++){
      neighbours[i]=myListOfNeighbours[i+1];
    }
  }
}

//-----------------------------------------------------------------------------
// PUBLIC class vertex operation getNeighbours()
// RETURNS a copy of the array of vertex identifiers with vertex's neighbours
//-----------------------------------------------------------------------------
void graph::vertex::setAllNeighbours(int myOrder){
  int i;
  int aux[myOrder];

  if(neighbours==NULL){
    neighbours=(int *)malloc(sizeof(int)*myOrder);
  } else {
    for(i=0;i<degree;i++){
      aux[i]=neighbours[i];
    }
    free(neighbours);
    neighbours=(int *)malloc(sizeof(int)*myOrder);
    for(i=0;i<degree;i++){
      neighbours[i]=aux[i];
    }
    for(i=degree;i<myOrder;i++){
      neighbours[i]=myOrder;
    }
  }
}

//-----------------------------------------------------------------------------
// PUBLIC class vertex operation vertexIsMyNeighbour()
// RETURNS
//   true if vertexSearched is neighbour of the vertex executing the operation
//   false otherwise
//-----------------------------------------------------------------------------
int graph::vertex::vertexIsMyNeighbour(int vertexSearched){
  int result=false;
  int i;

  if(degree!=0){
    for(i=0;i<degree;i++){
      result=result || neighbours[i]==vertexSearched;
    }
  }
  return result;
}


//-----------------------------------------------------------------------------
// PUBLIC class vertex operation getVertexBC()
// RETURNS the value of the PRIVATE variable vertexBBC
//-----------------------------------------------------------------------------
double graph::vertex::getVertexBC(){
  return vertexBBC;
}

//-----------------------------------------------------------------------------
// PUBLIC class vertex operation setVertexBC()
// SETS the value of the PRIVATE variable vertexBBC
// RETURNS the value of the PRIVATE variable vertexBBC
//-----------------------------------------------------------------------------
double graph::vertex::setVertexBC(double newBC){
  vertexBBC=newBC;
  return vertexBBC;
}

//-----------------------------------------------------------------------------
// PUBLIC class vertex operation addVertexBC()
// Adds the given value to the PRIVATE variable vertexC
// RETURNS the value of the PRIVATE variable vertexBBC
//-----------------------------------------------------------------------------
double graph::vertex::addVertexBC(double newBC){
  vertexBBC+=newBC;
  return vertexBBC;
}


//-----------------------------------------------------------------------------
// PUBLIC class graph operation getOrder()
// RETURNS the value of de PRIVATE variable order
//-----------------------------------------------------------------------------
int graph::getOrder() const{
  return order;
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation addVertex(newVertexId)
// This operation adds newVertexId to the graph
// RETURNS the updated value of de PRIVATE variable order
//-----------------------------------------------------------------------------
int graph::addVertex(int newVertexId){
//  int result=order;

  if(order<(newVertexId+1)){
    if(vertexArray==NULL){
      vertexArray=(vertex**)malloc(sizeof(vertex*)*(newVertexId+1));
    } else {
      vertexArray=(vertex**)realloc(
        vertexArray,sizeof(vertex*)*(newVertexId+1));
    }
    for(int i=order;i<(newVertexId+1);i++){
      vertexArray[i]=(vertex *)new vertex(i);
    }
    order=newVertexId+1;
  }
  return order;
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation addVertexNeighbour(sourceVertex,neighbour)
// This operation adds neighbour to vertex sourceVertex list of neighbours
// RETURNS the updated value of de PRIVATE variable order
//-----------------------------------------------------------------------------
int graph::addVertexNeighbour(int sourceVertex,int newNeighbour){
  int result=-1;

  if(sourceVertex!=newNeighbour){
    if(order<(sourceVertex+1)){
      addVertex(sourceVertex);
    }
    if(order<(newNeighbour+1)){
      addVertex(newNeighbour);
    }
    result=vertexArray[sourceVertex]->addNeighbour(newNeighbour);
    result=vertexArray[newNeighbour]->addNeighbour(sourceVertex);
    if(vertexArray[sourceVertex]->degree>degree){
      degree++;
    }
  }
  return result;
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation addNewVertexNeighbour(sourceVertex,neighbour)
// This operation adds neighbour to vertex sourceVertex list of neighbours
//-----------------------------------------------------------------------------
void graph::addNewVertexNeighbour(int sourceVertex,int newNeighbour){
  if(((order+1)>sourceVertex) &&
     ((order+1)>newNeighbour)) {

    vertex *myVertex=vertexArray[sourceVertex];
    int myDegree=myVertex->degree;
    int i;
    int found=false;
    vertex *myNeighbour=vertexArray[newNeighbour];
    int myNeighboursDegree=myNeighbour->degree;

    if(myDegree<(order-1)){
      for(i=0;i<myDegree;i++){
        if(myVertex->neighbours[i]==newNeighbour){
          found=true;
        }
      }
      if(!found){
        for(i=0;i<myNeighbour->degree;i++){
          if(myNeighbour->neighbours[i]==sourceVertex){
            found=true;
            printf("OJO\n");
          }
        }
        if(!found){
          myVertex->neighbours[myDegree++]=newNeighbour;
          myVertex->degree=myDegree;
          if(myDegree>degree){
            degree=myDegree;
          }
          myNeighbour->neighbours[myNeighboursDegree]=sourceVertex;
          myNeighboursDegree++;
          myNeighbour->degree=myNeighboursDegree;
          if(myNeighboursDegree>degree){
            degree=myNeighboursDegree;
          }
        }
      }
    }
//    myVertex=NULL;
//    myNeighbour=NULL;
  }
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation addVertexNeighbours(sourceVertex,neighbours)
// This operation adds neighbour to vertex sourceVertex list of neighbours
// RETURNS the updated value of de PRIVATE variable order
//-----------------------------------------------------------------------------
int graph::addVertexNeighbours(int sourceVertex,int *myNeighbours,int
myDegree){
  int i,result=-1;

  if(myDegree>0){
    for(i=0;i<myDegree;i++){
      result=vertexArray[sourceVertex]->addNeighbour(myNeighbours[i]);
      result=vertexArray[myNeighbours[i]]->addNeighbour(sourceVertex);
    }
    if(myDegree>degree){
      degree=myDegree;
    }
  }
  return result;
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation removeVertexNeighbours(sourceVertex)
// RETURNS the updated value of de PRIVATE variable order
//-----------------------------------------------------------------------------
void graph::removeVertexNeighbours(int sourceVertex){
  int i,j;
  vertex *myVertex,*myNeighbour;
  int found=false;
  int myNeighboursDegree;

  if(order>sourceVertex && vertexArray[sourceVertex]!=NULL){
    myVertex=vertexArray[sourceVertex];
    for(i=0;i<myVertex->degree;i++){
      myNeighbour=vertexArray[myVertex->neighbours[i]];
      myNeighboursDegree=myNeighbour->degree;
      found=false;
      for(j=0;j<myNeighboursDegree;j++){
        if(myNeighbour->neighbours[j]==sourceVertex){
          found=true;
          myNeighbour->degree--;
        }
        if(found){
          myNeighbour->neighbours[j]=myNeighbour->neighbours[j+1];
        }
      }
    }
    myVertex->degree=0;
  }
  myVertex=NULL;
  myNeighbour=NULL;
}


// Get a vertex list of neighbours
void graph::setAllVertexNeighbours(int myVertex,int *myListOfNeighbours){
  vertexArray[myVertex]->setAllNeighbours(myListOfNeighbours);
}


// Get a vertex list of neighbours
void graph::setAllVertexNeighbours(){
  for(int i=0;i<order;i++){
    vertexArray[i]->setAllNeighbours(order);
  }
}


// Get myVertex list of neighbours
int graph::getVertexNeighbours(int myVertex,int **myListOfNeighbours){
  return vertexArray[myVertex]->getNeighbours(myListOfNeighbours);
}


// Get the vertex degree
int graph::getDegree(int sourceVertex){
  return vertexArray[sourceVertex]->degree;
}


//-----------------------------------------------------------------------------
// PUBLIC class graph operation vertexDistance(vertexBegining,vertexEnding)
// RETURNS the length of any shortest path from vertexBegining to vertexEnding
//-----------------------------------------------------------------------------
int graph::vertexDistance(int vertexBegining,int vertexEnding){
  int result=order;
  if(distanceMatrix!=NULL){
    result=distanceMatrix[vertexBegining][vertexEnding];
  }
  return result;
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation vertexDistance(vertexBegining,vertexEnding)
// RETURNS the length of any shortest path from vertexBegining to vertexEnding
//-----------------------------------------------------------------------------
int graph::vertexAreNeighbours(int vertexBegining,int vertexEnding){
  return distanceMatrix[vertexBegining][vertexEnding]<2;
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation getDegree()
// RETURNS the value of de PRIVATE variable degree
//-----------------------------------------------------------------------------
int graph::getDegree(){
  return degree;
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation getDistanceMatrix()
// RETURNS a copy of de PRIVATE variable distanceMatrix
//-----------------------------------------------------------------------------
int **graph::getDistanceMatrix(){
  int i,j;
  int **result;

  result=(int **)malloc(sizeof(int)*order*order);
  for(i=0;i<order;i++){
    result[i]=(int *)malloc(sizeof(int)*order*order);
  }
  for(i=0;i<order;i++){
    result[i][i]=0;
    for(j=i+1;j<order;j++){
      result[i][j]=distanceMatrix[i][j];
      result[j][i]=distanceMatrix[j][i];
    }
  }
  return result;
}



//-----------------------------------------------------------------------------
// PUBLIC class graph operation updateDistanceMatrix()
// RETURNS true
//-----------------------------------------------------------------------------
int graph::updateDistanceMatrix(){
  int repeat=1,i,j,l;
  int *aux;

  if(distanceMatrix!=NULL){
    for(i=0;i<order;i++){
      free(distanceMatrix[i]);
    }
    free(distanceMatrix);
  }
  distanceMatrix=(int **)malloc(sizeof(int *)*order);
  for(i=0;i<order;i++){
    distanceMatrix[i]=(int*)malloc(sizeof(int)*order);
  }
  // We begin setting the matrix's diagonal to 0,as d(i,i)=0 for any vertex i
  for(i=0;i<order;i++){
    distanceMatrix[i][i]=0;
  }
  // As the d matrix is symetric,we only loop half of the matrix
  // Before having connectivity information,al vertex are unconnected
  for(i=0;i<order-1;i++){
    for(j=i+1;j<order;j++){
      distanceMatrix[i][j]=order;
      distanceMatrix[j][i]=order;
    }
  }
  // Distance to any neighbour is 1
  for(i=1;i<order;i++){
    for(j=0;j<vertexArray[i]->getNeighbours(&aux);j++){
      distanceMatrix[i][aux[j]]=1;
      distanceMatrix[aux[j]][i]=1;
    }
    free(aux);
  }
  // Now we have to lower distances
  // For each path i-j and any vertex l,if d(i,l)+d(l,j)<d(i,j)
  // then we need the setup the new value d(i,j)=d(i,l)+ d(l,j)
  // If any modification is made to the matrix,we'll have to start again
  while(repeat){
    repeat=0;
    for(i=1;i<order;i++){
      for(j=0;j<i;j++){
        for(l=0;l<order;l++){
          if((i!=l)&&(j!=l)&&(i!=j)&&
            ((distanceMatrix[i][l]+distanceMatrix[l][j])
            <distanceMatrix[i][j])){
            distanceMatrix[i][j]=distanceMatrix[i][l]+distanceMatrix[l][j];
            distanceMatrix[j][i]=distanceMatrix[i][l]+distanceMatrix[l][j];
            repeat=1 ;
          }
        }
      }
    }
  }
  return true;
}





void graph::brandes_betweenness_centrality(double *myBBC){
  int i,j,s,t,v,w;
  int S[order*order],Scount=0;
  int P[order][order],Pcount[order];
  int d[order];
  int Q[8192],Qcount=0,Qfirst=0,Qlast=0;
  int sigma[order];
  double delta[order];
  double aux;
  

  // [Ulrik Brandes' algorithm]
  // We initialize every single vertex betweenness centrality's value to 0
  for(i=0;i<order;i++){
    vertexArray[i]->setVertexBC(0.0);
  }
  // We initialize the List and the Queue
  for(i=0;i<8192;i++){
    Q[i]=-1;
  }
  for(i=0;i<order;i++){
    Pcount[i]=0;
    for(j=0;j<order;j++){
      P[i][j]=-1;
    }
  }
  // [Ulrik Brandes' algorithm] for every vertex s ...
  for(s=0;s<order;s++){
    for(t=0;t<order;t++){
      S[t]=-1;    // emptying stack
      for(i=0;i<order;i++){
        P[t][i]=-1;  // emptying list
      }
      sigma[t]=0;
      d[t]=-1;
      Pcount[t]=0;       // emptying list
    }
    Scount=0;         // emptying stack
    sigma[s]=1;
    d[s]=0;
    // emptying queue
    for(i=0;i<Qlast;i++){
      Q[i]=-1;
    }
    Qcount=0;
    Qfirst=0;
    Qlast=0;
    // queueing s
    Q[Qlast++]=s;
    Qcount++;
    while(Qcount>0){
      // dequeueing to v
      v=Q[Qfirst++];
      Qcount--;
      // pushing v to the stack
      S[Scount++]=v;
      // for each neighbour w of v
//printf("3BBC dequeueing and pushing V=%d\n",v);
/*
      for(j=0;j<vertexArray[v]->getDegree();j++){
      printf("BBC1\n");
      aux2=vertexArray[v]->getNeighbours();
      printf("BBC2\n");
      w=aux2[j];
      printf("BBC3\n");
      free(aux2);
      printf("BBC4\n");
*/
      for(j=0;j<vertexArray[v]->degree;j++){
//printf("BBC1\n");
        w=vertexArray[v]->neighbours[j];
        // If we have not yet found the d to vertex w
        // we need to enqueue vertex w and add the d to vertex w
        if(d[w]<0){
          Q[Qlast++]=w;
          Qcount++;
//printf("3BBC queueing W=%d\n",w);
          d[w]=d[v]+1;
        }
        // If the path length is minimal
        if(d[w]==(d[v]+1)){
          sigma[w]=sigma[w]+sigma[v];
          P[w][Pcount[w]++]=v;
        }
      }
    }
    for(v=0;v<order;v++){
      delta[v]=0.0;
    }
    while(Scount>0){
      w=S[--Scount];
      for(j=0;j<Pcount[w];j++){
        v=P[w][j];
        aux=delta[v];
        delta[v]=aux
            +(((double)(sigma[v])/(double)(sigma[w]))*(1.0+(double)(delta[w])));
      }
      if(w!=s){
        vertexArray[w]->addVertexBC(delta[w]);
      }
    }
  }
  for(i=0;i<order;i++){
    myBBC[i]=(vertexArray[i]->getVertexBC())/((order-1.0)*(order-2.0));
    vertexArray[i]->setVertexBC(myBBC[i]);
  }
}



int graph::graphNotConnected(int *unconnectedVertex){
  int i,result=false;
  for(i=0;i<order;i++){
    if(vertexArray[i]->degree==0){
      result=true;
      *unconnectedVertex=i;
//printf("NOT CONNECTED\n");//printGraph();
      break;
    }
  }
  return result;
}




void graph::printGraph(){
  int i,j;
  int auxNNeighbours;
  int *auxNeighbours;

//  printf("The graph has %d vertex:\n",order);
  for(i=0;i<order;i++){
    auxNNeighbours=vertexArray[i]->getNeighbours(&auxNeighbours);
//    printf("Vertex %3d(%d neighbours):",i,auxNNeighbours);
    for(j=0;j<auxNNeighbours;j++){
//      printf(" %d",auxNeighbours[j]);
    }
//    printf("\n");
    free(auxNeighbours);
  }
}

void graph::printGraph(FILE *myFile){
  int i,j;
  int auxNNeighbours;
  int *auxNeighbours;

  fprintf(myFile,"The graph has %d vertex:\n",order);
  for(i=0;i<order;i++){
    auxNNeighbours=vertexArray[i]->getNeighbours(&auxNeighbours);
    fprintf(myFile,"Vertex %3d(%d neighbours):",i,auxNNeighbours);
    for(j=0;j<auxNNeighbours;j++){
      fprintf(myFile,"%d ",auxNeighbours[j]);
    }
    fprintf(myFile,"\n");
    free(auxNeighbours);
  }
}


//Write a grapth
// In the format
// 0 is connected with 1 and 2
// 3 is connected with 1
//∫
// 1 2
// 0 3
// 2
// 1
void graph::printMyGraph(const char * outputGraphFilename){
	
	FILE *outputGraph;
	int i,j;
	int auxNNeighbours;
	int *auxNeighbours;

	outputGraph=fopen(outputGraphFilename,"w");
	if(outputGraph==NULL){
			printf("Cannot open out graph file %s for writting\n",outputGraphFilename);
			exit(-1);
	}

	for(i=0;i<order;i++){
		auxNNeighbours=vertexArray[i]->getNeighbours(&auxNeighbours);
		for(j=0;j<(auxNNeighbours-1);j++){
		  fprintf(outputGraph,"%d ",auxNeighbours[j]);
		}
		fprintf(outputGraph,"%d\n",auxNeighbours[auxNNeighbours-1]);
		free(auxNeighbours);
	}
	fclose(outputGraph);
}


typedef struct {
  double value;
  int position;
}myBC;


