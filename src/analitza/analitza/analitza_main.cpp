//
//  main.cpp
//  analitza
//
//  Created by Oscar Raig Colon on 03/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include <iostream>
#include "analitza.h"





/*******************************************************************************
 ********************************************************************************
 
 FUNCIO PRINCIPAL
 
 AQUESTA FUNCI” L¥UNIC QUE FA ES CRIDAR A LES ALTRES FUNCIONS.
 
 1.- LLEGIM LES DADES
 2.- CALCULEM LES DIST¿NCIES I EL CLUSTERING
 3.- CALCULEM LA BETWEENESS CENTRALITY
 
 ********************************************************************************
 *******************************************************************************/

int main(int argc, const char *argv[]){
    
    
    /* llegir el nom del fitxer de la comanda de linia                            */
    const char *nom = NULL;                  /* String on guardem el arxiu d¥entrada           */
    if (argc!=2) {
        printf("analitza: falta el nom del fitxer a llegir\n");
        exit(1);
    }
    
    nom = argv[1];
    int order=0,maxveins=0,minveins=0;
    float mitja = 0.0;
    llegir_dades(nom,order,maxveins,minveins,mitja);
    be_ce();
    float f1, f2, f3;
    distancies(&f1,&f2,&f3);
    
    return 0;
}

