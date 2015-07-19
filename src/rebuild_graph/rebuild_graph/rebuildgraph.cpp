//
//  rebuildgraph.c
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 14/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//
#include <math.h>
#include <stdio.h>
#include "rebuildgraph.h"
#include "CTrace.hpp"

#include  "FactoryGraphIndicator.h"
#include "graphIndicatorBetweennessCentrality.h"
#include "graphIndicatorCommunicabilityCentralityUsingMatrixExponential.h"

char* CRebuildGraph::getAbrevationFromIndicator(int indicator){
#define BC	"BC"
#define CC	"CC"
#define CBC "CBC"
#define ERROR "ERROR"
	
	
	switch (indicator) {
		case COMMUNICABILITY_CENTRALITY:
			return CC;
			break;
		case BETWEENNESS_CENTRALITY:
			return BC;
			break;
		case COMMUNICABILITY_BETWEENESS_CENTRALITY:
			return CBC;
			break;
		defult:
			return ERROR;
	}
	return ERROR;
}

char* CRebuildGraph::getStringFromIndicator(int indicator){
#define BC_LONG	"Betweenness Centrality"
#define CC_LONG	"Communicability Centrality"
#define CBC_LONG "Communicabilit Betweeness Centrality"
#define ERROR_LONG "ERROR"
	
	
	switch (indicator) {
		case COMMUNICABILITY_CENTRALITY:
			return CC_LONG;
			break;
		case BETWEENNESS_CENTRALITY:
			return BC_LONG;
			break;
		case COMMUNICABILITY_BETWEENESS_CENTRALITY:
			return CBC_LONG;
			break;
		defult:
			return ERROR;
	}
	return ERROR;
}

void CRebuildGraph::generateOutputFile(const  gslGraph *targetGraph,const char *inputFileName,double Tk,
						double costBest,double *targetBC,
						double *bestBC, time_t timeStart, time_t timeEnd,CSettingsSimulation settingSimulation){
	
	FILE *output=NULL;
	int graphOrder =targetGraph->getOrder();
	char outputFilename[STRING_LENGTH];
	strcpy(outputFilename,inputFileName);
	if ( !settingSimulation.outputFileNameSuFix.empty() ) {
		strcat(outputFilename,settingSimulation.outputFileNameSuFix.c_str());
	}
	strcat(outputFilename,".out");
	output=fopen(outputFilename,"w");
	if(output==NULL){
		printf("Cannot open output file %s for writting\n",outputFilename);
		exit(-1);
	}
	fprintf(output,"Graph reconstruction via  vertex betweenness centrality\n");
	fprintf(output,"\tOriginal graph description file-> %s",inputFileName);
	fprintf(output,"\t\tOrdre-> %d\n",targetGraph->getOrder());
	fprintf(output,"\tPseudorandom generator seeds-> %d,%d,%d\n",
			settingSimulation.random_value_x,settingSimulation.random_value_y,settingSimulation.random_value_z);
	if ( settingSimulation.thresholdAccepting  == THRESHOLD_ACCEPTING_ALGORITHM) {
			fprintf(output,"THRESHOLD ACCEPTANCE :\n");
	}else{
			fprintf(output,"SIMULATED ANNEALING:\n");
	}
	fprintf(output,"\tInitial Temperature-> %f\n",settingSimulation.To);
	fprintf(output,"\tTk final-> %f\n",Tk);
	fprintf(output,"\tMinimum Temperature-> %f\n",settingSimulation.tMin);
	fprintf(output,"\tGeometric cooling rate: T[k]=%f * T[k-1]\n",settingSimulation.k);
	fprintf(output,"\tNumber Maxim of Combinations-> %d\n",settingSimulation.nMax);
	fprintf(output,"RESULTS:\n");
	fprintf(output,"\tBest cost -> %3.20f\n",costBest);
	fprintf(output,"\t%s\n",getStringFromIndicator(settingSimulation.graphProperty));
	char * abreviatonIndicator = getAbrevationFromIndicator(settingSimulation.graphProperty);
	fprintf(output,"\t Desired %s  |  Closest %s  | Difference^2\n",abreviatonIndicator,abreviatonIndicator);
	for(int i=0; i<graphOrder; i++){
		fprintf(output,"\t%2.10f | ",targetBC[i]);
		fprintf(output,"%2.10f | ",bestBC[i]);
		fprintf(output,"%2.10f\n",pow(targetBC[i]-bestBC[i],2));
	}
//	fprintf(output,"CPU time needed: %f seconds\n",difftime(timeEnd,timeStart));
	fclose(output);
	
	
}

