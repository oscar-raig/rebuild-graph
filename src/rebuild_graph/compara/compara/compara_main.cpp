//
//  main.cpp
//  compara
//
//  Created by Oscar Raig Colon on 02/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include <iostream>
#include "compara.h"
#include <math.h>
#include <matrix/gsl_matrix.h>  //Per la descomposicio en valors singulars
#include <linalg/gsl_linalg.h>
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
				fprintf(out,"∑");
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
	
	
	gsl_vector_free(work);
	gsl_matrix_free(U1);
	gsl_matrix_free(U2);
	gsl_matrix_free(V1);
	gsl_matrix_free(V2);
	gsl_matrix_free(A1);
	gsl_matrix_free(F);
	
	return 0;
}
