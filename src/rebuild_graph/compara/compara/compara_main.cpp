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
#include "boost/program_options.hpp"
#include <exception>
#include <stdexcept>
#include "gslGraph.h"

using std::runtime_error;
/********************************* FUNCIO PRINCIPAL ********************************/

const char *graphFile1 = NULL;
const char *graphFile2 = NULL;
int formatFile = 0;
#define OLD_FORMAT 0

int  readConfiguration(int argc, const char * argv[] ){

	char formatdArgumentDescription[50];
	sprintf(formatdArgumentDescription,"%d: 1: adj list ",OLD_FORMAT);

	namespace po = boost::program_options;
	po::options_description argumentDescription("Options");
	argumentDescription.add_options()
	("help", "Print help messages" )
	("graphFile1", po::value< std::string >(),"graph file in python format")
	("graphFile2", po::value< std::string >(),"graph file in python format")
	
	("format",po::value<int>(),formatdArgumentDescription);
	
	
	po::variables_map argumentMap;
	po::store(po::parse_command_line(argc, argv, argumentDescription),
			  argumentMap);
	po::notify(argumentMap);
	if ( argumentMap.count("help")  )
	{
		std::cout << "Basic Command Line Parameter App" << std::endl
		<< argumentDescription << std::endl;
		
		return 0;
	}
	if ( !argumentMap.count("graphFile1")){
		throw runtime_error("grpah File 1 is not specified");
		return -1;
	}else{
		std::cout << "argumentMap count" << argumentMap.count("graphFile1");
		std::cout << "graphFile1 " <<  argumentMap["graphFile1"].as<std::string>() << std::endl;
	}
	if ( !argumentMap.count("graphFile2")){
		throw runtime_error("grpah File 2 is not specified");
		return -1;
	}else{
		std::cout << "argumentMap count" << argumentMap.count("graphFile2");
		std::cout << "graphFile2 " <<  argumentMap["graphFile2"].as<std::string>() << std::endl;
	}
	
	if ( argumentMap.count("format")  )
	{
		std::cout << "formatFile" << std::endl;
		formatFile = argumentMap["format"].as<int>();
	}
	
	graphFile1 =  argumentMap["graphFile1"].as<std::string>().c_str() ;
	graphFile2 =  argumentMap["graphFile2"].as<std::string>().c_str() ;
	
	return 1;
}

void printGslMatrix(const char*FileName,gsl_matrix* gslMatrix,const char *format){
	FILE *out;
	out=fopen(FileName,"w");

	printf("\n");
	for (size_t i = 0; i < gslMatrix->size1; i++) {
		for (size_t j = 0; j < gslMatrix->size2; j++) {
			fprintf(out,format, gsl_matrix_get(gslMatrix, i, j));
		}
		
		fprintf(out,"\n");
	}
}

void printGraph1(gsl_matrix * F){
	printGslMatrix("sortida.ini.txt",F,"%.0f ");
}

void printGraph2(gsl_matrix * F){
	printGslMatrix("sortida.res.txt",F,"%.0f ");
}

void printComparasion(gsl_matrix *F,int order1){
		printGslMatrix("sortida.cmp.txt",F,"%.2f ");
}

