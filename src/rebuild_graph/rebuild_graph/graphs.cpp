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
#define STRING_LENGTH 256

#define PYTHON  1
#define ADJLIST 2
#define BETWEENNESS 3

// Park-Miller pseudo random  number generator
#define LLAVOR_X 11
#define LLAVOR_Y 92
#define LLAVOR_Z 37

// Simulated Annealing
#define TO 10
#define TMIN 0.0001
#define NMAX 1000
#define K 0.9
#define TOL 0.0001
#define MAX_ITERATIONS 1000

// GLOBAL VARIABLES
int random_value_x;
int random_value_y;
int random_value_z;


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

  printf("The graph has %d vertex:\n",order);
  for(i=0;i<order;i++){
    auxNNeighbours=vertexArray[i]->getNeighbours(&auxNeighbours);
    printf("Vertex %3d(%d neighbours):",i,auxNNeighbours);
    for(j=0;j<auxNNeighbours;j++){
      printf(" %d",auxNeighbours[j]);
    }
    printf("\n");
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



//-----------------------------------------------------------------------------
// GLOBAL operation readPythonGraphFile(char *fileName)
// RETURNS a graph pointer
//-----------------------------------------------------------------------------
graph *readPythonGraphFile(char *fileName){
  FILE *input;
  int i=0;
  int vertex_identifier=0;
  int vertex_neighbour=0;
  char *line;
  char *aux,*newaux;

  graph *result=new graph();
  line=(char *)malloc(sizeof(char)*STRING_LENGTH);
  if((input=fopen(fileName,"rt"))==NULL){
      printf("file not found %s\n",fileName);
      char szPath[255];
	  if (!getwd(szPath)){
		  printf("Current path (%s)",szPath);
	  }
	  exit(1);
  }
  line[0]='\0';
  line=fgets(line,STRING_LENGTH,input);
  while(line!=NULL){
    if(line!=NULL && line[0]!='#'){
      vertex_identifier=(int)strtol(&line[i],&aux,10);
      result->addVertex(vertex_identifier);
      newaux=aux;
      do{
        aux=newaux;
        vertex_neighbour=(int)strtol(aux,&newaux,10);
        if(newaux-aux!=0){
          result->addVertexNeighbour(vertex_identifier,vertex_neighbour);
        }
      } while(aux!=newaux);
    }
    line[0]='\0';
    line=fgets(line,STRING_LENGTH,input);
  }
  fclose(input);
  free(line);
  result->updateDistanceMatrix();
  return result;
}

//-----------------------------------------------------------------------------
// GLOBAL operation generateRandomNumber()
// Wichmann-Hill method to generate random numbers (needs 3 seed numbers)
// RETURNS a random double [0,1(
//-----------------------------------------------------------------------------
double generateRandomNumber(){

  double temp;
  random_value_x=171*(random_value_x%177)-2*(random_value_x/177);
  if(random_value_x<0)
    random_value_x+=30269;
  random_value_y=172*(random_value_y%176)-35*(random_value_y/176);
  if(random_value_y<0)
    random_value_y+=30307;
  random_value_z=170*(random_value_z%178)-63*(random_value_z/178);
  if(random_value_z<0)
    random_value_z+=30323;
  temp=random_value_x/30269.0+random_value_y/30307.0+random_value_z/30323.0;
  return(temp-(int)temp);
}


//-----------------------------------------------------------------------------
// GLOBAL operation generateInitialGraph(int sourceGraphOrder)
// RETURNS a random graph with sourceGraphOrder vertex
//-----------------------------------------------------------------------------
graph *generateInitialGraph(int sourceGraphOrder){
  int i,j;
  int newNeighbour;
  int newDegree;
  graph *result=(graph *)new graph(sourceGraphOrder);

  for(i=0; i<sourceGraphOrder; i++){
    // The new vertex degree is to be
    //   at least 1 (the graphs needs to be connected)
    //   at most n-1 (the vertex is connected to every other vertex
    if(result->vertexArray[i]->degree==0){
      // vertex i has no neighbours yet
      newDegree=1+(int)(generateRandomNumber()*(sourceGraphOrder-1));
      // newDegre is in [1,n-1]
    } else if(result->vertexArray[i]->degree<(sourceGraphOrder-1)){
      // vertex i is connected to some other vertex
      newDegree=(int)(generateRandomNumber()*
        (sourceGraphOrder-result->vertexArray[i]->degree));
      // newDegree is in [0,n-1-degree]
    } else {
      // vertex i is already connected to all possible neighbours
      newDegree=0;
    }
    for(j=0;j<newDegree;j++){
      do{
        newNeighbour=(int)(generateRandomNumber()*(sourceGraphOrder));
        // newNeighbour is in [0,n-1]
      } while(result->vertexAreNeighbours(i,newNeighbour));
      result->addVertexNeighbour(i,newNeighbour);
    }
  }
  return result;
}


//-----------------------------------------------------------------------------
// GLOBAL operation copyGraph(sourceGraph)
// RETURNS a copy of sourceGraph
//-----------------------------------------------------------------------------
graph *copyGraph(graph *sourceGraph){
  int i,myDegree,*myNeighbours;
  int myOrder=sourceGraph->getOrder();
  graph *result=(graph *)new graph(myOrder);
  for(i=0;i<myOrder;i++){
    myDegree=sourceGraph->getVertexNeighbours(i,&myNeighbours);
    result->addVertexNeighbours(i,myNeighbours,myDegree);
    free(myNeighbours);
  }
  return result;
}

//-----------------------------------------------------------------------------
// GLOBAL operation copyGraph(sourceGraph,targetGraph)
// COPIES sourceGraph to targetGraph
//-----------------------------------------------------------------------------
void copyGraph(graph *sourceGraph,graph *targetGraph){
  int i,j,aux;
  int myOrder=sourceGraph->getOrder();

  for(i=0;i<myOrder;i++){
    aux=sourceGraph->vertexArray[i]->degree;
    targetGraph->vertexArray[i]->degree=aux;
    for(j=0;j<aux;j++){
      targetGraph->vertexArray[i]->neighbours[j]=
        sourceGraph->vertexArray[i]->neighbours[j];
    }
    for(j=i+1;j<myOrder;j++){
      aux=sourceGraph->distanceMatrix[i][j];
      targetGraph->distanceMatrix[i][j]=aux;
      targetGraph->distanceMatrix[j][i]=aux;
    }
  }

}

//-----------------------------------------------------------------------------
// GLOBAL operation modifyGraph(sourceGraph)
// MODIFIES a randon sourceGraph vertex's connections
//-----------------------------------------------------------------------------
void modifyGraph(graph *sourceGraph){
  int i,j;
  int vertex2change;
  int myOrder=sourceGraph->getOrder();
  int myNewNeighbour;
  int myNewNumberOfNeighbours;
  int newNeighbours[myOrder];
  int found;

  // Select vertex to change
  vertex2change=(int)(generateRandomNumber()*(myOrder));
  // vertex2change is in [0,n-1]
  // Disconnect vertex2change
//printf("modifyGraph\n");
//sourceGraph->printGraph();
//printf("modifyGraph remove vertex %d\n",vertex2change);
  sourceGraph->removeVertexNeighbours(vertex2change);
//sourceGraph->printGraph();
//printf("modifyGraph vertex removed\n");
  do{
  //Choose new vertex degree
    myNewNumberOfNeighbours=1+(int)(generateRandomNumber()*(myOrder-1));
    // myNewNumberOfNeighbours is in [1,n-1]
  // Connect new neighbours
    for(i=0; i<myNewNumberOfNeighbours; i++){
//printf("modifyGraph3 vertex2change %d\n",vertex2change);
//printf("modifyGraph3 Newdegree %d\n",myNewNumberOfNeighbours);
      do{
        found=false;
//printf("modifyGraph4\n");
        myNewNeighbour=(int)(generateRandomNumber()*(myOrder));
        // myNewNeighbour is in [0,n-1]
        for(j=0;j<i;j++){
//printf("modifyGraph4 new=%d newNei[j]=%d\n",myNewNeighbour,newNeighbours[j]);
          if((myNewNeighbour==newNeighbours[j])
            ||(myNewNeighbour==vertex2change)){
            found=true;
          }
        }
      } while(found||
          (sourceGraph->vertexAreNeighbours(vertex2change,myNewNeighbour)));
      newNeighbours[i]=myNewNeighbour;
//printf("modifyGraph5 vertex %d ADD neigh %d\n",vertex2change,myNewNeighbour);
      sourceGraph->addNewVertexNeighbour(vertex2change,myNewNeighbour);
    }
//    sourceGraph->printGraph();
  } while (sourceGraph->graphNotConnected(&vertex2change));
//sourceGraph->printGraph();
//printf("modifyGraph vertex reconnected\n");
}


//-----------------------------------------------------------------------------
// GLOBAL operation cost(double *tarjet,double *current,int count)
// RETURNS the cost
//-----------------------------------------------------------------------------
double cost(double *tarjet,double *current,int count){
  int i;
  double result=0.0;

  for(i=0; i<count; i++) {
    result+=pow((tarjet[i]-current[i]),2);
  }
  return pow(result,0.5);
}

void generateOutputFile(const  graph *targetGraph,const char *inputFileName,
						int lx, int ly, int lz, float To, float Tk,float Tmin,
						float k,long Nmax,double costBest,double *targetBC,
						double *bestBC, time_t timeStart, time_t timeEnd){
	
	FILE *output=NULL;
	int graphOrder =targetGraph->getOrder();
	char outputFilename[STRING_LENGTH];
	strcpy(outputFilename,inputFileName);
	strcat(outputFilename,".out");
	output=fopen(outputFilename,"w");
	if(output==NULL){
		printf("Cannot open output file %s for writting\n",outputFilename);
		exit(-1);
	}
	fprintf(output,"Graph reconstruction via  vertex betweenness centrality\n");
	fprintf(output,"\tOriginal graph description file-> %s",inputFileName);
	fprintf(output,"\t\tOrdre-> %d\n",targetGraph->getOrder());
	fprintf(output,"\tPseudorandom generator seeds-> %d,%d,%d\n",lx,ly,lz);
	fprintf(output,"SIMULATED ANNEALING:\n");
	fprintf(output,"\tTo-> %f\n",To);
	fprintf(output,"\tTk final-> %f\n",Tk);
	fprintf(output,"\tTmin-> %f\n",Tmin);
	fprintf(output,"\tGeometric cooling rate: T[k]=%f * T[k-1]\n",k);
	fprintf(output,"\tN-> %ld\n",Nmax);
	fprintf(output,"RESULTS:\n");
	fprintf(output,"\tBest cost -> %3.20f\n",costBest);
	fprintf(output,"\tBetweenness centrality\n");
	fprintf(output,"\t Desired BC  |  Closest BC  | Difference^2\n");
	for(int i=0; i<graphOrder; i++){
		fprintf(output,"\t%2.10f | ",targetBC[i]);
		fprintf(output,"%2.10f | ",bestBC[i]);
		fprintf(output,"%2.10f\n",pow(targetBC[i]-bestBC[i],2));
	}
	printf("CPU time needed: %f seconds\n",difftime(timeEnd,timeStart));
	fclose(output);

	
}

void AnnealingAlgorithm(double &Tk, int To,graph **pbestGraph,int graphOrder,
					double *bestBC,double *targetBC,  graph *oldGraph,
					FILE *logFile,double &costBest){
	double Tmin=TMIN;
	double k=K;
	int iterations=0;
	double tol=TOL;
	int weAreDone=0;
	costBest=0.0;
	double costOld=0.0;
	double costNew=0.0;
	long int Nmax=NMAX;
	long int N=0;
	graph * bestGraph= NULL;
	double newBC [graphOrder];
	graph *newGraph=NULL;
	
	// STARTING SIMMULATED ANNEALING
	Tk=To;
	bestGraph=generateInitialGraph(graphOrder);
	*pbestGraph= bestGraph;
	bestGraph->setAllVertexNeighbours();
	bestGraph->brandes_betweenness_centrality(bestBC);
	costBest=cost(targetBC,bestBC,graphOrder);
	costOld=2.0*costBest;
	costNew=costOld;
	oldGraph=copyGraph(bestGraph);
	oldGraph->setAllVertexNeighbours();
	newGraph=copyGraph(bestGraph);
	newGraph->setAllVertexNeighbours();
	
	int accept=false;
	int okTrue=0;
	int okFalse=0;
	int notOk=0;
	do{
		/* Repeat NMAX times */
		for(N=0;(N<Nmax)&&(!weAreDone);N++){
			// Slightly modify oldGraph to obtain newGraph
			//printf("ANTES NEWGRAPH\n"); newGraph->printGraph();
			//      modifyGraph(oldGraph,newGraph,getMax(oldBC,graphOrder),accept);
			//      modifyGraph(oldGraph,newGraph,bestGraph,accept);
			modifyGraph(newGraph);
			// Evaluate newGraph's vertex betweenness centrality
			newGraph->brandes_betweenness_centrality(newBC);
			// Update cost variables (new and old graphs)
			costOld=costNew;
			costNew=cost(targetBC,newBC,graphOrder);
			if(costNew<costBest){
				costBest=costNew;
				copyGraph(newGraph,bestGraph);
				memcpy(bestBC,newBC,graphOrder*sizeof(double));
				if(costBest<=tol){
					weAreDone=true;
					break;
				}
				accept=0;
				okTrue++;
				printf(".");
				fprintf(logFile,".");
			} else if(exp((costBest-costNew)/Tk)>generateRandomNumber()){
				// if newCost not is better than oldCost,
				// we still accept it if exp(df/T_k)<rand()
				accept=1;
				okFalse++;
				printf("o");
				fprintf(logFile,"o");
			} else {
				//otherwise we don't accept the new graph
				//        printf("best\n");
				//        bestGraph->printGraph();
				//        printf("new\n");
				//        newGraph->printGraph();
				copyGraph(bestGraph,newGraph);
				//        printf("best\n");
				//        bestGraph->printGraph();
				//        printf("new\n");
				//        newGraph->printGraph();
				accept=2;
				notOk++;
				printf("x");
				fprintf(logFile,"x");
			}
		}
		printf("\n");
		fprintf(logFile,"\n");
		printf("Tk=%2.15f\tBest Cost=%2.15f EXIT=%d Iterations=%d\n",
			   Tk,costBest,weAreDone,iterations);
		fprintf(logFile,"Tk=%2.15f\tBest Cost=%2.15f EXIT=%d Iterations=%d\n",
				Tk,costBest,weAreDone,iterations);
		// Lower temperature: T(k)=k*T(k-1)
		Tk*=k;
		// Update number of iterations
		iterations++;
	}while((Tk>=Tmin)&&(!weAreDone)&&(iterations!=MAX_ITERATIONS));

	
}



int
fregenerateGraph(CSettingsSimulation &settingsSimulation, double *&targetBC, double *&bestBC,int *order){

  int i=0;
  int lx=0;
  int ly=0;
  int lz=0;
  time_t timeStart;
  time_t timeEnd;

  char inputFilename[STRING_LENGTH];
  char outputGraphFilename[STRING_LENGTH];
  char inputGraphFilename[STRING_LENGTH];
  char logFilename[STRING_LENGTH];
  FILE *logFile=NULL;
 

  // Simmulated Annealing variables
  double To=TO;
  double Tmin=TMIN;
  double Tk=TO;
  long int Nmax=NMAX;
 
  double k=K;

 
  double costBest=0.0;


  graph *targetGraph=NULL;
  graph *bestGraph=NULL;
  graph *oldGraph=NULL;


  int graphOrder=0;
 
  // Default value initialization
  timeStart=time(NULL);
  random_value_x=LLAVOR_X;
  random_value_y=LLAVOR_Y;
  random_value_z=LLAVOR_Z;
  lx=random_value_x;
  ly=random_value_y;
  lz=random_value_z;

  // Substitution of default values with argument values
  printf("Reconstruction of a graph ");
  printf("from its vertex's betweenness centrality values\n");
  printf("Use: reconstruct [input_file] [P/A/B] [To] [Tmin] [Nmax] [k]");
	/*
  switch(argc){
    case  6:k=atof(argv[5]);
    case  5:Nmax=atoi(argv[4]);
    case  4:Tmin=atof(argv[3]);
    case  3:To=atof(argv[2]);
	case  2:strcpy(inputFilename,argv[1]);
    break;
    default:printf("ERROR incorrect parameters\n");
             exit(-1);
  }
	 */
	
	k = settingsSimulation.k;
	Nmax = settingsSimulation.nMax;
	Tmin = settingsSimulation.tMin;
	To = settingsSimulation.To;
	strcpy(inputFilename,settingsSimulation.inputFileName.c_str());

	targetGraph=readPythonGraphFile(inputFilename);
  

  graphOrder=targetGraph->getOrder();
  printf("si\n");

  //double targetBC [graphOrder];
  //double bestBC [graphOrder];
	targetBC =(double*) malloc(graphOrder*sizeof(double));
	bestBC=(double*) malloc(graphOrder*sizeof(double));
//	double *targetBC = *ptargetBC;
//	double *bestBC = *pbestBC;
  double oldBC [graphOrder];
  double newBC [graphOrder];
	*order = graphOrder;
	
  for(i=0;i<graphOrder;i++){
    targetBC[i]=0.0;
    bestBC[i]=0.0;
    oldBC[i]=0.0;
    newBC[i]=0.0;
  }
  targetGraph->printGraph();
     targetGraph->setAllVertexNeighbours();
    targetGraph->brandes_betweenness_centrality(targetBC);
    strcpy(inputGraphFilename,inputFilename);
    strcat(inputGraphFilename,".in");
	targetGraph->printMyGraph(inputGraphFilename);
  
  

  strcpy(outputGraphFilename,inputFilename);
  strcat(outputGraphFilename,".res");
 

  strcpy(logFilename,inputFilename);
  strcat(logFilename,".log");
  logFile=fopen(logFilename,"w");
  if(logFile==NULL){
    printf("Cannot open log file %s for writting \n",logFilename);
    exit(-1);
  }

	AnnealingAlgorithm( Tk, To,&bestGraph,graphOrder,
					   bestBC,targetBC, oldGraph,logFile,costBest);
	
  // Processing tasks accomplished
  // Showing results

  timeEnd=time(NULL);

  printf("RESULTS:\n");
  printf("CPU time needed: %f seconds\n",difftime(timeEnd,timeStart));
 // printf("Output file: %s\n",outputFilename);
	
	generateOutputFile(targetGraph,inputFilename, lx,  ly,  lz,  To,  Tk, Tmin,
											    k, Nmax, costBest,targetBC,
											   bestBC, timeStart, timeEnd);
 
	printf("\nReconstructed graph file: %s\n",outputGraphFilename);
	bestGraph->printGraph();

	bestGraph->printMyGraph(outputGraphFilename);
	
    return 1;
}


#define RESULT_OK 1

graph*
GetGraphfromFile(const char *argv[])
{
	CFuncTrace lFuncTrace(string("GetGraphfromFile"));
	graph *targetGraph=NULL;
	char inputFilename[STRING_LENGTH];
	
	if (argv[1]==NULL){
		lFuncTrace.trace("ERROR : argv[1] is NULL");
		return NULL;
	}
	strcpy(inputFilename,argv[1]);
	targetGraph=readPythonGraphFile(inputFilename);
	
	
	return targetGraph;
}

int fCalculateBeterness(const char *argv[]){
	CFuncTrace lFuncTrace("fCalculateBeterness");
	graph *targetGraph=NULL;
	int graphOrder=0;
	
	targetGraph=GetGraphfromFile(argv);
	graphOrder=targetGraph->getOrder();
	
	
	double targetBC [graphOrder];

	targetGraph->brandes_betweenness_centrality(targetBC);
	
	for (int i = 0; i < graphOrder; i++){
		lFuncTrace.trace("%2.10f\n",targetBC[i]);
	}
	
	return RESULT_OK;
}


gsl_vector_complex *
calculateEgeinval (gsl_matrix *target)
{
	
	
	int order = (int)target->size1;
	
	gsl_vector_complex *eval = gsl_vector_complex_alloc (order);
	gsl_matrix_complex *evec = gsl_matrix_complex_alloc (order, order);
	
	

	
	gsl_eigen_nonsymmv_workspace * w =
    gsl_eigen_nonsymmv_alloc (order);
	
	gsl_eigen_nonsymmv (target, eval, evec, w);
	
	gsl_eigen_nonsymmv_free (w);
	
	gsl_eigen_nonsymmv_sort (eval, evec,
							 GSL_EIGEN_SORT_ABS_DESC);
	
	{
		int i, j;
		
		for (i = 0; i < order; i++)
		{
			gsl_complex eval_i
			= gsl_vector_complex_get (eval, i);
			gsl_vector_complex_view evec_i
			= gsl_matrix_complex_column (evec, i);
			
			printf ("eigenvalue = %g + %gi\n",
					GSL_REAL(eval_i), GSL_IMAG(eval_i));
			printf ("eigenvector = \n");
			for (j = 0; j < order; ++j)
			{
				/* gsl_complex z = */
				gsl_vector_complex_get(&evec_i.vector, j);
//				printf("%g + %gi\n", GSL_REAL(z), GSL_IMAG(z));
			}
		}
	}
	
//	gsl_vector_complex_free(eval);
	gsl_matrix_complex_free(evec);
	
	return eval;
}

int
graphToGsl(graph * source, gsl_matrix* target){
	
	int i,j;
	gsl_matrix_set_zero(target);
	int graphOrder=source->getOrder();
	int auxNNeighbours;
	int *auxNeighbours;
	
	for(i=0;i<graphOrder;i++){
		auxNNeighbours=source->vertexArray[i]->getNeighbours(&auxNeighbours);
//		printf("Vertex %3d(%d neighbours):",i,auxNNeighbours);
		for(j=0;j<auxNNeighbours;j++){
//			printf(" %d",auxNeighbours[j]);
			 gsl_matrix_set (target, i, auxNeighbours[j], 1);
		}
		free(auxNeighbours);
	}
	return RESULT_OK;
}

int printGslMatrix(gsl_matrix* gslMatrix,const char *format="%.3f "){
	printf("\n");
	for (size_t i = 0; i < gslMatrix->size1; i++) {
		for (size_t j = 0; j < gslMatrix->size2; j++) {
			printf(format, gsl_matrix_get(gslMatrix, i, j));
		}
		
		printf("\n");
	}
	return RESULT_OK;
}

int printGslVector(gsl_vector* gslVector){
	printf("\n");
	for (size_t i = 0; i < gslVector->size; i++) {
			printf("%.03f ", gsl_vector_get(gslVector, i));
		printf("\n");
	}
	return RESULT_OK;
}


gsl_vector *
calculateExp(const gsl_vector_complex *eval){
	CFuncTrace lFuncTrace("calculateExp");
	int order = (int)eval->size;
	
	lFuncTrace.trace("Ordre for Expo %d",order);
	
	gsl_vector *evalexp = gsl_vector_alloc (order);
	for ( int i = 0; i < order; i++){
		gsl_complex eval_i
		= gsl_vector_complex_get (eval, i);
		
		
//		printf ("eigenvalue = %g + %gi\n",
//				GSL_REAL(eval_i), GSL_IMAG(eval_i));
		gsl_vector_set(evalexp,i,gsl_sf_exp(GSL_REAL(eval_i)));
		
		printf("W exp %g\n",gsl_sf_exp(GSL_REAL(eval_i)));
													
	}
	return evalexp;
}

int
fCalculateCommunicability(const char *argv[]){
	CFuncTrace lFuncTrace("fCalculateConnectivity");
	
	graph *targetGraph=NULL;
	targetGraph=GetGraphfromFile(argv);
	int graphOrder=targetGraph->getOrder();
	lFuncTrace.trace("Graph Order %d",graphOrder);
	
	// Get Numpy Matrix // Matriu d'adjacencia
	gsl_matrix *A1=gsl_matrix_alloc(graphOrder,graphOrder);
	
	targetGraph->printGraph();
	
	graphToGsl(targetGraph,A1);
	
	lFuncTrace.trace("Printing Home made Matrix\n");
	printGslMatrix(A1);
	lFuncTrace.trace("Printing with gsl function\n");
	gsl_matrix_fprintf(stdout, A1, "%g");
	
	gsl_vector_complex *eval = calculateEgeinval(A1);
	
	/*gsl_vector *evalexp = */ calculateExp(eval);

	return RESULT_OK;
}

gsl_vector * GetDiagonalFromGslMatrix(const gsl_matrix * gslMatrix){
	
	int nMatrixOrder = (int) gslMatrix->size1;
	gsl_vector * gslvDiagonal = gsl_vector_alloc(nMatrixOrder);
	
	for (int i=0; i < nMatrixOrder;i++){
		gsl_vector_set(gslvDiagonal,i,gsl_matrix_get(gslMatrix,i,i));
	}
	
	return gslvDiagonal;
}

int
fCalculateCommunicability_cent_exp(const char *argv[]){
	CFuncTrace lFuncTrace("fCalculateCommunicability_cent_exp");
	
	graph *targetGraph=NULL;
	targetGraph=GetGraphfromFile(argv);
	int graphOrder=targetGraph->getOrder();
	lFuncTrace.trace("Graph Order %d",graphOrder);
	
	// Get Numpy Matrix // Matriu d'adjacencia
	gsl_matrix *A1=gsl_matrix_alloc(graphOrder,graphOrder);
	
	targetGraph->printGraph();
	
	graphToGsl(targetGraph,A1);
	lFuncTrace.trace("\nPrinting Home made Matrix\n");
	printGslMatrix(A1," %g");
	gsl_matrix *A1expm=gsl_matrix_alloc(graphOrder,graphOrder);
	
	gsl_linalg_exponential_ss(A1, A1expm, .01);
	lFuncTrace.trace("Printing ExpmMatrix");
	printGslMatrix(A1expm);
	
	gsl_vector * gslvDiagonal = GetDiagonalFromGslMatrix(A1expm);
	
	lFuncTrace.trace("Printing Diagonal From ExpmMatrix");
	printGslVector(gslvDiagonal);
	return RESULT_OK;
	
}


