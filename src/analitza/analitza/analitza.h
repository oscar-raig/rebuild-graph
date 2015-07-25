//
//  analitza.h
//  analitza
//
//  Created by Oscar Raig Colon on 03/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef analitza_analitza_h
#define analitza_analitza_h

#define VERTEXS 5000
#define DEGREE 40
#define MAXTBL 110

#define READ_GRAPH_OK 1
#define READ_GRAPH_ERROR -1

/*******************************************************************************
 ********************************************************************************
 
 DEFINICIO DELS TIPUS I VARIABLES GLOBALS
 
 ********************************************************************************
 *******************************************************************************/

typedef struct {
	int     name;               /* Nom                                            */
	int     degree;             /* Grau                                           */
	int     nei[DEGREE*3];      /* Veins                                          */
	float   BC;                 /* Beetweenness Centrality                        */
	int     nllista;            /* Llista per al c‡lcul de la                     */
	int     llista[VERTEXS];    /* Beetweenness Centrality                        */
} vert;




extern vert vertex[VERTEXS];       /* Vector de Vertexs                              */
extern int dist[VERTEXS][VERTEXS]; /* Matriu de dist‡ncies                           */
extern int dis[VERTEXS];           /* Vector de dist‡ncies                           */
extern int Nvertexs;               /* Nombre de Vertexs                              */
extern const char *nom;                  /* String on guardem el arxiu d¥entrada           */


int llegir_dades(const char* nom, int &linies,int &maxveins,int &minveins, float &mitja );
int distancies( float *clusTotal, float *diametreMax,float *disMitjaTotal);
int be_ce();
float clustering(int vert);
#endif