void CRebuildGraph::CompareAndGenerateResults(CSettingsSimulation settingsSimulation,
											  gslGraph *targetGraph,
											  gslGraph *bestGraph,
											  char* inputFilename,
											  time_t timeStart,
											  double Tk,
											    double *&bestBC,
											  double costBest,
											double &compareResult,
											char *outputGraphFilename
											  ){
	CFuncTrace lFuncTrace(false,string("CRebuildGraph::CompareAndGenerateResults"));
	time_t timeEnd;
	gsl_matrix *targetGraphGsl = gsl_matrix_calloc(targetGraph->getOrder(), targetGraph->getOrder());
	gsl_matrix *bestGraphGsl = gsl_matrix_calloc(bestGraph->getOrder(), bestGraph->getOrder());
	
	targetGraph->graphToGsl(targetGraphGsl);
	bestGraph->graphToGsl(bestGraphGsl);
	
	
	compareResult = this->compareMatrix(targetGraphGsl, bestGraphGsl);
	
	// Processing tasks accomplished
	// Showing results
	
	timeEnd=time(NULL);
	
	lFuncTrace.trace(CTrace::TRACE_DEBUG,"RESULTS:\n");
	lFuncTrace.trace(CTrace::TRACE_DEBUG,"CPU time needed: %f seconds\n",difftime(timeEnd,timeStart));
	// printf("Output file: %s\n",outputFilename);
	
	double *targetBC = NULL;
	{
		graphIndicator * graphIndicator = FactoryGraphIndicator::CreategraphIndicator(settingsSimulation.graphProperty,targetGraph);
		      targetBC = graphIndicator->calculateIndicatorWithReescale(settingsSimulation.reescale);
		delete  graphIndicator;
	}
	
	
	generateOutputFile(targetGraph,inputFilename,  Tk, costBest,targetBC,
					   bestBC, timeStart, timeEnd,settingsSimulation);
	
	lFuncTrace.trace(CTrace::TRACE_DEBUG,"Reconstructed graph file: %s",outputGraphFilename);
	
	bestGraph->printMyGraph(outputGraphFilename,settingsSimulation.outputFormatGraphResultAdjList);

	gsl_matrix_free(bestGraphGsl);
	gsl_matrix_free(targetGraphGsl);
	
}


#define RESULT_OK 1



int CRebuildGraph::calculateBeterness(const char *argv[]){
	CFuncTrace lFuncTrace(false,"fCalculateBeterness");
	gslGraph *targetGraph=NULL;
	int graphOrder=0;
	
//	targetGraph=GetGraphfromFile(argv[1]);
	graphOrder=targetGraph->getOrder();
	
	
	double *targetBC  = NULL;
	
//	targetGraph->brandes_betweenness_centrality(targetBC);

	graphIndicatorBetweennessCentrality *betweennessCentrality =
	new graphIndicatorBetweennessCentrality ( targetGraph );
	
	targetBC = betweennessCentrality->calculateIndicatorWithReescale(true);
	
	
	for (int i = 0; i < graphOrder; i++){
		lFuncTrace.trace(CTrace::TRACE_DEBUG,"%2.10f\n",targetBC[i]);
	}
	
	return RESULT_OK;
}


gsl_vector_complex *
CRebuildGraph::calculateEgeinval (gsl_matrix *target)
{
	
	
	int order = (int)target->size1;
	
	gsl_vector_complex *eval = gsl_vector_complex_alloc (order);
	gsl_matrix_complex *evec = gsl_matrix_complex_alloc (order, order);
	

	gsl_eigen_nonsymmv_workspace * w =
    gsl_eigen_nonsymmv_alloc (order);
	
	gsl_eigen_nonsymmv (target, eval, evec, w);
	
	gsl_eigen_nonsymmv_free (w);
	
	gsl_eigen_nonsymmv_sort (eval, evec,
							 GSL_EIGEN_SORT_ABS_DESC);
	
	{
		int i, j;
		
		for (i = 0; i < order; i++)
		{
			gsl_complex eval_i
			= gsl_vector_complex_get (eval, i);
			gsl_vector_complex_view evec_i
			= gsl_matrix_complex_column (evec, i);
			
			printf ("eigenvalue = %g + %gi\n",
					GSL_REAL(eval_i), GSL_IMAG(eval_i));
			printf ("eigenvector = \n");
			for (j = 0; j < order; ++j)
			{
				/* gsl_complex z = */
				gsl_vector_complex_get(&evec_i.vector, j);
				//				printf("%g + %gi\n", GSL_REAL(z), GSL_IMAG(z));
			}
		}
	}
	
	//	gsl_vector_complex_free(eval);
	gsl_matrix_complex_free(evec);
	
	return eval;
}



