//
//  rebuildgraph.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 14/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef rebuild_graph_rebuildgraph_h
#define rebuild_graph_rebuildgraph_h

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_linalg.h>

#include "CTrace.hpp"

// Simulated Annealing
#define TEMPER_INITIAL_DEFAULT 10
#define TEMPER_MIN_DEFAULT 0.0001
#define NUMBER_MAX_COMBINATIONS_DEFAULT 1000
#define K 0.9
#define TOL 0.0001
#define MAX_ITERATIONS 1000
#define STRING_LENGTH 256

// Park-Miller pseudo random  number generator
#define LLAVOR_X 11
#define LLAVOR_Y 92
#define LLAVOR_Z 37


#endif
