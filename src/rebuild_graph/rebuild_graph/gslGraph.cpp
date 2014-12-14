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
		free(matrix);
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


void gslGraph::copyGraph(gslGraph * newgslGraph)const{
	newgslGraph->order = getOrder();
	newgslGraph->degree = getDegree();
	if ( order !=  matrix->size1)
		throw runtime_error("Order and matrix size are different");
	if ( order !=  matrix->size2)
		throw runtime_error("Order and matrix size are different");
	if ( newgslGraph->matrix)
		free (newgslGraph->matrix);
	newgslGraph->matrix = gsl_matrix_alloc(matrix->size1,matrix->size2);
	gsl_matrix_memcpy ( newgslGraph->matrix, matrix );
}


void gslGraph::printMyGraph(const char * outputGraphFilename)const{
	CFuncTrace trace (false,"printMyGraph");
	FILE *outputGraph = NULL;
	outputGraph=fopen(outputGraphFilename,"w");
	if(outputGraph==NULL){
		printf("Cannot open out graph file %s for writting\n",outputGraphFilename);
		exit(-1);
	}
	for (size_t i = 0; i < matrix->size1; i++) {
		for (size_t j = 0; j < matrix->size2; j++) {
			fprintf(outputGraph,"%d ",(int)
					gsl_matrix_get(matrix, i, j));
			if ( j == matrix->size2 -1 )
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

gslGraph *gslGraph::readPythonGraphFile(char *fileName){
	
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

void gslGraph::printGraph(int TRACE_LEVEL){
	int i,j;
	CFuncTrace trace(false,"gslGraph::printGraph");
	
	trace.trace(DEBUG_GSL_GRAPH,"The graph has %d order %d degree:\n",order,degree);
	for(i=0;i<order;i++){
		trace.trace(DEBUG_GSL_GRAPH,"Vertex %d",i);
		for(j=0;j<order;j++){
			if (gsl_matrix_get (matrix, i,j)) {
				trace.trace(DEBUG_GSL_GRAPH," %d ",j);
			}
		}
		  	trace.trace(DEBUG_GSL_GRAPH,"\n");
	}
}




void gslGraph::node_and_edge_betweenness_bin(const gsl_matrix* sourceGraph, gsl_vector* node_betweenness,gsl_matrix* edge_betweenness) const {
	CFuncTrace trace(false,"node_and_edge_betweenness_bin");
	if (sourceGraph->size1 != sourceGraph->size2) {
		trace.trace(CTrace::level::TRACE_ERROR,"ERROR size2 and size2 different");
		return;
	}
	bool free_node_betweenness = false;
	bool free_edge_betweenness = false;
	
	// BC=zeros(n,1);
	if (node_betweenness == NULL) {
		free_node_betweenness = true;
		node_betweenness = gsl_vector_calloc(sourceGraph->size1);
	} else {
		gsl_vector_set_zero(node_betweenness);
	}
	if (edge_betweenness == NULL) {
		free_edge_betweenness = true;
		edge_betweenness = gsl_matrix_calloc(sourceGraph->size1, sourceGraph->size2);
	} else {
		gsl_matrix_set_zero(edge_betweenness);
	}
	
	
	// for u=1:n
	for (int u = 0; u < (int)sourceGraph->size1; u++) {
		
		
		gsl_vector* d = gsl_vector_calloc(sourceGraph->size1);
		gsl_vector_set(d, u, 1.0);
		
		// NP=zeros(1,n); NP(u)=1;
		gsl_vector* sigma = gsl_vector_calloc(sourceGraph->size1);
		gsl_vector_set(sigma, u, 1.0);
		
		// P=false(n);
		gsl_matrix* p = gsl_matrix_calloc(sourceGraph->size1, sourceGraph->size2);
		
		// Q=zeros(1,n); q=n;
		gsl_vector* Q = gsl_vector_calloc(sourceGraph->size1);
		int q = (int)sourceGraph->size1 - 1;
		
		//gsl_matrix* copy_sourceGraph = copy(sourceGraph);
		gsl_matrix * copy_sourceGraph = gsl_matrix_alloc(sourceGraph->size1,sourceGraph->size2);
		gsl_matrix_memcpy ( copy_sourceGraph, sourceGraph );
		
		// V=u;
		gsl_vector* V = gsl_vector_alloc(1);
		gsl_vector_set(V, 0, u);
		
		// while V
		while (V != NULL) {
			
			// Gu(:,V)=0;
			for (int V_index = 0; V_index < (int)V->size; V_index++) {
				int v = (int)gsl_vector_get(V, V_index);
				gsl_vector_view copy_m_column = gsl_matrix_column(copy_sourceGraph, v);
				gsl_vector_set_zero(&copy_m_column.vector);
			}
			
			// for v=V
			for (int V_index = 0; V_index < (int)V->size; V_index++) {
				int v = (int)gsl_vector_get(V, V_index);
				gsl_vector_set(Q, q--, v);
				
				// W=find(Gu(v,:));
				gsl_vector_view copy_m_row = gsl_matrix_row(copy_sourceGraph, v);
				gsl_vector* W = find(&copy_m_row.vector);
				
				// For all the neighbours of v
				for (int W_index = 0; W != NULL && W_index < (int)W->size; W_index++) {
					int w = (int)gsl_vector_get(W, W_index);
					// shortest path to w via v?
					if (fp_nonzero(gsl_vector_get(d, w))) {
						
						double sigma_w = gsl_vector_get(sigma, w);
						double sigma_v = gsl_vector_get(sigma, v);
						gsl_vector_set(sigma, w, sigma_w + sigma_v);
						
						gsl_matrix_set(p, w, v, 1.0);
						
						// else
					} else {
						
						gsl_vector_set(d, w, 1.0);
						gsl_vector_set(sigma, w, gsl_vector_get(sigma, v));
						gsl_matrix_set(p, w, v, 1.0);
					}
				}
				if (W != NULL) {
					gsl_vector_free(W);
				}
			}
			
			// V=find(any(Gu(V,:),1));
			gsl_vector* column_indices = sequence(0, (int)sourceGraph->size2 - 1);
			gsl_matrix* copy_m_rows = ordinal_index(copy_sourceGraph, V, column_indices);
			gsl_vector* any_copy_m_rows = any(copy_m_rows);
			gsl_vector_free(V);
			V = find(any_copy_m_rows);
			gsl_vector_free(any_copy_m_rows);
			gsl_matrix_free(copy_m_rows);
			gsl_vector_free(column_indices);
		}
		
		// if ~all(D)
		if (all(d) == 0) {
			trace.trace(CTrace::level::TRACE_DEBUG,"All the distances are 0");
			// Q(1:q)=find(~D);
			gsl_vector* not_d = logical_not(d);
			gsl_vector* not_d_indices = find(not_d);
			gsl_vector_view Q_upto_q = gsl_vector_subvector(Q, 0, q + 1);
			gsl_vector_memcpy(&Q_upto_q.vector, not_d_indices);
			gsl_vector_free(not_d_indices);
			gsl_vector_free(not_d);
		}
		
		// S returns vertices in order of non-increasing distance from s
		gsl_vector* delta = gsl_vector_calloc(sourceGraph->size1);
		
		// for w=Q(1:n-1);
		for (int Q_index = 0; Q_index < (int)sourceGraph->size1 - 1; Q_index++) {
			
			int w = (int)gsl_vector_get(Q, Q_index);
			// BC(w)=BC(w)+DP(w)
			double bcw = gsl_vector_get(node_betweenness, w);
			double delta_w = gsl_vector_get(delta, w);
			gsl_vector_set(node_betweenness, w, bcw + delta_w);
			
			// for v=find(P(w,:))
			gsl_vector_view p_row = gsl_matrix_row(p, w);
			gsl_vector* found_p_row = find(&p_row.vector);
			//			printGslMatrix(p);
			for (int p_index = 0; found_p_row != NULL && p_index < (int)found_p_row->size; p_index++) {
				int v = (int)gsl_vector_get(found_p_row, p_index);
				//				std::cout << " v " << v << "Is connected with w" << w << std::endl;
				// DPvw=(1+DP(w)).*NP(v)./NP(w);
				double sigma_v = gsl_vector_get(sigma, v);
				double sigma_w = gsl_vector_get(sigma, w);
				double dpvw = (1 + delta_w) * sigma_v / sigma_w;
				//				std::cout << " sigma_v " << sigma_v << "sigma_w" << sigma_w << std::endl;
				// DP(v)=DP(v)+DPvw;
				double dpv = gsl_vector_get(delta, v);
				gsl_vector_set(delta, v, dpv + dpvw);
				double ebcvw = gsl_matrix_get(edge_betweenness, v, w);
				
				gsl_matrix_set(edge_betweenness, v, w, ebcvw + dpvw);
				
			}
			if (found_p_row != NULL) {
				gsl_vector_free(found_p_row);
			}
		}
		
		gsl_vector_free(delta);
		gsl_matrix_free(copy_sourceGraph);
		gsl_vector_free(Q);
		gsl_matrix_free(p);
		gsl_vector_free(sigma);
		gsl_vector_free(d);
	}
	if (free_node_betweenness) {
		gsl_vector_free(node_betweenness);
	}
	if (free_edge_betweenness) {
		gsl_matrix_free(edge_betweenness);
	}
	
}



gsl_vector* gslGraph::sequence(int start, int end) const{
	int step =1;
	int n_seq = (end - start) / step + 1;
	if (n_seq <= 0) {
		return NULL;
	}
	gsl_vector* seq_v = gsl_vector_alloc(n_seq);
	for (int i = 0, value = start; i < n_seq; i++, value += step) {
		gsl_vector_set(seq_v, i, value);
	}
	return seq_v;
}


/*
 * Computes node betweenness centrality for a binary graph.  Results are
 * returned in a vector where each element is the betweenness centrality of the
 * corresponding node.
 */
gsl_vector* gslGraph::betweenness_bin(const gsl_matrix* sourceGraph) const {
	CFuncTrace trace(false,"gslGraph::betweenness_bin");
	gsl_vector* betweenness = gsl_vector_alloc(sourceGraph->size1);
	gsl_matrix* betweeness_edge = gsl_matrix_calloc( sourceGraph->size1, sourceGraph->size2);
	int static num_traces = 0;
	if ( num_traces++ % 1000 == 0){
		trace.trace(CTrace::level::TRACE_ERROR,"REVEIW This code begin");
		//	node_and_edge_betweenness_bin(sourceGraph, NULL ,betweeness_edge);
		//	printGslMatrix(betweeness_edge);
		trace.trace(CTrace::level::TRACE_ERROR,"REVEIW This code end");
	}
	node_and_edge_betweenness_bin(sourceGraph, betweenness, NULL );
	return betweenness;
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


void gslGraph::brandes_comunicability_centrality_exp(double *myCExp)
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


/*
 * Emulates (~v).
 */
gsl_vector * gslGraph::logical_not(const gsl_vector* v) const {
	gsl_vector* not_v = gsl_vector_alloc(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		bool z = fp_zero(gsl_vector_get(v, i));
		gsl_vector_set(not_v, i, (double)z);
	}
	return not_v;
}

// Matrix-by-two-vectors indexing (non-mixed)

gsl_matrix* gslGraph::ordinal_index(const gsl_matrix* m, const gsl_vector* rows,
						  const gsl_vector* columns) const {
	gsl_matrix* index_m = gsl_matrix_alloc(rows->size, columns->size);
	for (int i = 0; i < (int)rows->size; i++) {
		int row = (int)gsl_vector_get(rows, i);
		for (int j = 0; j < (int)columns->size; j++) {
			int column = (int)gsl_vector_get(columns, j);
			double value = gsl_matrix_get(m, row, column);
			gsl_matrix_set(index_m, i, j, value);
		}
	}
	return index_m;
}

gsl_vector* gslGraph::find(const gsl_vector* v, int n ,
				 const std::string& direction ) const {
	int n_find = nnz(v);
	if (n_find == 0 || n < 1) {
		return NULL;
	}
	gsl_vector* find_v = gsl_vector_alloc((n < n_find) ? n : n_find);
	if (direction == "first") {
		int position = 0;
		for (int i = 0; i < (int)v->size && position < (int)find_v->size; i++) {
			if (fp_nonzero(gsl_vector_get(v, i))) {
				gsl_vector_set(find_v, position, i);
				position++;
			}
		}
		return find_v;
	} else if (direction == "last") {
		int position = ((int)find_v->size) - 1;
		for (int i = ((int)v->size) - 1; i >= 0 && position >= 0; i--) {
			if (fp_nonzero(gsl_vector_get(v, i))) {
				gsl_vector_set(find_v, position, i);
				position--;
			}
		}
		return find_v;
	} else {
		gsl_vector_free(find_v);
		std::cout << "find::Returning NULL" << std::endl;
		return NULL;
	}
}


void gslGraph::brandes_betweenness_centrality(double *myBBC){
	CFuncTrace trace (false,"brandes_betweenness_centrality");
	gsl_vector * v =betweenness_bin(matrix);
	gslVectorToArray(v,myBBC);
	for(int i=0;i< order;i++){
		myBBC[i]=(myBBC[i])/((order-1.0)*(order-2.0));
		trace.trace(CTrace::level::TRACE_DEBUG," BC ",  myBBC[i]);
	}
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



gsl_vector* gslGraph::any(const gsl_matrix* m, int dim ) const {
	if (dim == 1) {
		gsl_vector* any_v = gsl_vector_alloc(m->size2);
		for (int i = 0; i < (int)m->size2; i++) {
			gsl_vector_const_view m_col_i = gsl_matrix_const_column(m, i);
			gsl_vector_set(any_v, i, any(&m_col_i.vector));
		}
		return any_v;
	} else if (dim == 2) {
		gsl_vector* any_v = gsl_vector_alloc(m->size1);
		for (int i = 0; i < (int)m->size1; i++) {
			gsl_vector_const_view m_row_i = gsl_matrix_const_row(m, i);
			gsl_vector_set(any_v, i, any(&m_row_i.vector));
		}
		return any_v;
	} else {
		return NULL;
	}
}


int  gslGraph::nnz(const gsl_vector* v) const {
	int nnz = 0;
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i))) {
			nnz++;
		}
	}
	return nnz;
}


int  gslGraph::any(const gsl_vector* v) const {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i))) {
			return 1;
		}
	}
	return 0;
}

int  gslGraph::all(const gsl_vector* v) const {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_zero(gsl_vector_get(v, i))) {
			return 0;
		}
	}
	return 1;
}


int  gslGraph::graphToGsl( gsl_matrix* target){
	gsl_matrix_memcpy(target,matrix);
	return 1;
}
/*
gsl_matrix *  gslGraph::copy ( const gsl_matrix *orig)const{
	gsl_matrix * result = gsl_matrix_alloc(orig->size1,orig->size2);
	gsl_matrix_memcpy ( result, orig );
	return result;
}
 */

int gslGraph::compare(gslGraph * graph1, gslGraph * graph2){
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
