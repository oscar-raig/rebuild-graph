//
//  graphIndicatorBetweennessCentrality.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 22/12/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include "graphIndicatorBetweennessCentrality.h"
#include <stdexcept>
#include <queue>
#include <list>
using namespace std;
using std::runtime_error;
#include <map>


void graphIndicatorBetweennessCentrality::brandes_betweenness_centrality(double *arrayIndicatorBetweennesscentrality){
	
	CFuncTrace trace (false,"brandes_betweenness_centrality");
	gsl_matrix *gslmatrix = gsl_matrix_alloc(this->graph->getOrder(), this->graph->getOrder());
	
	this->graph->graphToGsl( gslmatrix );
	
	gsl_vector * v = betweenness_bin( gslmatrix );
	gslGraph::gslVectorToArray(v,arrayIndicatorBetweennesscentrality);
	for(int i=0;i< this->graph->getOrder();i++){
		arrayIndicatorBetweennesscentrality[i]=
		(arrayIndicatorBetweennesscentrality[i])/((this->graph->getOrder()-1.0)*(this->graph->getOrder()-2.0));
		trace.trace(CTrace::TRACE_DEBUG," BC ",  arrayIndicatorBetweennesscentrality[i]);
	}
}


/*
 * Computes node betweenness centrality for a binary graph.  Results are
 * returned in a vector where each element is the betweenness centrality of the
 * corresponding node.
 */
gsl_vector* graphIndicatorBetweennessCentrality::betweenness_bin(const gsl_matrix* sourceGraph) const {
	CFuncTrace trace(false,"gslGraph::betweenness_bin");
	gsl_vector* betweenness = gsl_vector_calloc(sourceGraph->size1);
	node_and_edge_betweenness_bin(sourceGraph, betweenness);
	return betweenness;
}

/*
 * Emulates (~v).
 */
gsl_vector * graphIndicatorBetweennessCentrality::logical_not(const gsl_vector* v) const {
	gsl_vector* not_v = gsl_vector_alloc(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		bool z = fp_zero(gsl_vector_get(v, i));
		gsl_vector_set(not_v, i, (double)z);
	}
	return not_v;
}


int  graphIndicatorBetweennessCentrality::getNumberOfNonZeroInVector(const gsl_vector* v) const {
	int nnz = 0;
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i))) {
			nnz++;
		}
	}
	return nnz;
}

gsl_vector* graphIndicatorBetweennessCentrality::returnVectorWithNonZeroIndexOfASourceVector(const gsl_vector* v,
													int columnException ) const {
	int n_find = getNumberOfNonZeroInVector(v);
	if (n_find == 0 ) {
		return NULL;
	}
	gsl_vector* find_v = gsl_vector_alloc( n_find);

	int position = 0;
	for (int i = 0; i < (int)v->size && position < (int)find_v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i)) && columnException != i ) {
			gsl_vector_set(find_v, position, i);
			position++;
		}
	}
	return find_v;
}

void graphIndicatorBetweennessCentrality::recalculateDeltaAndBetweennessCentrality(
				std::list<int>		S,gsl_matrix *p,int s, gsl_vector *sigma,
				gsl_vector* betweenness_centrality)const{
	// S returns vertices in order of non-increasing distance from s
	gsl_vector* delta = gsl_vector_calloc(betweenness_centrality->size);
	
	// for w=Q(1:n-1);
	int w;
	while ( !S.empty()){
		w = S.front();
		S.pop_front();
		calculateDeltaForW(p,w,sigma,delta);
		if ( w != s)
			updateBetweenessCentralityForW(delta,w,betweenness_centrality);
	}
	
	gsl_vector_free(delta);
}

