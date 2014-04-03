//
//  main.cpp
//  analitza
//
//  Created by Oscar Raig Colon on 03/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include <iostream>
#include "analitza.h"

vert vertex[VERTEXS];       /* Vector de Vertexs                              */
int dist[VERTEXS][VERTEXS]; /* Matriu de dist‡ncies                           */
int dis[VERTEXS];           /* Vector de dist‡ncies                           */
int Nvertexs;               /* Nombre de Vertexs                              */
const char *nom;                  /* String on guardem el arxiu d¥entrada           */



/*******************************************************************************
 ********************************************************************************
 
 FUNCIO PRINCIPAL
 
 AQUESTA FUNCI” L¥UNIC QUE FA ES CRIDAR A LES ALTRES FUNCIONS.
 
 1.- LLEGIM LES DADES
 2.- CALCULEM LES DIST¿NCIES I EL CLUSTERING
 3.- CALCULEM LA BETWEENESS CENTRALITY
 
 ********************************************************************************
 *******************************************************************************/

int main(int argc, const char *argv[])

{
	int i;
	
	/* llegir el nom del fitxer de la comanda de linia                            */
	
	if (argc!=2) { printf("analitza: falta el nom del fitxer a llegir\n"); exit(1); }
	
	for (i = 1; i < argc; i++)  nom = argv[i];
	
	llegir_dades();
	i=be_ce();
	i=distancies();
	
}