void printGraph1Graph2AndComparasionInVisualMode(gsl_matrix* A1, gsl_matrix *F,int order1){
	float delta;
	FILE *out;
	out=fopen("sortida.txt","w");
	printf("\nResultats en sortida.txt");
	fprintf(out, "DIFERENCIA (delta) -> %f\n\n",delta);
	
	int i,j;
	for(i=0; i<order1; i++){
		for(j=0; j<order1; j++){
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
		for(j=0; j<order1; j++){
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


int main(int argc, const char *argv[])
{
	CFuncTrace trace(CTrace::TRACE_INFO,"main");
	int i,j;
	TGraf graf1,graf2;
	float delta;
	
	try{
		int order1=0;
		int order2=0;
		gsl_matrix *A1 = NULL;
		gsl_matrix *A2=  NULL;
		if ( readConfiguration(argc,argv)<= 0 ){
			return 0;
		}
		if ( formatFile == OLD_FORMAT){
			carrega_matriu(&graf1, (char*)graphFile1);
			carrega_matriu(&graf2, (char*)graphFile2);

			
			order1= graf1.ordre;
			order2= graf2.ordre;
			A1=gsl_matrix_alloc(order1,order1);
			A2=gsl_matrix_alloc(order1,order1);
			
			omple_matriu(&graf1,A1);
			omple_matriu(&graf2,A2);
		}else{
			trace.trace(CTrace::TRACE_INFO,"Reading adjlist file"); 
			gslGraph * graph1 = new gslGraph();
			gslGraph * graph2 = new gslGraph();
			graph1->readPythonGraphFile(graphFile1);
			graph2->readPythonGraphFile(graphFile2);
			order1 = graph1->getOrder();
			order2 =  graph1->getOrder();
			A1=gsl_matrix_alloc(order1,order1);
			A2=gsl_matrix_alloc(order1,order1);
			graph1->graphToGsl(A1);
			graph2->graphToGsl(A2);
			
			delete graph1;
			delete graph2;
			
		}
		
		if( order1 != order2 ){
			printf("\n\nERROR-Els dos fitxers proporcionats tenen ordres diferents");
			exit(-1);
		}

		gsl_vector *work = gsl_vector_alloc (order1);
		gsl_vector *s=gsl_vector_alloc(order1);
		gsl_matrix *U1=gsl_matrix_alloc(order1,order1);
		gsl_matrix *U2=gsl_matrix_alloc(order1,order1);
		gsl_matrix *V1=gsl_matrix_alloc(order1,order1);
		gsl_matrix *V2=gsl_matrix_alloc(order1,order1);
		

		gsl_matrix_memcpy (U1, A1);
		//gsl_linalg_SV_decomp (gsl_matrix * A, gsl_matrix * V, gsl_vector * S, gsl_vector * work)
		//La matriu A es substitueix per U a la sortida
		gsl_linalg_SV_decomp(U1,V1,s,work);

		gsl_matrix_memcpy (U2, A2);
		gsl_linalg_SV_decomp(U2,V2,s,work);

		//F = U1 VS2 V1^T = U1 U2^T A2 V2 V1^T
		gsl_matrix *F=gsl_matrix_alloc(order1,order1);
		gsl_matrix_transpose(U2);
		multiplica(F,U1,U2);
		multiplica(F,F,A2);
		multiplica(F,F,V2);
		gsl_matrix_transpose(V1);
		multiplica(F,F,V1);

		//F ja esta calculada. Calculem la norma.
		delta=0;
		for(i=0; i<order1; i++){
			for(j=0; j<order1; j++){
				delta+=pow(gsl_matrix_get(A1,i,j)-gsl_matrix_get(F,i,j),2);
			}
		}
		delta=std::pow(delta,0.5f);
		delta/=order1;

		printf("\nDIFERENCIA (delta) -> %f",delta);

		//Per presentar-la, definim positiva i normalitzem la matriu F
		if(gsl_matrix_min(F)<0)
			gsl_matrix_add_constant (F, -gsl_matrix_min(F));
		if(gsl_matrix_max(F)>0)
			gsl_matrix_scale (F, 1/gsl_matrix_max(F));

		printGraph1Graph2AndComparasionInVisualMode(A1,F,order1);
		printComparasion(F,order1);
		printGraph1(A1);
		printGraph2(A2);
		
					
		gsl_vector_free(work);
		gsl_matrix_free(U1);
		gsl_matrix_free(U2);
		gsl_matrix_free(V1);
		gsl_matrix_free(V2);
		gsl_matrix_free(A1);
		gsl_matrix_free(F);
	}catch( std::exception &e){
		std::cout << "ERROR :" << e.what()<< std::endl;

	}
	return 0;
}