void  graphIndicatorBetweennessCentrality::updateBetweenessCentralityForW(gsl_vector* delta,int w,gsl_vector* betweenness_centrality) const{
	// BC(w)=BC(w)+DP(w)
	double Old_betweenness_centrality_ForVertex_W = gsl_vector_get(betweenness_centrality, w);
	double delta_w = gsl_vector_get(delta, w);
//	cout << "BC :Actualitzatn bc amb delta " << delta_w <<  " per vertex " << w << endl;
	gsl_vector_set(betweenness_centrality, w, Old_betweenness_centrality_ForVertex_W + delta_w);
//	cout << "Actualitzatn betweeness " << gsl_vector_get(betweenness_centrality,w) << " " << w << endl;
	
}

void  graphIndicatorBetweennessCentrality::calculateSigma(int v, gsl_matrix *sourceGraph,map<int, int> &D,
											std::queue<int> &Queue,gsl_matrix *P,gsl_vector *sigma) const{
	//			std::cout <<"\tIterating on " << v << std::endl;
	// W=find(Gu(v,:));
	gsl_vector_view copy_m_row = gsl_matrix_row((gsl_matrix*)sourceGraph, v);
	gsl_vector* W = returnVectorWithNonZeroIndexOfASourceVector(&copy_m_row.vector,v);
	
	//    for w in G[v]:
	for (int W_index = 0; W != NULL && W_index < (int)W->size; W_index++) {
		int w = (int)gsl_vector_get(W, W_index);
		//					std::cout <<"\t\tIterating on " << w << std::endl;
		
		if ( D.find(w) == D.end() ){
			//						std::cout <<"w not in D " << w << std::endl;
			Queue.push(w);
			int value = D[v] + 1;
			D[w] = value;
		}
		
		//	for (std::map<int,int>::iterator it=D.begin(); it!=D.end(); ++it)
		//			std::cout << it->first << " => " << it->second << '\n';
		
		if( ((int)D[w]) == ((int)D[v]) + 1 ){
			//						std::cout << "We have found shortest path w Dw v Dv" << w << D[w] << v << D[v] << endl;
			gsl_matrix_set(P, w, v, 1.0);  // P[w].append(v) # predecessors
			double sigma_w = gsl_vector_get(sigma, w);
			double sigma_v = gsl_vector_get(sigma, v);
			gsl_vector_set(sigma, w, sigma_w + sigma_v);
			//						cout << "\t\t\tsigma w w " << w  << " "<< sigma_w + sigma_v << endl;
		}
	}
	if (W != NULL) {
		gsl_vector_free(W);
	}

}


void  graphIndicatorBetweennessCentrality::calculateDeltaForW(gsl_matrix *p, int w,gsl_vector* sigma,
														  gsl_vector*delta) const{
	// for v=find(P(w,:))
	double delta_w = gsl_vector_get(delta, w);
	gsl_vector_view p_row = gsl_matrix_row(p, w);
	gsl_vector* found_p_row = returnVectorWithNonZeroIndexOfASourceVector(&p_row.vector,-1);
	//			printGslMatrix(p);
	for (int p_index = 0; found_p_row != NULL && p_index < (int)found_p_row->size; p_index++) {
		int v = (int)gsl_vector_get(found_p_row, p_index);
//		std::cout << "Calculate delta: v " << v << "Is connected with w" << w << std::endl;
		// DPvw=(1+DP(w)).*NP(v)./NP(w);
		double sigma_v = gsl_vector_get(sigma, v);
		double sigma_w = gsl_vector_get(sigma, w);
		double dpvw = (1.0 + delta_w) * sigma_v / sigma_w;
		//	std::cout << " sigma_v " << sigma_v << " sigma_w " << sigma_w << " dpvw " << dpvw << std::endl;
		// DP(v)=DP(v)+DPvw;
		double dpv = gsl_vector_get(delta, v);
		gsl_vector_set(delta, v, dpv + dpvw);
//		std::cout << "New delta " <<  dpv + dpvw << " For node " << v << endl;
		
	}
	if (found_p_row != NULL) {
		gsl_vector_free(found_p_row);
	}
}

