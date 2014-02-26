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
//////////                        GRAPH CLASS                         //////////
////////////////////////////////////////////////////////////////////////////////

class graph{
  public:

    ////////////////////////////////////////////////////////////////////////////
    ////////                        VERTEX CLASS                        ////////
    ////////////////////////////////////////////////////////////////////////////

    class vertex{

      public:

        int degree;          // vertex number of neighbours
        int *neighbours;     // vertex array of neighbours

      //========================================================================
      // class vertex constructors
      //------------------------------------------------------------------------
      // Class vertex has 4 private variables to be initialize on creation
      //  vertexId                If not given,an impossible value is set(-1)
      //  degree                  Upon creation,a vertex has no neighbours
      //  neighbours              Upon creation,the list of neighbours is empty
      //  vertexBBC               If not given,an impossible value is set(-1)
      //========================================================================

      //========================================================================
      // vertex constructor(no parameters are given)
      //------------------------------------------------------------------------
      //  vertexId                Not given. Set to -1
      //  degree                  Set to 0
      //  neighbours              The list of neighbours is empty
      //  vertexBBC               Not given. Set to -1
      //========================================================================
        vertex():
          vertexId(-1),
          degree(0),
          neighbours(NULL),
          vertexBBC(-1.0){
        }

      //========================================================================
      // vertex constructor(newVertexId vertex identifier is given)
      //------------------------------------------------------------------------
      //  vertexId                Set to newVertexId
      //  degree                  Set to 0
      //  neighbours              The list of neighbours is empty
      //  vertexBBC               Not given. Set to -1
      //========================================================================
        vertex(int newVertexId):
          vertexId(newVertexId),
          degree(0),
          neighbours(NULL),
          vertexBBC(-1.0){
        }

      //========================================================================
      // class vertex destructor
      //------------------------------------------------------------------------
      // Class vertex's variable neighbours needs to free memory alocation
      //========================================================================
        ~vertex(){
          if(neighbours!=NULL){
            free(neighbours);
          }
        }

      //------------------------------------------------------------------------
      // class vertex operations
      //------------------------------------------------------------------------

        // Get the vertex identifier
        int getVertexId();

        // Set the vertex identifier to newVertexId
        int setVertexId(int newVertexId);

        // Get the number of neighbours of the vertex
        int getDegree();

        // Add vertex newNeighbourId to the vertex list of neighbours
        int addNeighbour(int newNeighbourId);

        // Add vertex newNeighbourId to the vertex list of neighbours
        int addNewNeighbour(int newNeighbourId);

        // Remove vertex oldNeighbourId from the vertex list of neighbours
        int removeNeighbour(int oldNeighbourId);

        // Remove all neighbours from the vertex list
        int removeNeighbours();

        // Get the vertex list of neighbours
        int *getNeighbours(int *myNumberOfNeighbours);

        // Get the vertex list of neighbours
        int getNeighbours(int **listOfNeighbours);

        // Get the vertex list of neighbours
        int *getNeighbours();

        // Get the vertex list of neighbours
        void setAllNeighbours(int myOrder);

        // Get the vertex list of neighbours
        void setAllNeighbours(int *myListOfNeighbours);

        // Get the vertex list of neighbours
        void getAllNeighbours(int *myListOfNeighbours[]);

        // Is vertexSearched one of the vertex neighbours?
        int vertexIsMyNeighbour(int vertexSearched);

        // Get the vertex betweenness centrality
        double getVertexBC();

        // Set the vertex betweenness Centrality
        double setVertexBC(double newBC);

        // Add the given value to the vertex betweenness Centrality value
        double addVertexBC(double newBC);

      protected:
        int vertexId;        // vertex identifier
        double vertexBBC;    // vertex(Brandes)Betweenness Centrality
    };

    // Array of vertex
    vertex **vertexArray;

    // Distance matrix
    int ** distanceMatrix;

    //==========================================================================
    // class graph constructors
    //--------------------------------------------------------------------------
    // Class graph has 4 private variables to initialize on creation
    //  order                    Upon creation, a graph has no vertex
    //  vertexArray              Upon creation, the list of neighbours is empty
    //  degree                   Upon creation, the graph's degree is ZERO
    //  distanceMatrix           Upon creation, there is no d matrix
    //==========================================================================

    //==========================================================================
    // graph constructor(no parameters are given)
    //--------------------------------------------------------------------------
    //  order                    Upon creation, the graph has no vertex
    //  vertexArray              Upon creation, the list of neighbours is empty
    //  degree                   Upon creation, the graph's degree is ZERO
    //  distanceMatrix           Upon creation, there is no d matrix
    //==========================================================================
    graph():
      order(0),
      vertexArray(NULL),
      degree(0),
      distanceMatrix(NULL) {
    }

