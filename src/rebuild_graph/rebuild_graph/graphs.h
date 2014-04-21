//
//  graphs.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 04/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef rebuild_graph_graphs_h
#define rebuild_graph_graphs_h


#include "CSettingsSumulation.h"
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
    void printMyGraph(const char*outputFileName);
	
    // Print the graph's betweenness centrality values
    void printGraphBetweenness();
	
    // Print the graph's betweenness centrality values
    void printGraphBetweenness(FILE *myFile);
	
    ////////////////////////////////////////////////////////////////////////////
    /////             Other interesting graph operations                   /////
    ////////////////////////////////////////////////////////////////////////////
	
    // Get the number of vertex (order) of the graph
    int getOrder() const;
	
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

int
fregenerateGraph(CSettingsSimulation &settingsSimulation, double *&targetBC, double *&bestBC,int *order);

#endif