int CRebuildGraph::printGslMatrix(gsl_matrix* gslMatrix,const char *format){
	printf("\n");
	for (size_t i = 0; i < gslMatrix->size1; i++) {
		for (size_t j = 0; j < gslMatrix->size2; j++) {
			printf(format, gsl_matrix_get(gslMatrix, i, j));
		}
		
		printf("\n");
	}
	return RESULT_OK;
}



int CRebuildGraph::printGslVector(gsl_vector* gslVector){
	printf("\n");
	for (size_t i = 0; i < gslVector->size; i++) {
		printf("%.03f ", gsl_vector_get(gslVector, i));
		printf("\n");
	}
	return RESULT_OK;
}

// 2014-11-26 calculateExp, may be we should delete?
gsl_vector *
CRebuildGraph::calculateExp(const gsl_vector_complex *eval){
	CFuncTrace lFuncTrace(false,"calculateExp");
	int order = (int)eval->size;
	
	lFuncTrace.trace(CTrace::TRACE_DEBUG,"Ordre for Expo %d",order);
	
	gsl_vector *evalexp = gsl_vector_alloc (order);
	for ( int i = 0; i < order; i++){
		gsl_complex eval_i
		= gsl_vector_complex_get (eval, i);
		//		printf ("eigenvalue = %g + %gi\n",
		//				GSL_REAL(eval_i), GSL_IMAG(eval_i));
		gsl_vector_set(evalexp,i,gsl_sf_exp(GSL_REAL(eval_i)));
		
		printf("W exp %g\n",gsl_sf_exp(GSL_REAL(eval_i)));
		
	}
	return evalexp;
}

int
CRebuildGraph::calculateCommunicability(const char *argv[]){
	CFuncTrace lFuncTrace(false,"fCalculateConnectivity");
	
	gslGraph *targetGraph=NULL;
//	targetGraph=GetGraphfromFile(argv[1]);
	int graphOrder=targetGraph->getOrder();
	lFuncTrace.trace(CTrace::TRACE_DEBUG,"Graph Order %d",graphOrder);
	
	// Get Numpy Matrix // Matriu d'adjacencia
	gsl_matrix *A1=gsl_matrix_calloc(graphOrder,graphOrder);
	
	//targetGraph->printGraph();
	
	targetGraph->graphToGsl(A1);
	
	lFuncTrace.trace(CTrace::TRACE_DEBUG,"Printing Home made Matrix\n");
	printGslMatrix(A1);
	lFuncTrace.trace(CTrace::TRACE_DEBUG,"Printing with gsl function\n");
	gsl_matrix_fprintf(stdout, A1, "%g");
	
	gsl_vector_complex *eval = calculateEgeinval(A1);
	
	/*gsl_vector *evalexp = */ calculateExp(eval);
	
	return RESULT_OK;
}







int
CRebuildGraph::calculateCommunicability_cent_exp(const char *argv[]){
	CFuncTrace lFuncTrace(false,"fCalculateCommunicability_cent_exp");
	
	gslGraph *targetGraph=NULL;
//	targetGraph=GetGraphfromFile(argv[1]);
	int graphOrder=targetGraph->getOrder();
	lFuncTrace.trace(CTrace::TRACE_DEBUG,"Graph Order %d",graphOrder);
//	double * bestCommCentExp = (double*)malloc(graphOrder * sizeof(double));
	
//	targetGraph->brandes_comunicability_centrality_exp(bestCommCentExp);

	graphIndicatorCommunicabilityCentralityUsingMatrixExponential *communicabilityCentrality =
	new graphIndicatorCommunicabilityCentralityUsingMatrixExponential(targetGraph);
	
	double * bestCommCentExp = communicabilityCentrality->calculateIndicatorWithReescale(false);

	return RESULT_OK;
	
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
	 
/*	gsl_blas_dgemm(
				   CblasNoTrans,
				   CblasNoTrans,
				   1.0, A, B, 0.0, C);*/
	gsl_matrix_free(temp);
}

