//
//  main.cpp
//  convert_graph_file
//
//  Created by Oscar Raig Colon on 02/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include <iostream>


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

/******************************************************************************/
/* Carrega en una estructura TGraf la matriu descrita en el fitxer *nom
 Omple l'ordre, el grau_max, conn[][] i grau[]. Torna el nombre de posicions que
 no sÛn zero a la matriu d'adjacencies.
 */
int carrega_matriu(TGraf *graf, const char *nom){
    FILE *f_llista;
    char c;
    int i,j,k,temp,val,linies=0, maxveins=0, veins=0, posicio=0;
    int nozero=0;
    
    if ((f_llista=fopen(nom, "rt"))==NULL){printf("\nERROR-Impossible obrir fitxer %s\n",nom); exit(-1); }
    
    /*  Mirar el nombre de vËrtexs que tÈ la xarxa (#lÌnies) i
     el grau m‡xim (#max de nombres en una lÌnia).*/
    while (!feof(f_llista)) {
        c=fgetc(f_llista);
        posicio++;
        if (!feof(f_llista) && c==' ')veins++;
        if (!feof(f_llista) && c=='\n') {
            veins++;
            if (veins>maxveins) maxveins=veins;
            if (posicio>1) linies++;
            posicio=0;
            veins=0;
        }
    }
    fclose(f_llista);
    /*  En aquest punt, la variable 'linies' contÈ el nombre de vËrtexs,
     i 'maxveins' el grau m‡xim. Guardem aquestes dades a l'estructura. */
    /*  Comprovem que la matriu d'adjacencies pot encabir la xarxa del fitxer i la inicialitzem */
    if((linies>MAX_VERTEX)||(maxveins>MAX_VEINS)){ printf("\nERROR-La xarxa descrita en el fitxer %s Ès massa complicada",nom);exit(-1);}
    else{
        graf->ordre=linies;
        graf->grau_max=maxveins;
    }
    
    /*  Obrim de nou el fitxer i omplim */
    if ((f_llista=fopen(nom, "rt"))==NULL){printf("\nERROR-Impossible obrir fitxer %s\n",nom); exit(-1); }
    i=0;
    j=0;
    while (!feof(f_llista) && i<linies) {
        fscanf(f_llista, "%i", &val);
        if(j==0)
            graf->conn[i][j]=val;
        else{
            k=j-1;
            temp=graf->conn[i][k];
            while((temp>val)&&(k>=0)){
                graf->conn[i][k+1]=temp;
                k--;
                if(k>=0) temp=graf->conn[i][k];
            }
            graf->conn[i][k+1]=val;
        }
        j++;
        fscanf(f_llista, "%c", &c);
        if (c=='\n'){graf->grau[i]=j;nozero+=j;i++;j=0;}
    }
    fclose(f_llista);
    graf->nzero=nozero;
    return nozero;
}

void
print_TGraf(const TGraf *graf){
    
    int i,j;
    
    
    for(i=0; i<graf->ordre; i++){
        
        std::cout << i;
        for(j=0; j<graf->grau[i]; j++){
            if (graf->conn[i][j] > i ){
            
                std::cout << " " << graf->conn[i][j];
            }
        }
        
        std::cout << std::endl;
    }
    
}

int main(int argc, const char * argv[])
{
    TGraf graf1; 
    if (argc <2 )
    {
        printf("Usage convert_graph_file  name_of_file.old\n");
        return 1;
    }
    
    if (argv[1] == NULL)
    {
        printf("File name is null\n");
        return 1;
    }

    carrega_matriu(&graf1, argv[1]);
    
    print_TGraf(&graf1);
    
    return 0;
}

