/********************************* LLIBRERIES *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <memory.h>
#include "compara.h"


#include <matrix/gsl_matrix.h>  //Per la descomposicio en valors singulars


/******************************************************************************/
/* Carrega en una estructura TGraf la matriu descrita en el fitxer *nom
   Omple l'ordre, el grau_max, conn[][] i grau[]. Torna el nombre de posicions que
   no són zero a la matriu d'adjacencies.
 */
int carrega_matriu(TGraf *graf, char *nom){
	FILE *f_llista;
	char c;
	int i,j,k,temp,val,linies=0, maxveins=0, veins=0, posicio=0;
	int nozero=0;
	
	if ((f_llista=fopen(nom, "rt"))==NULL){printf("\nERROR-Impossible obrir fitxer %s\n",nom); exit(-1); }

	/*	Mirar el nombre de vèrtexs que té la xarxa (#línies) i 
		el grau màxim (#max de nombres en una línia).*/
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
	/*	En aquest punt, la variable 'linies' conté el nombre de vèrtexs, 
	i 'maxveins' el grau màxim. Guardem aquestes dades a l'estructura. */
	/*  Comprovem que la matriu d'adjacencies pot encabir la xarxa del fitxer i la inicialitzem */
	if((linies>MAX_VERTEX)||(maxveins>MAX_VEINS)){ printf("\nERROR-La xarxa descrita en el fitxer %s és massa complicada",nom);exit(-1);}
	else{
        graf->ordre=linies;
      	graf->grau_max=maxveins;
      	}

	/*	Obrim de nou el fitxer i omplim */
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

//Passa la llista d'adjacencies del graf a una matriu d'adjacencies
void
omple_matriu(TGraf *graf, gsl_matrix *m){
   int i,j;
   gsl_matrix_set_zero(m);
   
   for(i=0; i<graf->ordre; i++){
      for(j=0; j<graf->grau[i]; j++){
         gsl_matrix_set (m, i, graf->conn[i][j], 1);
      }
   }
}

//Multiplica matrius quadrades
void multiplica(gsl_matrix *result, gsl_matrix *m1, gsl_matrix *m2){
   int i,j,k;
   double resultat;
   gsl_matrix *temp=gsl_matrix_alloc(m1->size1,m1->size1);
   gsl_matrix_set_zero(temp);
   
   for(i=0; i<m1->size1; i++){
      for(j=0; j<m1->size1;j++){
         resultat=0;
         for(k=0; k<m1->size1; k++){
            resultat+=gsl_matrix_get(m1,i,k)*gsl_matrix_get(m2,k,j);
         }
         gsl_matrix_set(temp,i,j,resultat);
      }
   }
   
   gsl_matrix_memcpy (result, temp);
}


