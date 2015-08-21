//
//  compara.h
//  compara
//
//  Created by Oscar Raig Colon on 02/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef compara_compara_h
#define compara_compara_h

#include <matrix/gsl_matrix.h> 

/********************************* CONSTANTS **********************************/
/* Constants dels grafs */
#define MAX_VERTEX 2500
#define MAX_VEINS 50

/********************************* ESTRUCTURES ********************************/
/* Estructura per contenir un graf */
typedef struct{
    int ordre; /* Nombre de vËrtexs */
    int grau_max; /* M‡xim nombre de veÔns d'un vËrtex */
    int conn[MAX_VERTEX][MAX_VEINS];
    int grau[MAX_VERTEX];
    int nzero; /* Nombre de posicions de la matriu d'adj que no son zero (sumatori de graus) */
}TGraf;

int carrega_matriu(TGraf *graf, char *nom);
void omple_matriu(TGraf *graf, gsl_matrix *m);
void multiplica(gsl_matrix *result, gsl_matrix *m1, gsl_matrix *m2);

#endif
