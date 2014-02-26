/********************************* LLIBRERIES *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <memory.h>

#include <gsl/gsl_matrix.h>  //Per la descomposicio en valors singulars

/********************************* CONSTANTS **********************************/
/* Constants dels grafs */
#define MAX_VERTEX 350
#define MAX_VEINS 50

/********************************* ESTRUCTURES ********************************/
/* Estructura per contenir un graf */
typedef struct{
    int ordre; /* Nombre de vèrtexs */
    int grau_max; /* Màxim nombre de veïns d'un vèrtex */
    int conn[MAX_VERTEX][MAX_VEINS];
    int grau[MAX_VERTEX];
    int nzero; /* Nombre de posicions de la matriu d'adj que no son zero (sumatori de graus) */
}TGraf;

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
void omple_matriu(TGraf *graf, gsl_matrix *m){
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

/********************************* FUNCIO PRINCIPAL ********************************/

int main(int argc, char *argv[])
{
  int i,j;
  TGraf graf1,graf2;
  float delta;
  
  printf("COMPARA.EXE - Aplica la transformacio F=U1^T U2 A2 V2^T V1 per tal de");
  printf("\n              comparar dos grafs.");
  printf("\n              COMPARA.EXE fitxer_graf1 fitxer_graf2");
  switch(argc){
     case 3: break;
     default: exit(1);
  }
  
  carrega_matriu(&graf1, argv[1]);
  carrega_matriu(&graf2, argv[2]);
  
  if(graf1.ordre!=graf2.ordre){
     printf("\n\nERROR-Els dos fitxers proporcionats tenen ordres diferents");
     exit(-1);
  }
  
  gsl_vector *work = gsl_vector_alloc (graf1.ordre);
  gsl_vector *s=gsl_vector_alloc(graf1.ordre);
  gsl_matrix *U1=gsl_matrix_alloc(graf1.ordre,graf1.ordre);
  gsl_matrix *U2=gsl_matrix_alloc(graf1.ordre,graf1.ordre);
  gsl_matrix *V1=gsl_matrix_alloc(graf1.ordre,graf1.ordre);
  gsl_matrix *V2=gsl_matrix_alloc(graf1.ordre,graf1.ordre);
  gsl_matrix *A1=gsl_matrix_alloc(graf1.ordre,graf1.ordre);
  gsl_matrix *A2=gsl_matrix_alloc(graf1.ordre,graf1.ordre);

  omple_matriu(&graf1,A1);
  omple_matriu(&graf2,A2);
  
  
  gsl_matrix_memcpy (U1, A1);
  //gsl_linalg_SV_decomp (gsl_matrix * A, gsl_matrix * V, gsl_vector * S, gsl_vector * work) 
  //La matriu A es substitueix per U a la sortida
  gsl_linalg_SV_decomp(U1,V1,s,work);
  
  gsl_matrix_memcpy (U2, A2);
  gsl_linalg_SV_decomp(U2,V2,s,work);
  
  //F = U1 VS2 V1^T = U1 U2^T A2 V2 V1^T
  gsl_matrix *F=gsl_matrix_alloc(graf1.ordre,graf1.ordre);
  gsl_matrix_transpose(U2);
  multiplica(F,U1,U2);
  multiplica(F,F,A2);
  multiplica(F,F,V2);
  gsl_matrix_transpose(V1);
  multiplica(F,F,V1);
  
  //F ja esta calculada. Calculem la norma.
  delta=0;
  for(i=0; i<graf1.ordre; i++){
     for(j=0; j<graf1.ordre; j++){
        delta+=pow(gsl_matrix_get(A1,i,j)-gsl_matrix_get(F,i,j),2);
     }
  }
  delta=pow(delta,0.5);
  delta/=graf1.ordre;
  
  printf("\nDIFERENCIA (delta) -> %f",delta);
 
  //Per presentar-la, definim positiva i normalitzem la matriu F
  if(gsl_matrix_min(F)<0)
     gsl_matrix_add_constant (F, -gsl_matrix_min(F));
  if(gsl_matrix_max(F)>0)
     gsl_matrix_scale (F, 1/gsl_matrix_max(F)); 
  
  FILE *out;
  out=fopen("sortida.txt","w");
  printf("\nResultats en sortida.txt");
  fprintf(out, "DIFERENCIA (delta) -> %f\n\n",delta);
  for(i=0; i<graf1.ordre; i++){
     for(j=0; j<graf1.ordre; j++){
        if(gsl_matrix_get(A1,i,j)==0)
           fprintf(out," ");
        else if(gsl_matrix_get(A1,i,j)==1)
           fprintf(out,"#");
        else{
           printf("\nERROR-Matriu no valida");
           exit(1);
           }
     }
     fprintf(out,"\t|\t");
     for(j=0; j<graf1.ordre; j++){
        if(gsl_matrix_get(F,i,j)<0.2)
           fprintf(out," ");
        else if(gsl_matrix_get(F,i,j)<0.4)
           fprintf(out,"·");
        else if(gsl_matrix_get(F,i,j)<0.6)
           fprintf(out,"^");
        else if(gsl_matrix_get(F,i,j)<0.8)
           fprintf(out,"-");
        else if(gsl_matrix_get(F,i,j)<0.95)
           fprintf(out,"/");
        else
           fprintf(out,"#");
     }
     fprintf(out,"\n");
  }
  fclose(out);



 out=fopen("ini.txt","w");
  printf("\nGraf inicial en Mathematica Table format"); 
  for(i=0; i<graf1.ordre; i++)
     {
     for(j=0; j<graf1.ordre; j++)
           fprintf(out,"%1.2f ",gsl_matrix_get(A1,i,j));
     fprintf(out,"\n");
     }
  fclose(out);
 
  out=fopen("fin.txt","w");
  printf("\nGraf final en Mathematica Table format"); 
  for(i=0; i<graf1.ordre; i++)
     {
     for(j=0; j<graf1.ordre; j++)
           fprintf(out,"%1.2f ",gsl_matrix_get(F,i,j));
     fprintf(out,"\n");
     }
  fclose(out);
 

  gsl_vector_free(work);
  gsl_matrix_free(U1);
  gsl_matrix_free(U2);
  gsl_matrix_free(V1);
  gsl_matrix_free(V2);
  gsl_matrix_free(A1);
  gsl_matrix_free(F);
  
  return 0;
}