void InitMatrix(int size,
				gsl_vector **work,
				gsl_vector **s,
				gsl_matrix **U1,
				gsl_matrix **U2,
				gsl_matrix **V1,
				gsl_matrix **V2)
{
	*work = gsl_vector_alloc (size);
	*s=gsl_vector_alloc(size);
	*U1=gsl_matrix_alloc(size,size);
	*U2=gsl_matrix_alloc(size,size);
	*V1=gsl_matrix_alloc(size,size);
	*V2=gsl_matrix_alloc(size,size);
}

void FreeMatrix(gsl_vector **work,
				gsl_vector **s,
				gsl_matrix **U1,
				gsl_matrix **U2,
				gsl_matrix **V1,
				gsl_matrix **V2,
				gsl_matrix **F )
{
	gsl_vector_free(*s);
	gsl_vector_free(*work);
	gsl_matrix_free(*U1);
	gsl_matrix_free(*U2);
	gsl_matrix_free(*V1);
	gsl_matrix_free(*V2);
	gsl_matrix_free(*F);

	
}

void CRebuildGraph::printingCompareMatrixResults(float delta,
												 gsl_matrix *F,
												 gsl_matrix* matrixA
												 ){
	CFuncTrace lFuncTrace(false,"CRebuildGraph::printingCompareMatrixResults");
	lFuncTrace.trace(CTrace::TRACE_DEBUG,"DIFERENCIA (delta) -> %f",delta);
	
	//Per presentar-la, definim positiva i normalitzem la matriu F
	if(gsl_matrix_min(F)<0)
		gsl_matrix_add_constant (F, -gsl_matrix_min(F));
	if(gsl_matrix_max(F)>0)
		gsl_matrix_scale (F, 1/gsl_matrix_max(F));
	
	FILE *out;
	out=fopen("sortida.txt","w");
	lFuncTrace.trace(CTrace::TRACE_DEBUG,"Resultats en sortida.txt");
	fprintf(out, "DIFERENCIA (delta) -> %f\n\n",delta);
	for(int i=0; i<matrixA->size1; i++){
		for(int j=0; j<matrixA->size1; j++){
			if(gsl_matrix_get(matrixA,i,j)==0){
				fprintf(out," ");
			}else if(gsl_matrix_get(matrixA,i,j)==1){
				fprintf(out,"#");
			}else{
				printf("\nERROR-Matriu no valida %f",gsl_matrix_get(matrixA,i,j));
				exit(1);
			}
		}
		
		fprintf(out,"\t|\t");
		for(int j=0; j<matrixA->size1; j++){
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

	
}


float
CRebuildGraph::compareMatrix(gsl_matrix* matrixA, gsl_matrix*matrixB){

	
	float delta;
	gsl_vector *work ,*s;
	
	
	if (matrixA->size1 != matrixB->size1)
		throw runtime_error(" size 1 and size 2 are different");
	
	gsl_matrix *U1, *U2,*V1,*V2;
	
	InitMatrix((int)matrixA->size1,&work,&s,&U1,&U2,&V1,&V2);

	gsl_matrix_memcpy (U1, matrixA);
	//gsl_linalg_SV_decomp (gsl_matrix * A, gsl_matrix * V, gsl_vector * S, gsl_vector * work)
	//La matriu A es substitueix per U a la sortida
	gsl_linalg_SV_decomp(U1,V1,s,work);

	gsl_matrix_memcpy (U2, matrixB);
	gsl_linalg_SV_decomp(U2,V2,s,work);

	//F = U1 VS2 V1^T = U1 U2^T A2 V2 V1^T
	gsl_matrix *F=gsl_matrix_alloc(matrixA->size1,matrixA->size1);
	gsl_matrix_transpose(U2);
	multiplica(F,U1,U2);
	multiplica(F,F,matrixB);
	multiplica(F,F,V2);
	gsl_matrix_transpose(V1);
	multiplica(F,F,V1);

	//F ja esta calculada. Calculem la norma.
	delta=0;
	for(int i=0; i<matrixA->size1; i++){
	for(int j=0; j<matrixA->size1; j++){
		delta+=pow(gsl_matrix_get(matrixA,i,j)-gsl_matrix_get(F,i,j),2);
	}
	}
	delta=std::pow(delta,0.5f);
	delta/=matrixA->size1;

	printingCompareMatrixResults(delta,F,matrixA);
	FreeMatrix(&work,
			   &s,
			  &U1,
			   &U2,
			   &V1,
			   &V2,
			   &F );

	return delta;
}


