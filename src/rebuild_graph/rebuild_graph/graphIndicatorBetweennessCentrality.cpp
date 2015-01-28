//
//  graphIndicatorBetweennessCentrality.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 22/12/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include "graphIndicatorBetweennessCentrality.h"
#include <stdexcept>
using std::runtime_error;


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
	gsl_vector* betweenness = gsl_vector_alloc(sourceGraph->size1);
	node_and_edge_betweenness_bin(sourceGraph, betweenness, NULL );
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


int  graphIndicatorBetweennessCentrality::nnz(const gsl_vector* v) const {
	int nnz = 0;
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i))) {
			nnz++;
		}
	}
	return nnz;
}

gsl_vector* graphIndicatorBetweennessCentrality::find(const gsl_vector* v, int n ,
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


void graphIndicatorBetweennessCentrality::node_and_edge_betweenness_bin(const gsl_matrix* sourceGraph, gsl_vector* node_betweenness,gsl_matrix* edge_betweenness) const {
	CFuncTrace trace(false,"node_and_edge_betweenness_bin");
	if (sourceGraph->size1 != sourceGraph->size2) {
		trace.trace(CTrace::TRACE_ERROR,"ERROR size2 and size2 different");
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
			gsl_matrix* copy_m_rows = submatrix(copy_sourceGraph, V, column_indices);
			gsl_vector* any_copy_m_rows = any(copy_m_rows);
			gsl_vector_free(V);
			V = find(any_copy_m_rows);
			gsl_vector_free(any_copy_m_rows);
			gsl_matrix_free(copy_m_rows);
			gsl_vector_free(column_indices);
		}
		
		// if ~all(D)
		if (all(d) == 0) {
			trace.trace(CTrace::TRACE_DEBUG,"All the distances are 0");
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

gsl_vector* graphIndicatorBetweennessCentrality::sequence(int start, int end) const{
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

int  graphIndicatorBetweennessCentrality::any(const gsl_vector* v) const {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(gsl_vector_get(v, i))) {
			return 1;
		}
	}
	return 0;
}

int  graphIndicatorBetweennessCentrality::all(const gsl_vector* v) const {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_zero(gsl_vector_get(v, i))) {
			return 0;
		}
	}
	return 1;
}

// Matrix-by-two-vectors indexing (non-mixed)

gsl_matrix* graphIndicatorBetweennessCentrality::submatrix(const gsl_matrix* m, const gsl_vector* rows,
									const gsl_vector* columns){
	
	if ( m  == NULL ) {
		throw runtime_error("matrix is NULL");
	}
	if ( rows	== NULL ) {
		throw runtime_error("ROWS is NULL");
	}
	if ( columns  == NULL ) {
		throw runtime_error("COLUMN is NULL");
	}
	
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


gsl_vector* graphIndicatorBetweennessCentrality::any(const gsl_matrix* m, int dim ) const {
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