void graphIndicatorBetweennessCentrality::node_and_edge_betweenness_bin(const gsl_matrix* sourceGraph, gsl_vector* betweenness_centrality) const {
	CFuncTrace trace(false,"node_and_edge_betweenness_bin");
	if (sourceGraph->size1 != sourceGraph->size2) {
		trace.trace(CTrace::TRACE_ERROR,"ERROR size2 and size2 different");
		return;
	}
//	gsl_matrix * copy_sourceGraph = gsl_matrix_alloc(sourceGraph->size1,sourceGraph->size2);
//	gsl_matrix_memcpy ( copy_sourceGraph, sourceGraph );
	// for u=1:n
	for (int s = 0; s < (int)sourceGraph->size1; s++) {
//		std::cout << "Working with s: " << s << "---------"<< endl;
		
		map<int, int> D; //   D={}
		D[s]=0;
	//	for (std::map<int,int>::iterator it=D.begin(); it!=D.end(); ++it)
	//		std::cout << it->first << " => " << it->second << '\n';
		
		
		// sigma=dict.fromkeys(G,0)    # sigma[v]=0 for v in G
		gsl_vector* sigma = gsl_vector_calloc(sourceGraph->size1);
		gsl_vector_set(sigma, s, 1.0);  //   sigma[s]=1
		
		//  P={} //  for v in G: P[v]=[]
		gsl_matrix* P = gsl_matrix_calloc(sourceGraph->size1, sourceGraph->size2);
		

		// Q=[s]
		std::queue<int> Queue;
		std::list<int> S; //  S=[]
		
		Queue.push(s);
		
		// while V
		while (!Queue.empty()) {
			int v;
			v= Queue.front();
			Queue.pop();
			S.push_front(v);
			calculateSigma(v,(gsl_matrix*) sourceGraph,D,Queue,P,sigma);

		}
		recalculateDeltaAndBetweennessCentrality(S,P,s,sigma,betweenness_centrality);
		gsl_matrix_free(P);
		gsl_vector_free(sigma);
	}
//	gsl_matrix_free(copy_sourceGraph);
	
}



int  graphIndicatorBetweennessCentrality::anyNonZeroElemenInVector(const gsl_vector* v) const {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i))) {
			return 1;
		}
	}
	return 0;
}

int  graphIndicatorBetweennessCentrality::allNonZero(const gsl_vector* v) const {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_zero(gsl_vector_get(v, i))) {
			return 0;
		}
	}
	return 1;
}

// Matrix-by-two-vectors indexing (non-mixed)

gsl_matrix* graphIndicatorBetweennessCentrality::submatrix(const gsl_matrix* m, const gsl_vector* rows,
									int column_size){
	
	if ( m  == NULL ) {
		throw runtime_error("matrix is NULL");
	}
	if ( rows	== NULL ) {
		throw runtime_error("ROWS is NULL");
	}
	if ( column_size == 0)
		throw runtime_error("column size cannot be 0");
	
	gsl_matrix* index_m = gsl_matrix_alloc(rows->size, column_size);
	for (int i = 0; i < (int)rows->size; i++) {
		int row = (int)gsl_vector_get(rows, i);
		for (int j = 0; j < column_size; j++) {
			double value = gsl_matrix_get(m, row, j);
			gsl_matrix_set(index_m, i, j, value);
		}
	}
	return index_m;
}


gsl_vector* graphIndicatorBetweennessCentrality::anyUnconnectedVertex(const gsl_matrix* m ) const {
		gsl_vector* any_v = gsl_vector_alloc(m->size2);
		for (int i = 0; i < (int)m->size2; i++) {
			gsl_vector_const_view m_col_i = gsl_matrix_const_column(m, i);
			gsl_vector_set(any_v, i, anyNonZeroElemenInVector(&m_col_i.vector));
		}
		return any_v;
	}