    //==========================================================================
    // graph constructor(number of vertex given)
    //--------------------------------------------------------------------------
    // Creates a graph with myOrder unconnected vertex
    //  order                    Set to myOrder
    //  vertexArray              Initialized as an array of myOrder vertex
    //  degree                   Upon creation, the graph's degree is ZERO
    //  distanceMatrix           myOrder x myOrder matrix with 0's & myOrder's
    //==========================================================================
    graph(int myOrder):
      order(myOrder),
      vertexArray(NULL),
      degree(0),
      distanceMatrix(NULL) {
      int i,j;
      // Allocate memory for vertexArray
      vertexArray=(vertex **)malloc(sizeof(vertex *)*order);
      // Allocate memory for distanceMatrix
      distanceMatrix=(int **)malloc(sizeof(int *)*order);
      for(i=0;i<order;i++){
        vertexArray[i]=(vertex *)new vertex(i);
        distanceMatrix[i]=(int *)malloc(sizeof(int)*order);
        distanceMatrix[i][i]=0;
      }
      for(i=0;i<order;i++){
        for(j=i+1;j<order;j++){
          distanceMatrix[i][j]=order;
          distanceMatrix[j][i]=order;
        }
      }
    }

    //==========================================================================
    // graph constructor(number of vertex and vertex array given)
    //--------------------------------------------------------------------------
    // Creates a graph with myOrder vertex and a set of connections between them
    //  order                    Set to myOrder
    //  vertexArray              Initialized as an array of myOrder vertex
    //                           each vertex is initialized with its neighbours
    //                           as described in the given newVertexArray
    //  degree                   Set to the maximum vertex degree value
    //  distanceMatrix           fully functional distance matrix
    //==========================================================================
    graph(int myOrder,int **newVertexArray):
          order(myOrder),
          vertexArray(NULL),
          degree(0),
          distanceMatrix(NULL) {
      int i,j,k,l;
      int repeat=1;
      // Allocate memory for vertexArray
      vertexArray=(vertex **)malloc(sizeof(vertex *)*order);
      // Allocate memory for distanceMatrix
      distanceMatrix=(int **)malloc(sizeof(int *)*order);
      for(i=0;i<order;i++){
        vertexArray[i]=(vertex *)new vertex(i);
        distanceMatrix[i]=(int *)malloc(sizeof(int)*order);
        distanceMatrix[i][i]=0;
      }
      for(i=0;i<order-1;i++){
        for(j=i+1;j<order;j++){
          distanceMatrix[i][j]=order;
          distanceMatrix[j][i]=order;
        }
      }
      // Initialize each vertex in the array as described in newVertexArray
      for(int i=0;i<order;i++){
        // Add all its neighbours
        vertexArray[i]->setAllNeighbours(newVertexArray[i]);
        // If needed, update the graph degree
        if(newVertexArray[i][0]>degree){
          degree++;
        }
      }
      for(int i=0;i<order;i++){
        for(int j=1;j<newVertexArray[i][0];j++){
          k=newVertexArray[i][j];
          distanceMatrix[i][k]=1;
          distanceMatrix[k][i]=1;
        }
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
    }

    //==========================================================================
    // class graph destructor
    //--------------------------------------------------------------------------
    // Class graph 2 private variables that have to free memory alocation
    //  vertexArray
    //  distanceMatrix
    //==========================================================================
    ~graph(){
      for(int i=0;i<order;i++){
        delete vertexArray[i];
        free(distanceMatrix[i]);
      }
      free(vertexArray);
      free(distanceMatrix);
    }


    ////////////////////////////////////////////////////////////////////////////
    /////                     Vertex operations                            /////
    ////////////////////////////////////////////////////////////////////////////
    /////        All vertex operations are done in a graph context         /////
    ////////////////////////////////////////////////////////////////////////////

    // Add a vertex with the given newVertexId identifier to the graph
    int addVertex(int newVertexId);

    // Add a vertex neighbour newNeighbour to vertex sourceVertex
    int addVertexNeighbour(int sourceVertex,int newNeighbour);

    // Add a vertex neighbour newNeighbour to vertex sourceVertex
    void addNewVertexNeighbour(int sourceVertex,int newNeighbour);

    // Add newDegree vertex neighbours in newNeighbours to vertex sourceVertex
    int addVertexNeighbours(int sourceVertex,int *newNeighbours,int newDegree);

    // Remove all neighbours to vertex sourceVertex
    void removeVertexNeighbours(int sourceVertex);

    // Get vertex sourceVertex array of neighbours identifiers
    int  getVertexNeighbours(int sourceVertex,int **listOfNeighbours);

    // Get vertex sourceVertex array of neighbours identifiers
    void setAllVertexNeighbours();

    // Get vertex sourceVertex array of neighbours identifiers
    void setAllVertexNeighbours(int sourceVertex,int *listOfNeighbours);

    // Get vertex sourceVertex degree
    int getDegree(int sourceVertex);

    // Get the shortest path length from vertexBegining to vertexEnding
    int vertexDistance(int vertexBegining,int vertexEnding);

    // Are vertexBegining and vertexEnding directly connected (neighbours)
    int vertexAreNeighbours(int vertexBegining,int vertexEnding);

    // Is vertexSearched in any short path from vertexBegining to vertexEnding?
    int vertexInPath(int vertexSearched,int vertexBegining,int vertexEnding);

    // Are firstVertexSearched and secondVertexSearched
    //   in the same shortest path from vertexBegining to vertexEnding?
    int vertexInSameShortestPath(
      int firstVertexSearched,int secondVertexSearched,
      int vertexBegining,int vertexEnding
   );

    // Print the graph
    void printGraph();

    // Print the graph to a file
    void printGraph(FILE *myfile);

    // Print the graph MY way to a file
    void printMyGraph(FILE *myfile);

    // Print the graph's betweenness centrality values
    void printGraphBetweenness();

    // Print the graph's betweenness centrality values
    void printGraphBetweenness(FILE *myFile);

    ////////////////////////////////////////////////////////////////////////////
    /////             Other interesting graph operations                   /////
    ////////////////////////////////////////////////////////////////////////////

    // Get the number of vertex (order) of the graph
    int getOrder();

    // Get the graph degree
    int getDegree();

    int updateDistanceMatrix();
//    int updateDistanceMatrix(int myOrder);

    // get the d matrix of the graph
    int **getDistanceMatrix();

    // Show the d matrix of the graph
    void showDistanceMatrix();

    // Vertex Betweenness Centrality specfic operations
    void vertex_betweenness(double *bc);

    // Vertex Betweenness Centrality specfic operations
//    int *vertex_betweenness(double *bc);

    // Vertex Betweenness Centrality specfic operations
    void brandes_betweenness_centrality(double *bbc);

    int graphNotConnected(int *unconnectedVertex);

  private:

    //==========================================================================
    // Class Graph private variables and operations
    //--------------------------------------------------------------------------
    //==========================================================================

    // Number of vertex
    int order;

    // Degree
    int degree;
    int maxDegree;
    int minDegree;

    int getShortPaths(int begining,int ending,int *matrix);
    int getShortPathsThroughK(
          int begining,
          int ending,
          int middlepoint,
          int *matrix
       );
};


////////////////////////////////////////////////////////////////////////////////
//////////                     VERTEX OPERATIONS                      //////////
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// PUBLIC class vertex operation getVertexId()
// RETURNS the value of de PRIVATE variable vertexId
//-----------------------------------------------------------------------------
int graph::vertex::getVertexId(){
  return vertexId;
}

//-----------------------------------------------------------------------------
// PUBLIC class vertex operation setVertexId()
// SETS the value of de PRIVATE variable vertexId to newVertexId
// RETURNS true
//-----------------------------------------------------------------------------
int graph::vertex::setVertexId(int newVertexId){
  vertexId=newVertexId;
  return true;
}

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
// PUBLIC class vertex operation addNeighbour(newNeighbourId)
// This operation:
//  -adds newNeighbourId to the list of neighbours(if needed)
//  -updates the value of the PRIVATE variable degree
// RETURNS the updated value of de PRIVATE variable degree
//-----------------------------------------------------------------------------
int graph::vertex::addNewNeighbour(int newNeighbourId){
  int i;
  int found=false;

  if(vertexId!=newNeighbourId){
    if(degree==0){
      neighbours[0]=newNeighbourId;
      degree=1;
    } else {
      for(i=0;i<degree;i++){
        if(neighbours[i]==newNeighbourId){
          found=true;
          break;
        }
      }
      if(!found){
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
// PUBLIC class vertex operation removeNeighbours()
// This operation:
//  -removes all neighbours(if any)
//  -updates the value of the PRIVATE variable degree
// RETURNS the updated value of de PRIVATE variable degree
//-----------------------------------------------------------------------------
int graph::vertex::removeNeighbours(){
  degree=0;
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
int graph::getOrder(){
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
  int sourceVertexDegree;
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
// PUBLIC class graph operation showDistanceMatrix()
// Shows the distanceMatrix
//-----------------------------------------------------------------------------
void graph::showDistanceMatrix(){
  int i,j;

  printf("\nDistance Matrix:\n");
  for(i=0;i<order;i++){
    for(j=0;j<order;j++){
      printf("%3d ",distanceMatrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation updateDistanceMatrix()
// RETURNS true
//-----------------------------------------------------------------------------
int graph::updateDistanceMatrix(){
  int repeat=1,i,j,k,l;
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

//-----------------------------------------------------------------------------
// PUBLIC class graph operation vertexInSameShortestPath(
//                              firstVertexSearched,secondVertexSearched,
//                              vertexBegining,vertexEnding)
// RETURNS
//  -true if any shortest path from vertexBegining to vertexEnding
//          goes through vertexSearched
//  -false otherwise
//-----------------------------------------------------------------------------
int graph::vertexInSameShortestPath(
           int firstVertexSearched,int secondVertexSearched,
           int vertexBegining,int vertexEnding){

  int result=(firstVertexSearched!=secondVertexSearched);

  result=result &&(firstVertexSearched!=vertexBegining);
  result=result &&(firstVertexSearched!=vertexEnding);
  result=result &&(secondVertexSearched!=vertexBegining);
  result=result &&(secondVertexSearched!=vertexEnding);
  result=result &&(vertexBegining!=vertexEnding);
  result=result &&((vertexDistance(vertexBegining,vertexEnding)==
                     (vertexDistance(vertexBegining,firstVertexSearched)+
                       vertexDistance(firstVertexSearched,secondVertexSearched)
+
                       vertexDistance(secondVertexSearched,vertexEnding)))
                    ||(vertexDistance(vertexBegining,vertexEnding)==
                     (vertexDistance(vertexBegining,secondVertexSearched)+
                       vertexDistance(secondVertexSearched,firstVertexSearched)
+
                       vertexDistance(firstVertexSearched,vertexEnding))));
  return result;
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation
//        vertexInPath(vertexSearched,vertexBegining,vertexEnding)
// RETURNS
//  -true if any shortest path from vertexBegining to vertexEnding
//          goes through vertexSearched
//  -false otherwise
//-----------------------------------------------------------------------------
int graph::vertexInPath(int vertexSearched,int vertexBegining,int
vertexEnding){
  int result=(vertexSearched!=vertexBegining);

  result=result &&(vertexSearched!=vertexEnding);
  result=result &&(vertexBegining!=vertexEnding);
  result=result &&(vertexDistance(vertexBegining,vertexEnding)==
                    (vertexDistance(vertexBegining,vertexSearched)+
                      vertexDistance(vertexSearched,vertexEnding)));
  return result;
}

int graph::getShortPaths(int begining,int ending,int *matrix){
  int i,j;
  int d=distanceMatrix[begining][ending];
  int r=0;
  int nVertexOfShortestPath=0;

  if(begining==ending){
    r=0;
  } else if(matrix[0]==(d-1)){
    if(d==1){
      r=0;
    } else{
      r=1;
    }
  } else{
    switch(d){
      case 0:
      case 1:
        r=0;
        break;
      case 2:
        for(i=1;i<=matrix[0];i++){
          if(vertexArray[begining]->vertexIsMyNeighbour(matrix[i])&&
              vertexArray[ending]->vertexIsMyNeighbour(matrix[i])){
            r++;
          }
        }
        break;
      default:
        for(i=1;i<=matrix[0];i++){
          if(vertexArray[begining]->vertexIsMyNeighbour(matrix[i])){
            nVertexOfShortestPath=matrix[0];
            int myArray[nVertexOfShortestPath];
            nVertexOfShortestPath--;
            myArray[0]=nVertexOfShortestPath;
            for(j=1;j<i;j++){
              myArray[j]=matrix[j];
            }
            for(j=i+1;j<=(nVertexOfShortestPath+1);j++){
              myArray[j-1]=matrix[j];
            }
            r+=getShortPaths(matrix[i],ending,myArray);
          }
        }
      break;
    }
  }
  return r;
}

int graph::getShortPathsThroughK(
            int begining,
            int ending,
            int middlepoint,
            int *matrix){
  int i,j;
  int d=distanceMatrix[begining][ending];
  int r=0;
  int nVertexOfShortestPath=0;

  switch(d){
    case 0:
      r=0;
      break;
    case 1:
      r=0;
      break;
    case 2:
      if(vertexArray[begining]->vertexIsMyNeighbour(middlepoint)&&
          vertexArray[ending]->vertexIsMyNeighbour(middlepoint)){
        r=1;
      } else if(begining==middlepoint){
        r=getShortPaths(begining,ending,matrix);
      } else if(ending==middlepoint){
        r=getShortPaths(begining,middlepoint,matrix);
      }
      break;
    default:
      if(begining==middlepoint){
        r=getShortPaths(begining,ending,matrix);
      } else if(ending==middlepoint){
        r=getShortPaths(begining,middlepoint,matrix);
      } else if(matrix[0]==(d-1)){
        r=1;
      } else{
        for(i=1;i<=matrix[0];i++){
          if(distanceMatrix[begining][ending]==
             distanceMatrix[begining][matrix[i]]+
             distanceMatrix[matrix[i]][ending]){
            nVertexOfShortestPath=matrix[0];
            int myArray[nVertexOfShortestPath];
            nVertexOfShortestPath--;
            myArray[0]=nVertexOfShortestPath;
            for(j=1;j<i;j++){
              myArray[j]=matrix[j];
            }
            for(j=i+1;j<=(nVertexOfShortestPath+1);j++){
              myArray[j-1]=matrix[j];
            }
            if(matrix[i]==middlepoint){
              r+=getShortPaths(middlepoint,ending,myArray);
            } else{
              r+=getShortPathsThroughK(matrix[i],ending,middlepoint,myArray);
            }
          }
        }
      break;
    }
  }
  return r;
}


//-----------------------------------------------------------------------------
// PUBLIC class graph operation vertex_betweenness()
// RETURNS an array of integers (one for each vertex in the graph)
//         corresponding to the number of short paths each vertex belongs to
//-----------------------------------------------------------------------------
void graph::vertex_betweenness(double *result){
//  double *result;
  int resultsMatrix[order][order*order];
  int i,j,k;
  int pathNumber=0;
  double aux=0;
  updateDistanceMatrix();
  for(k=0;k<order;k++){
    pathNumber=0;
    for(i=0;i<order-1;i++){
      for(j=i+1;j<order;j++){
        if(vertexInPath(k,i,j)){
          resultsMatrix[k][pathNumber]=1;
        } else{
          resultsMatrix[k][pathNumber]=0;
        }
        pathNumber++;
      }
    }
    result[k]=0.0;
  }

  for(k=0;k<order;k++){
    pathNumber=0;
    for(i=0;i<order-1;i++){
      for(j=i+1;j<order;j++){
        result[k]+=2.0*(double)resultsMatrix[k][pathNumber];
        pathNumber++;
      }
    }
  }

//  for(i=0;i<order;i++){
//    result[i]=result[i]/((order-1)*(order-2));
//  }

//  printf("\nBetweenness:\n");
//  for(i=0;i<order;i++){
//    printf("Vertex %d: %d(%f)(N=%f)(n-1)*(n-2)=%f\n",i,result[i],
//     (double)result[i]/((order-1)*(order-2)),
//     (double)order,(double)((order-1)*(order-2)));
//    aux+=(double)result[i]/((order-1)*(order-2));
//  }
//  printf("Suma de betweenness: %f\n",aux);

//  return result;
}

//-----------------------------------------------------------------------------
// PUBLIC class graph operation vertex_betweenness(double *result2)
// RETURNS an array of integers (one for each vertex in the graph)
//         corresponding to the number of short paths each vertex belongs to
//-----------------------------------------------------------------------------
/*
int *graph::vertex_betweenness(double *result2){
  int i,j,k,a,b,c;
  int resultsMatrix[order][order*order];
  int resultsMatrixK[order][order*order];
  int pathNumber=0;
  int paths[order*order];
  int r2[order*order];
  int *result=(int *)malloc(sizeof(int)*order);

  result2=(double *)realloc(result2,sizeof(double)*order);
  for(i=0;i<order*order;i++){
    r2[i]=0;
    paths[i]=-1;
  }

// We first need to assure the d matrix is updated
  updateDistanceMatrix();

// Let's see if any short path from vertex i to vertex j goes through vertex k
// We'll store the results in resultsMatrix[vertexNumer][pathNumber]
// Array result stores the number of short paths each vertex participates in
// Array r2 stores the number of vertex in any short path from i to j
  for(k=0;k<order;k++){
    pathNumber=0;
    result[k]=0;
    result2[k]=0.0;
    for(i=0;i<order;i++){
      for(j=0;j<order;j++){
        if(vertexInPath(k,i,j)){
          resultsMatrix[k][pathNumber]=1;
          resultsMatrixK[k][pathNumber]=1;
          result[k]++;
          r2[pathNumber]++;
        } else{
          resultsMatrix[k][pathNumber]=0;
          resultsMatrixK[k][pathNumber]=0;
        }
        pathNumber++;
      }
    }
  }

// Let's now count the number of possible short path from i to j
  pathNumber=0;
  for(i=0;i<order;i++){
    for(j=0;j<order;j++){
      if(paths[pathNumber]==-1){        // the path has not yet been calculated
        a=r2[pathNumber];               // a gets the number of vertex
        int myArray[a+1];               // myArray gets the path vertex IDs
        myArray[0]=a;
        c=1;
        for(k=0;k<order;k++){
          if(resultsMatrix[k][pathNumber]!=0){
            myArray[c++]=k;
          }
        }
        paths[pathNumber]=getShortPaths(i,j,myArray);
      }
      pathNumber++;
    }
  }

// Let's count the number of short paths through k
  pathNumber=0;
  for(i=0;i<order;i++){
    for(j=0;j<order;j++){
      if(paths[pathNumber]>1){
        a=r2[pathNumber];               // a gets the number of vertex
        int myArray[a+1];               // myArray gets the path vertex IDs
        myArray[0]=a;
        c=1;
        for(k=0;k<order;k++){
          if(resultsMatrix[k][pathNumber]!=0){
            myArray[c++]=k;
          }
        }
        for(k=0;k<order;k++){
          if(resultsMatrixK[k][pathNumber]>0){
            resultsMatrixK[k][pathNumber]=getShortPathsThroughK(i,j,k,myArray);
          }
        }
      }
      pathNumber++;
    }
  }

// Let's normalize the results array
  pathNumber=0;
  for(i=0;i<order;i++){
    for(j=0;j<order;j++){
      for(k=0;k<order;k++){
        if(((b=resultsMatrixK[k][pathNumber])>0)&&
          ((a=paths[pathNumber])>0)){
          result2[k]+=(((double)(b*1.0))/((double)(a*1.0)));
        }
      }
      pathNumber++;
    }
  }

  printf("Betweenness Centrality\n");
  for(i=0;i<order;i++){
    printf("Vertex %3d: %2.15f\n",
      i,result2[i]/double(((order-1.0)*(order-2.0)))
   );
  }
  return result;
}
*/

void graph::brandes_betweenness_centrality(double *myBBC){
  int i,j,k,s,t,v,w;
  int S[order*order],Scount=0;
  int P[order][order],Pcount[order];
  int d[order];
  int Q[8192],Qcount=0,Qfirst=0,Qlast=0;
  int sigma[order];
  double delta[order];
  double aux;
  int *aux2;

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


void graph::printGraphBetweenness(){
  int i;

  printf("Betweenness Centrality(Ulrik Brandes)\n");
  for(i=0;i<order;i++){
    printf("Vertex %3d: %2.19f\n",i,vertexArray[i]->getVertexBC());
  }
}

void graph::printGraphBetweenness(FILE *myFile){
  int i;

  fprintf(myFile,"Betweenness Centrality(Ulrik Brandes)\n");
  for(i=0;i<order;i++){
    fprintf(myFile,"Vertex %3d: %2.19f\n",i,vertexArray[i]->getVertexBC());
  }
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

void graph::printMyGraph(FILE *myFile){
  int i,j;
  int auxNNeighbours;
  int *auxNeighbours;

  for(i=0;i<order;i++){
    auxNNeighbours=vertexArray[i]->getNeighbours(&auxNeighbours);
    for(j=0;j<(auxNNeighbours-1);j++){
      fprintf(myFile,"%d ",auxNeighbours[j]);
    }
    fprintf(myFile,"%d\n",auxNeighbours[auxNNeighbours-1]);
    free(auxNeighbours);
  }
}


typedef struct {
  double value;
  int position;
}myBC;


/*
int readGraphFile(char *fileName,int *n_vertex,int *n_edges,int **vertex){
  FILE *input;
  int i,j,value;
  int lines=0;
  char c;

  if((input=fopen(fileName,"rt"))==NULL){
    printf("file not found");
    printf("\n");
    exit(1);
  }
  fscanf(input,"%d %d",n_vertex,n_edges);
  while(!feof(input)){
    c=fgetc(input);
    if(!feof(input)&& c=='\n'){
      lines++;
    }
  }
  fclose(input);
  if(lines!=(*n_vertex+1)){
    printf("ERROR: Incorrect number of vertex.\n");
    exit(1);
  }
  graph *g=(graph *)new graph(*n_vertex);

  // Obrim de nou el fitxer i guardem la informaci�del graf a vertex[]
  //Aprofitem la passada per generar el fitxer graus.txt

  if((input=fopen(fileName,"rt"))==NULL){
    printf("ep! no trobo el fitxer.\n");
    exit(1);
  }
  i=0;
  j=0;
  do{
    fscanf(input,"%c",&c);
  } while(c!='\n');
  do{
    if(fscanf(input,"%d",&value)==EOF)break;
    g->addVertexNeighbour(i,value);
    if(fscanf(input,"%c",&c)==EOF)break;
    if(c=='\n'){
      i++;
    }
  } while(!feof(input));
  fclose(input);
  g->printGraph();
  g->vertex_betweenness();
  return 0;
}
*/

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
    printf("file not found\n");
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
// GLOBAL operation readGraphFile(char *fileName)
// RETURNS a graph pointer
//-----------------------------------------------------------------------------
graph *readGraphFile(char *fileName){
  FILE *input;
  int i=0;
  int vertex_identifier=0;
  int vertex_neighbour=0;
  char *line;
  char *aux,*newaux;

  graph *result=new graph();
  line=(char *)malloc(sizeof(char)*STRING_LENGTH);
  if((input=fopen(fileName,"rt"))==NULL){
    printf("file not found\n");
    exit(1);
  }
  line[0]='\0';
  line=fgets(line,STRING_LENGTH,input);
  while(line!=NULL){
    if(line!=NULL && line[0]!='#'){
//      vertex_identifier=(int)strtol(&line[i],&aux,10);
      result->addVertex(vertex_identifier);
      aux=&line[i];
      newaux=aux;
      do{
        aux=newaux;
        vertex_neighbour=(int)strtol(aux,&newaux,10);
        if(newaux-aux!=0){
          result->addVertexNeighbour(vertex_identifier,vertex_neighbour);
        }
      } while(aux!=newaux);
      vertex_identifier++;
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
// GLOBAL operation readGraphFile(char *fileName)
// RETURNS a graph pointer
//-----------------------------------------------------------------------------
int readBetweenness(char *fileName,double* myBC){
  FILE *input;
  int i=0;
  int vertexCount=0;
  char *line;
  char *aux,*newaux;

  line=(char *)malloc(sizeof(char)*STRING_LENGTH);
  if((input=fopen(fileName,"rt"))==NULL){
    printf("file not found\n");
    exit(1);
  }
  line[0]='\0';
  line=fgets(line,STRING_LENGTH,input);
  while(line!=NULL){
    if(line!=NULL && line[0]!='#'){
//      vertex_identifier=(int)strtol(&line[i],&aux,10);
      aux=&line[i];
      newaux=aux;
      do{
        aux=newaux;
        strtol(aux,&newaux,10);
        if(newaux-aux!=0){
          vertexCount++;
        }
      } while(aux!=newaux);
    }
    line[0]='\0';
    line=fgets(line,STRING_LENGTH,input);
  }
  fclose(input);

  myBC=(double *)malloc(sizeof(int)*vertexCount);

  if((input=fopen(fileName,"rt"))==NULL){
    printf("file not found\n");
    exit(1);
  }
  line[0]='\0';
  line=fgets(line,STRING_LENGTH,input);
  while(line!=NULL){
    if(line!=NULL && line[0]!='#'){
//      vertex_identifier=(int)strtol(&line[i],&aux,10);
      aux=&line[i];
      newaux=aux;
      do{
        aux=newaux;
        strtol(aux,&newaux,10);
        if(newaux-aux!=0){
          vertexCount++;
        }
      } while(aux!=newaux);
    }
    line[0]='\0';
    line=fgets(line,STRING_LENGTH,input);
  }
  fclose(input);
  free(line);

  return vertexCount;
}



//-----------------------------------------------------------------------------
// GLOBAL operation readParameters(char *fileName)
// RETURNS an int
//-----------------------------------------------------------------------------
int readParameters(char *fileName){
  FILE *input;
  int result;
  char *line=(char *)malloc(sizeof(char)*STRING_LENGTH);

  if((input=fopen(fileName,"rt"))==NULL){
    printf("file not found\n");
    exit(1);
  }
  line[0]='\0';
  line=fgets(line,STRING_LENGTH,input);
  while(line!=NULL){
    if(line!=NULL && line[0]!='#'){
    }
    line[0]='\0';
    line=fgets(line,STRING_LENGTH,input);
  }
  fclose(input);
  free(line);
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
// GLOBAL operation copyArray(sourceArray,objectArray,arraySize)
// Copies arraySize double values from sourceArray to objectArray
//-----------------------------------------------------------------------------
void copyArray(double *sourceArray,double *objectArray,int arraySize){
  for(int i=0;i<arraySize;i++){
    objectArray[i]=sourceArray[i];
  }
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


int *getSortingOrder(double *source,int count){
  int i,j,min_pos,aux_pos;
  double min_val,aux_val;
  int *result=(int *)malloc(sizeof(int)*count);
  myBC *aux=(myBC *)malloc(sizeof(myBC)*count);
  for(i=0;i<count;i++){
    aux[i].position=i;
    aux[i].value=source[i];
  }

  for(i=0;i<count;i++){
    min_val=aux[i].value;
    min_pos=aux[i].position;
    for(j=i+1;j<count;j++){
      if(aux[j].value<min_val){
        min_val=aux[j].value;
        min_pos=j;
      }
    }
    if(min_pos!=aux[i].position){
      aux_val=aux[i].value;
      aux_pos=aux[i].position;
      aux[i].value=min_val;
      aux[i].position=min_pos;
      aux[min_pos].value=aux_val;
      aux[min_pos].position=aux_pos;
    }
  }
  for(i=0;i<count;i++){
    result[i]=aux[i].position;
  }
  free(aux);
  return result;
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

//-----------------------------------------------------------------------------
// GLOBAL operation cost(double *tarjet,double *current,int count)
// RETURNS the cost
//-----------------------------------------------------------------------------
double cost(double *target,double *current, int *sorting, int count){
  int i;
  double result=0.0;
  double aux[count];

  for(i=0;i<count;i++){
    aux[i]=current[sorting[i]];
  }
  for(i=0; i<count; i++){
    result+=(double)i*(double)pow((target[i]-aux[i]),2);
  }
  return((double)pow(result,0.5));
}

int getMax(double *source, int count){
  double max=source[0];
  double aux;
  int result=0;
  for(int i=0;i<count;i++){
    if((aux=source[i])>max) {
      max=aux;
      result=i;
    }
  }
  return result;
}

int getMin(double *source, int count){
  double min=source[0];
  double aux;
  int result=0;
  for(int i=0;i<count;i++){
    if((aux=source[i])<min) {
      min=aux;
      result=i;
    }
  }
  return result;
}

int getMax(double *source1, double *source2,int count){
  double max=pow(source1[0]-source2[0],2);
  double aux;
  int result=0;
  for(int i=0;i<count;i++){
    if((aux=pow(source1[i]-source2[i],2))>max) {
      max=aux;
      result=i;
    }
  }
  return result;
}

int getMin(double *source1,double *source2, int count){
  double min=pow(source1[0]-source2[0],2);
  double aux;
  int result=0;
  for(int i=0;i<count;i++){
    if((aux=pow(source1[i]-source2[i],2))<min) {
      min=aux;
      result=i;
    }
  }
  return result;
}

int main(int argc,char *argv[]){

  int i=0;
  int lx=0;
  int ly=0;
  int lz=0;
  int weAreDone=0;
  time_t timeStart;
  time_t timeEnd;

  char inputFilename[STRING_LENGTH];
  char outputFilename[STRING_LENGTH];
  char outputGraphFilename[STRING_LENGTH];
  char inputGraphFilename[STRING_LENGTH];
  char logFilename[STRING_LENGTH];

  FILE *input=NULL;
  FILE *output=NULL;
  FILE *outputGraph=NULL;
  FILE *inputGraph=NULL;
  FILE *logFile=NULL;
  int iterations=0;
  double tol=TOL;

  // Simmulated Annealing variables
  double To=TO;
  double Tmin=TMIN;
  double Tk=TO;
  long int Nmax=NMAX;
  long int N=0;
  double k=K;

  double costNew=0.0;
  double costBest=0.0;
  double costOld=0.0;

  graph *targetGraph=NULL;
  graph *bestGraph=NULL;
  graph *oldGraph=NULL;
  graph *newGraph=NULL;

  int graphOrder=0;
  int inputFormat=PYTHON;

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
  printf("[llav_x][llav_y][llav_z]\n");
  switch(argc){
    case 10:random_value_z=atoi(argv[9]);
    case  9:random_value_y=atoi(argv[8]);
    case  8:random_value_x=atoi(argv[7]);
    case  7:k=atof(argv[6]);
    case  6:Nmax=atoi(argv[5]);
    case  5:Tmin=atof(argv[4]);
    case  4:To=atof(argv[3]);
    case  3:
      if((strcmp(argv[2],"P")==0)||(strcmp(argv[2],"p")==0)){
        inputFormat=PYTHON;
      } else if((strcmp(argv[2],"A")==0)||(strcmp(argv[2],"a")==0)) {;
        inputFormat=ADJLIST;
      } else if((strcmp(argv[2],"B")==0)||(strcmp(argv[2],"b")==0)){;
        inputFormat=BETWEENNESS;
      }
    case  2:strcpy(inputFilename,argv[1]);
    break;
    default:printf("ERROR incorrect parameters\n");
             exit(-1);
  }
printf("hola\n");
  if(inputFormat==PYTHON){
    targetGraph=readPythonGraphFile(inputFilename);
  } else if(inputFormat==ADJLIST){
    targetGraph=readGraphFile(inputFilename);
  } else if(inputFormat==BETWEENNESS){
//    graphOrder=readBetweenness(inputFilename,targetBC);
  }

  graphOrder=targetGraph->getOrder();
  printf("si\n");

  double targetBC [graphOrder];
  double bestBC [graphOrder];
  double oldBC [graphOrder];
  double newBC [graphOrder];

  for(i=0;i<graphOrder;i++){
    targetBC[i]=0.0;
    bestBC[i]=0.0;
    oldBC[i]=0.0;
    newBC[i]=0.0;
  }
  targetGraph->printGraph();
  if((inputFormat==PYTHON)||(inputFormat==ADJLIST)){
    targetGraph->setAllVertexNeighbours();
    targetGraph->brandes_betweenness_centrality(targetBC);
    strcpy(inputGraphFilename,inputFilename);
    strcat(inputGraphFilename,".in");
    inputGraph=fopen(inputGraphFilename,"w");
    if(inputGraph==NULL){
      printf("Cannot open in graph file %s for writting\n",
             inputGraphFilename);
      exit(-1);
    }
    targetGraph->printMyGraph(inputGraph);
    fclose(inputGraph);
  }

  strcpy(outputFilename,inputFilename);
  strcat(outputFilename,".out");
  output=fopen(outputFilename,"w");
  if(output==NULL){
    printf("Cannot open output file %s for writting\n",outputFilename);
    exit(-1);
  }

  strcpy(outputGraphFilename,inputFilename);
  strcat(outputGraphFilename,".res");
  outputGraph=fopen(outputGraphFilename,"w");
  if(outputGraph==NULL){
    printf("Cannot open out graph file %s for writting\n",outputGraphFilename);
    exit(-1);
  }

  strcpy(logFilename,inputFilename);
  strcat(logFilename,".log");
  logFile=fopen(logFilename,"w");
  if(logFile==NULL){
    printf("Cannot open log file %s for writting %s\n",logFilename);
    exit(-1);
  }

  // STARTING SIMMULATED ANNEALING
  Tk=To;
  bestGraph=generateInitialGraph(graphOrder);
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
        copyArray(newBC,bestBC,graphOrder);
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

  // Processing tasks accomplished
  // Showing results

  timeEnd=time(NULL);

  printf("RESULTS:\n");
  printf("CPU time needed: %f seconds\n",difftime(timeEnd,timeStart));
  printf("Output file: %s\n",outputFilename);
  fprintf(output,"Graph reconstruction via  vertex betweenness centrality\n");
  fprintf(output,"\tOriginal graph description file-> %s",inputFilename);
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
  for(i=0; i<graphOrder; i++){
    fprintf(output,"\t%2.10f | ",targetBC[i]);
    fprintf(output,"%2.10f | ",bestBC[i]);
    fprintf(output,"%2.10f\n",pow(targetBC[i]-bestBC[i],2));
  }
  fprintf(output,"CPU time needed: %f seconds\n",difftime(timeEnd,timeStart));
  fclose(output);

  printf("\nReconstructed graph file: %s\n",outputGraphFilename);
  bestGraph->printGraph();

  bestGraph->printMyGraph(outputGraph);
  fclose(outputGraph);
}
