//
//  rebuildgraph.c
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 14/04/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//
#include <math.h>
#include <stdio.h>
#include "graphs.h"
#include "rebuildgraph.h"
#include "CTrace.hpp"
/*
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_cblas.h> */
//-----------------------------------------------------------------------------
// GLOBAL operation readPythonGraphFile(char *fileName)
// RETURNS a graph pointer
//-----------------------------------------------------------------------------
graph *CRebuildGraph::readPythonGraphFile(char *fileName){
	FILE *input;
	int i=0;
	int vertex_identifier=0;
	int vertex_neighbour=0;
	char *line;
	char *aux,*newaux;
	
	graph *result=new graph();
	line=(char *)malloc(sizeof(char)*STRING_LENGTH);
	if((input=fopen(fileName,"rt"))==NULL){
		printf("file not found %s\n",fileName);
		char szPath[255];
		if (!getwd(szPath)){
			printf("Current path (%s)",szPath);
		}
		exit(1);
	}
	line[0]='\0';
	line=fgets(line,STRING_LENGTH,input);
	while(line!=NULL){
		if(line!=NULL && line[0]!='#'){
			vertex_identifier=(int)strtol(&line[i],&aux,10);
			result->addVertex(vertex_identifier);
			newaux=aux;
			do{
				aux=newaux;
				vertex_neighbour=(int)strtol(aux,&newaux,10);
				if(newaux-aux!=0){
					result->addVertexNeighbour(vertex_identifier,vertex_neighbour);
				}
			} while(aux!=newaux);
		}
		line[0]='\0';
		line=fgets(line,STRING_LENGTH,input);
	}
	fclose(input);
	free(line);
	result->updateDistanceMatrix();
	return result;
}

//-----------------------------------------------------------------------------
// GLOBAL operation generateRandomNumber()
// Wichmann-Hill method to generate random numbers (needs 3 seed numbers)
// RETURNS a random double [0,1(
//-----------------------------------------------------------------------------
double
CRebuildGraph::generateRandomNumber(int &random_value_x,int &random_value_y, int &random_value_z){
	
	double temp;
	random_value_x=171*(random_value_x%177)-2*(random_value_x/177);
	if(random_value_x<0)
		random_value_x+=30269;
	random_value_y=172*(random_value_y%176)-35*(random_value_y/176);
	if(random_value_y<0)
		random_value_y+=30307;
	random_value_z=170*(random_value_z%178)-63*(random_value_z/178);
	if(random_value_z<0)
		random_value_z+=30323;
	temp=random_value_x/30269.0+random_value_y/30307.0+random_value_z/30323.0;
	return(temp-(int)temp);
}


//-----------------------------------------------------------------------------
// GLOBAL operation generateInitialGraph(int sourceGraphOrder)
// RETURNS a random graph with sourceGraphOrder vertex
//-----------------------------------------------------------------------------
graph *CRebuildGraph::generateInitialGraph(int sourceGraphOrder,int &random_value_x,int &random_value_y,int &random_value_z){
	int i,j;
	int newNeighbour;
	int newDegree;
	graph *result=(graph *)new graph(sourceGraphOrder);
	
	for(i=0; i<sourceGraphOrder; i++){
		// The new vertex degree is to be
		//   at least 1 (the graphs needs to be connected)
		//   at most n-1 (the vertex is connected to every other vertex
		if(result->vertexArray[i]->degree==0){
			// vertex i has no neighbours yet
			newDegree=1+(int)(generateRandomNumber(random_value_x,random_value_y,random_value_z)*(sourceGraphOrder-1));
			// newDegre is in [1,n-1]
		} else if(result->vertexArray[i]->degree<(sourceGraphOrder-1)){
			// vertex i is connected to some other vertex
			newDegree=(int)(generateRandomNumber(random_value_x,random_value_y,random_value_z)*
							(sourceGraphOrder-result->vertexArray[i]->degree));
			// newDegree is in [0,n-1-degree]
		} else {
			// vertex i is already connected to all possible neighbours
			newDegree=0;
		}
		for(j=0;j<newDegree;j++){
			do{
				newNeighbour=(int)(generateRandomNumber(random_value_x,random_value_y,random_value_z)*(sourceGraphOrder));
				// newNeighbour is in [0,n-1]
			} while(result->vertexAreNeighbours(i,newNeighbour));
			result->addVertexNeighbour(i,newNeighbour);
		}
	}
	return result;
}


//-----------------------------------------------------------------------------
// GLOBAL operation copyGraph(sourceGraph)
// RETURNS a copy of sourceGraph
//-----------------------------------------------------------------------------
graph *CRebuildGraph::copyGraph(graph *sourceGraph){
	int i,myDegree,*myNeighbours;
	int myOrder=sourceGraph->getOrder();
	graph *result=(graph *)new graph(myOrder);
	for(i=0;i<myOrder;i++){
		myDegree=sourceGraph->getVertexNeighbours(i,&myNeighbours);
		result->addVertexNeighbours(i,myNeighbours,myDegree);
		free(myNeighbours);
	}
	return result;
}

//-----------------------------------------------------------------------------
// GLOBAL operation copyGraph(sourceGraph,targetGraph)
// COPIES sourceGraph to targetGraph
//-----------------------------------------------------------------------------
void CRebuildGraph::copyGraph(graph *sourceGraph,graph *targetGraph){
	int i,j,aux;
	int myOrder=sourceGraph->getOrder();
	
	for(i=0;i<myOrder;i++){
		aux=sourceGraph->vertexArray[i]->degree;
		targetGraph->vertexArray[i]->degree=aux;
		for(j=0;j<aux;j++){
			targetGraph->vertexArray[i]->neighbours[j]=
			sourceGraph->vertexArray[i]->neighbours[j];
		}
		for(j=i+1;j<myOrder;j++){
			aux=sourceGraph->distanceMatrix[i][j];
			targetGraph->distanceMatrix[i][j]=aux;
			targetGraph->distanceMatrix[j][i]=aux;
		}
	}
	
}

//-----------------------------------------------------------------------------
// GLOBAL operation modifyGraph(sourceGraph)
// MODIFIES a randon sourceGraph vertex's connections
//-----------------------------------------------------------------------------
void CRebuildGraph::modifyGraph(graph *sourceGraph,int &random_value_x,int &random_value_y,int &random_value_z){
	int i,j;
	int vertex2change;
	int myOrder=sourceGraph->getOrder();
	int myNewNeighbour;
	int myNewNumberOfNeighbours;
	int newNeighbours[myOrder];
	int found;
	
	// Select vertex to change
	vertex2change=(int)(generateRandomNumber(random_value_x,random_value_y,random_value_z)*(myOrder));
	// vertex2change is in [0,n-1]
	// Disconnect vertex2change
	//printf("modifyGraph\n");
	//sourceGraph->printGraph();
	//printf("modifyGraph remove vertex %d\n",vertex2change);
	sourceGraph->removeVertexNeighbours(vertex2change);
	//sourceGraph->printGraph();
	//printf("modifyGraph vertex removed\n");
	do{
		//Choose new vertex degree
		myNewNumberOfNeighbours=1+(int)(generateRandomNumber(random_value_x,random_value_y,random_value_z)*(myOrder-1));
		// myNewNumberOfNeighbours is in [1,n-1]
		// Connect new neighbours
		for(i=0; i<myNewNumberOfNeighbours; i++){
			//printf("modifyGraph3 vertex2change %d\n",vertex2change);
			//printf("modifyGraph3 Newdegree %d\n",myNewNumberOfNeighbours);
			do{
				found=false;
				//printf("modifyGraph4\n");
				myNewNeighbour=(int)(generateRandomNumber(random_value_x,random_value_y,random_value_z)*(myOrder));
				// myNewNeighbour is in [0,n-1]
				for(j=0;j<i;j++){
					//printf("modifyGraph4 new=%d newNei[j]=%d\n",myNewNeighbour,newNeighbours[j]);
					if((myNewNeighbour==newNeighbours[j])
					   ||(myNewNeighbour==vertex2change)){
						found=true;
					}
				}
			} while(found||
					(sourceGraph->vertexAreNeighbours(vertex2change,myNewNeighbour)));
			newNeighbours[i]=myNewNeighbour;
			//printf("modifyGraph5 vertex %d ADD neigh %d\n",vertex2change,myNewNeighbour);
			sourceGraph->addNewVertexNeighbour(vertex2change,myNewNeighbour);
		}
		//    sourceGraph->printGraph();
	} while (sourceGraph->graphNotConnected(&vertex2change));
	//sourceGraph->printGraph();
	//printf("modifyGraph vertex reconnected\n");
}


//-----------------------------------------------------------------------------
// GLOBAL operation cost(double *tarjet,double *current,int count)
// RETURNS the cost
//-----------------------------------------------------------------------------
double CRebuildGraph::cost(double *tarjet,double *current,int count){
	int i;
	double result=0.0;
	
	for(i=0; i<count; i++) {
		result+=pow((tarjet[i]-current[i]),2);
	}
	return pow(result,0.5);
}

void CRebuildGraph::generateOutputFile(const  graph *targetGraph,const char *inputFileName,double Tk,
						double costBest,double *targetBC,
						double *bestBC, time_t timeStart, time_t timeEnd,CSettingsSimulation settingSimulation){
	
	FILE *output=NULL;
	int graphOrder =targetGraph->getOrder();
	char outputFilename[STRING_LENGTH];
	strcpy(outputFilename,inputFileName);
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
	fprintf(output,"SIMULATED ANNEALING:\n");
	fprintf(output,"\tInitial Temperature-> %f\n",settingSimulation.To);
	fprintf(output,"\tTk final-> %f\n",Tk);
	fprintf(output,"\tMinimum Temperature-> %f\n",settingSimulation.tMin);
	fprintf(output,"\tGeometric cooling rate: T[k]=%f * T[k-1]\n",settingSimulation.k);
	fprintf(output,"\tNumber Maxim of Combinations-> %d\n",settingSimulation.nMax);
	fprintf(output,"RESULTS:\n");
	fprintf(output,"\tBest cost -> %3.20f\n",costBest);
	fprintf(output,"\tBetweenness centrality\n");
	fprintf(output,"\t Desired BC  |  Closest BC  | Difference^2\n");
	for(int i=0; i<graphOrder; i++){
		fprintf(output,"\t%2.10f | ",targetBC[i]);
		fprintf(output,"%2.10f | ",bestBC[i]);
		fprintf(output,"%2.10f\n",pow(targetBC[i]-bestBC[i],2));
	}
//	fprintf(output,"CPU time needed: %f seconds\n",difftime(timeEnd,timeStart));
	fclose(output);
	
	
}

void CRebuildGraph::AnnealingAlgorithm(double &Tk, graph **pbestGraph,int graphOrder,
						double *bestBC,double *targetBC,
						FILE *logFile,double &costBest,
						CSettingsSimulation settingSimulation){
	
	CFuncTrace lFuncTrace(false,"CRebuildGraph::AnnealingAlgorithm");
	double temperMin=TEMPER_MIN_DEFAULT;
	double k=K;
	int iterations=0;
	double tol=TOL;
	int weAreDone=0;
	costBest=0.0;
	double costOld=0.0;
	double costNew=0.0;
	long int N=0;
	graph * bestGraph= NULL;
	double newBC [graphOrder];
	graph *newGraph=NULL;
	graph *oldGraph=NULL;
	
	for(int i=0;i<graphOrder;i++){
		bestBC[i]=0.0;
		newBC[i]=0.0;
	}
	
	// STARTING SIMMULATED ANNEALING
	Tk=settingSimulation.To;
	bestGraph=generateInitialGraph(graphOrder,settingSimulation.random_value_x,settingSimulation.random_value_y,settingSimulation.random_value_z);
	*pbestGraph= bestGraph;
	bestGraph->setAllVertexNeighbours();
	
	if( settingSimulation.graphProperty == BETWEENNESS_CENTRALITY )
			bestGraph->brandes_betweenness_centrality(bestBC);
	else if ( settingSimulation.graphProperty == COMMUNICABILITY_BETWEENESS )
		brandes_comunicability_centrality_exp(bestGraph,bestBC);
	else
		communicability_betweenness_centrality(bestGraph,bestBC);
		
	costBest=cost(targetBC,bestBC,graphOrder);
	costOld=2.0*costBest;
	costNew=costOld;
	oldGraph=copyGraph(bestGraph);
	oldGraph->setAllVertexNeighbours();
	newGraph=copyGraph(bestGraph);
	newGraph->setAllVertexNeighbours();

	
	int okTrue=0;
	int okFalse=0;
	int notOk=0;
	do{
		/* Repeat NMAX times */
		for(N=0;(N<settingSimulation.nMax)&&(!weAreDone);N++){
			// Slightly modify oldGraph to obtain newGraph

			modifyGraph(newGraph,settingSimulation.random_value_x,settingSimulation.random_value_y,settingSimulation.random_value_z);
			// Evaluate newGraph's vertex betweenness centrality
			if( settingSimulation.graphProperty == BETWEENNESS_CENTRALITY )
				newGraph->brandes_betweenness_centrality(newBC);
			else if ( settingSimulation.graphProperty == COMMUNICABILITY_BETWEENESS )
				brandes_comunicability_centrality_exp(newGraph,newBC);
			else
				communicability_betweenness_centrality(bestGraph,newBC);
			// Update cost variables (new and old graphs)
			costOld=costNew;
			costNew=cost(targetBC,newBC,graphOrder);
			if(costNew<costBest){
				costBest=costNew;
				copyGraph(newGraph,bestGraph);
				memcpy(bestBC,newBC,graphOrder*sizeof(double));
				if(costBest<=tol){
					weAreDone=true;
					break;
				}
				okTrue++;
				lFuncTrace.trace(".");
				fprintf(logFile,".");
			} else if(exp((costBest-costNew)/Tk)>generateRandomNumber(settingSimulation.random_value_x,settingSimulation.random_value_y,settingSimulation.random_value_z)){
				// if newCost not is better than oldCost,
				// we still accept it if exp(df/T_k)<rand()
				okFalse++;
				lFuncTrace.trace("o");
				fprintf(logFile,"o");
			} else {
				//otherwise we don't accept the new graph
				copyGraph(bestGraph,newGraph);
				notOk++;
				lFuncTrace.trace("x");
				fprintf(logFile,"x");
			}
		}
		lFuncTrace.trace("\n");
		fprintf(logFile,"\n");
		lFuncTrace.trace("Tk=%2.15f\tBest Cost=%2.15f EXIT=%d Iterations=%d\n",
			   Tk,costBest,weAreDone,iterations);
		fprintf(logFile,"Tk=%2.15f\tBest Cost=%2.15f EXIT=%d Iterations=%d\n",
				Tk,costBest,weAreDone,iterations);
		// Lower temperature: T(k)=k*T(k-1)
		Tk*=k;
		// Update number of iterations
		iterations++;
	}while((Tk>=temperMin)&&(!weAreDone)&&(iterations!=settingSimulation.maxIterations));
	
	
}



int
CRebuildGraph::regenerateGraph(CSettingsSimulation &settingsSimulation,
							   double *&targetBC,
							   double *&bestBC,
							   int &graphOrder,
							   double &compareResult){
	
	
	time_t timeStart;
	time_t timeEnd;
	
	char inputFilename[STRING_LENGTH];
	char outputGraphFilename[STRING_LENGTH];
	char inputGraphFilename[STRING_LENGTH];
	char logFilename[STRING_LENGTH];
	FILE *logFile=NULL;
	
	
	// Simmulated Annealing variables
	double temperMin=TEMPER_MIN_DEFAULT;
	double Tk=TEMPER_INITIAL_DEFAULT;
	double k=K;
	
	
	double costBest=0.0;
	
	
	graph *targetGraph=NULL;
	graph *bestGraph=NULL;
	
	// Default value initialization
	timeStart=time(NULL);
	
	
	
	// Substitution of default values with argument values
	printf("Reconstruction of a graph ");
	printf("from its vertex's betweenness centrality values\n");
	printf("Use: reconstruct [input_file] [P/A/B] [To] [Tmin] [Nmax] [k]");
		
	k = settingsSimulation.k;
	temperMin = settingsSimulation.tMin;
	strcpy(inputFilename,settingsSimulation.inputFileName.c_str());
	
	targetGraph=readPythonGraphFile(inputFilename);
	
	graphOrder=0;
	graphOrder=targetGraph->getOrder();
	
	targetBC =(double*) malloc(graphOrder*sizeof(double));
	bestBC=(double*) malloc(graphOrder*sizeof(double));
	
	
	
	for(int i=0;i<graphOrder;i++){
		targetBC[i]=0.0;
		bestBC[i]=0.0;
	}
	
	targetGraph->printGraph();
	targetGraph->setAllVertexNeighbours();
	
	if( settingsSimulation.graphProperty == BETWEENNESS_CENTRALITY )
		targetGraph->brandes_betweenness_centrality(targetBC);
    else if ( settingsSimulation.graphProperty == COMMUNICABILITY_BETWEENESS )
		brandes_comunicability_centrality_exp(targetGraph,targetBC);
	else if ( settingsSimulation.graphProperty == COMMUNICABILITY_BETWEENESS_CENTRALITY )
		communicability_betweenness_centrality(targetGraph,targetBC);
	else
	{
		std::cout << " graphProperty is not set" << std::endl;
		return -1;
	}
	strcpy(inputGraphFilename,inputFilename);
    strcat(inputGraphFilename,".in");
	targetGraph->printMyGraph(inputGraphFilename);
	
	
	
	strcpy(outputGraphFilename,inputFilename);
	strcat(outputGraphFilename,".res");
	
	
	strcpy(logFilename,inputFilename);
	strcat(logFilename,".log");
	logFile=fopen(logFilename,"w");
	if(logFile==NULL){
		printf("Cannot open log file %s for writting \n",logFilename);
		exit(-1);
	}
	
	AnnealingAlgorithm( Tk, &bestGraph,graphOrder,
					   bestBC,targetBC, logFile,costBest,settingsSimulation);
	
	gsl_matrix *targetGraphGsl = gsl_matrix_alloc(targetGraph->getOrder(), targetGraph->getOrder());
	gsl_matrix *bestGraphGsl = gsl_matrix_alloc(bestGraph->getOrder(), bestGraph->getOrder());
	
	graphToGsl(targetGraph,targetGraphGsl);
	graphToGsl(bestGraph,bestGraphGsl);
	
	
	compareResult = compareMatrix(targetGraphGsl, bestGraphGsl);
	
	// Processing tasks accomplished
	// Showing results
	
	timeEnd=time(NULL);
	
	printf("RESULTS:\n");
	printf("CPU time needed: %f seconds\n",difftime(timeEnd,timeStart));
	// printf("Output file: %s\n",outputFilename);
	
	generateOutputFile(targetGraph,inputFilename,  Tk, costBest,targetBC,
					   bestBC, timeStart, timeEnd,settingsSimulation);
	
	printf("\nReconstructed graph file: %s\n",outputGraphFilename);
	bestGraph->printGraph();
	
	bestGraph->printMyGraph(outputGraphFilename);
	
    return 1;
}


#define RESULT_OK 1

graph*
CRebuildGraph::GetGraphfromFile(const char *argv[])
{
	CFuncTrace lFuncTrace(false,string("GetGraphfromFile"));
	graph *targetGraph=NULL;
	char inputFilename[STRING_LENGTH];
	
	if (argv[1]==NULL){
		lFuncTrace.trace("ERROR : argv[1] is NULL");
		return NULL;
	}
	strcpy(inputFilename,argv[1]);
	targetGraph=readPythonGraphFile(inputFilename);
	
	
	return targetGraph;
}

int CRebuildGraph::calculateBeterness(const char *argv[]){
	CFuncTrace lFuncTrace(false,"fCalculateBeterness");
	graph *targetGraph=NULL;
	int graphOrder=0;
	
	targetGraph=GetGraphfromFile(argv);
	graphOrder=targetGraph->getOrder();
	
	
	double targetBC [graphOrder];
	
	targetGraph->brandes_betweenness_centrality(targetBC);
	
	for (int i = 0; i < graphOrder; i++){
		lFuncTrace.trace("%2.10f\n",targetBC[i]);
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

gsl_matrix *
CRebuildGraph::gslCopyGraph(const gsl_matrix* target){
	
	gsl_matrix *dest=gsl_matrix_alloc(target->size1,target->size1);

	gsl_matrix_memcpy (dest, target);
	return dest;
}


int
CRebuildGraph::graphToGsl(graph * source, gsl_matrix* target){
	
	int i,j;
	gsl_matrix_set_zero(target);
	int graphOrder=source->getOrder();
	int auxNNeighbours;
	int *auxNeighbours;
	
	for(i=0;i<graphOrder;i++){
		auxNNeighbours=source->vertexArray[i]->getNeighbours(&auxNeighbours);
		//		printf("Vertex %3d(%d neighbours):",i,auxNNeighbours);
		for(j=0;j<auxNNeighbours;j++){
			//			printf(" %d",auxNeighbours[j]);
			gsl_matrix_set (target, i, auxNeighbours[j], 1);
		}
		free(auxNeighbours);
	}
	return RESULT_OK;
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

int CRebuildGraph::gslVectorToArray(gsl_vector* gslVector, double* arrayDoubles)
{

	for (size_t i = 0; i < gslVector->size; i++) {
		arrayDoubles[i]=  gsl_vector_get(gslVector, i);
	
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


gsl_vector *
CRebuildGraph::calculateExp(const gsl_vector_complex *eval){
	CFuncTrace lFuncTrace(false,"calculateExp");
	int order = (int)eval->size;
	
	lFuncTrace.trace("Ordre for Expo %d",order);
	
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
	
	graph *targetGraph=NULL;
	targetGraph=GetGraphfromFile(argv);
	int graphOrder=targetGraph->getOrder();
	lFuncTrace.trace("Graph Order %d",graphOrder);
	
	// Get Numpy Matrix // Matriu d'adjacencia
	gsl_matrix *A1=gsl_matrix_alloc(graphOrder,graphOrder);
	
	targetGraph->printGraph();
	
	graphToGsl(targetGraph,A1);
	
	lFuncTrace.trace("Printing Home made Matrix\n");
	printGslMatrix(A1);
	lFuncTrace.trace("Printing with gsl function\n");
	gsl_matrix_fprintf(stdout, A1, "%g");
	
	gsl_vector_complex *eval = calculateEgeinval(A1);
	
	/*gsl_vector *evalexp = */ calculateExp(eval);
	
	return RESULT_OK;
}

void
CRebuildGraph::	communicability_betweenness_centrality(graph *targetGraph,double *myCExp)
{
	CFuncTrace lFuncTrace(false,"communicability_betweenness_centrality");
	/* Step 1
	 nodelist = G.nodes() # ordering of nodes in matrix
	 n = len(nodelist)
	 A = nx.to_numpy_matrix(G,nodelist)
	 # convert to 0-1 matrix
	 A[A!=0.0] = 1
	 */
	int graphOrder=targetGraph->getOrder();
	
	
	gsl_vector * matrixFinalResult = gsl_vector_alloc(graphOrder);
	
	
	// Get Numpy Matrix // Matriu d'adjacencia
	gsl_matrix *A1=gsl_matrix_alloc(graphOrder,graphOrder);
	
	//	targetGraph->printGraph();
	
	graphToGsl(targetGraph,A1);
		lFuncTrace.trace("\nPrinting Home made Matrix\n");
//		printGslMatrix(A1," %g");
	 
	 /* Step 2
	 expA = scipy.linalg.expm(A)
	 mapping = dict(zip(nodelist,range(n)))
	 sc = {}
	 */
	gsl_matrix *A1expm=gsl_matrix_alloc(graphOrder,graphOrder);
	
	gsl_linalg_exponential_ss(A1, A1expm, .01);
	//	lFuncTrace.trace("Printing ExpmMatrix");
//		printGslMatrix(A1expm);

	for ( int iteratorNode = 0; iteratorNode < graphOrder; iteratorNode++){
		gsl_matrix *copyA1 = gslCopyGraph(A1);
		gslDeleteNodeConnections(copyA1,iteratorNode);
//		printGslMatrix(copyA1);
		/*
		 B = (expA - scipy.linalg.expm(A)) / expA
		*/
		gsl_matrix *copyA1expm=gsl_matrix_alloc(graphOrder,graphOrder);
		gsl_linalg_exponential_ss(copyA1, copyA1expm, .01);
		
		gsl_matrix *copyexpmAForSubstract  = gslCopyGraph(A1expm);
		
		gsl_matrix_sub(copyexpmAForSubstract,copyA1expm);
		gsl_matrix_div_elements (copyexpmAForSubstract, A1expm);
		lFuncTrace.trace("Printing expA- scip\n");
//		printGslMatrix(copyexpmAForSubstract);
		gslDeleteNodeConnections(copyexpmAForSubstract,iteratorNode);
		
		gsl_matrix *copyB = gslCopyGraph(copyexpmAForSubstract);
		
		for ( int col =0; col < graphOrder; col++){
			for ( int row = 0; row< graphOrder ; row ++){
				if ( row != col)
					gsl_matrix_set(copyB,row,col,0);
			}
		}
		gsl_matrix_sub(copyexpmAForSubstract,copyB);
//		printGslMatrix(copyexpmAForSubstract);
		double sum = 0;
		for ( int col =0; col < graphOrder; col++){
			for ( int row = 0; row< graphOrder ; row ++){
				
					sum +=gsl_matrix_get(copyexpmAForSubstract,row,col);
			}
		}
		lFuncTrace.trace("Suma %f\n",sum);
		gsl_vector_set(matrixFinalResult,iteratorNode,sum);

	}
	gslVectorToArray(matrixFinalResult,myCExp);
}


gsl_vector *
CRebuildGraph::getDiagonalFromGslMatrix(const gsl_matrix * gslMatrix){
	
	int nMatrixOrder = (int) gslMatrix->size1;
	gsl_vector * gslvDiagonal = gsl_vector_alloc(nMatrixOrder);
	
	for (int i=0; i < nMatrixOrder;i++){
		gsl_vector_set(gslvDiagonal,i,gsl_matrix_get(gslMatrix,i,i));
	}
	
	return gslvDiagonal;
}


void
CRebuildGraph::brandes_comunicability_centrality_exp(graph *targetGraph,double *myCExp){
//	CFuncTrace lFuncTrace("CRebuildGraph::brandes_comunicability_centrality_exp");
	
	int graphOrder=targetGraph->getOrder();
	// Get Numpy Matrix // Matriu d'adjacencia
	gsl_matrix *A1=gsl_matrix_alloc(graphOrder,graphOrder);
	
//	targetGraph->printGraph();
	
	graphToGsl(targetGraph,A1);
//	lFuncTrace.trace("\nPrinting Home made Matrix\n");
//	printGslMatrix(A1," %g");
	gsl_matrix *A1expm=gsl_matrix_alloc(graphOrder,graphOrder);
	
	gsl_linalg_exponential_ss(A1, A1expm, .01);
//	lFuncTrace.trace("Printing ExpmMatrix");
//	printGslMatrix(A1expm);
	
	gsl_vector * gslvDiagonal = getDiagonalFromGslMatrix(A1expm);
	
//	lFuncTrace.trace("Printing Diagonal From ExpmMatrix");
//	printGslVector(gslvDiagonal);
	
	gslVectorToArray(gslvDiagonal,myCExp);
}

int
CRebuildGraph::calculateCommunicability_cent_exp(const char *argv[]){
	CFuncTrace lFuncTrace(false,"fCalculateCommunicability_cent_exp");
	
	graph *targetGraph=NULL;
	targetGraph=GetGraphfromFile(argv);
	int graphOrder=targetGraph->getOrder();
	lFuncTrace.trace("Graph Order %d",graphOrder);
	double * bestCommCentExp = (double*)malloc(graphOrder * sizeof(double));
	
	brandes_comunicability_centrality_exp(targetGraph,bestCommCentExp);
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
				gsl_matrix **matrixA,
				gsl_matrix **F )
{
	gsl_vector_free(*s);
	gsl_vector_free(*work);
	gsl_matrix_free(*U1);
	gsl_matrix_free(*U2);
	gsl_matrix_free(*V1);
	gsl_matrix_free(*V2);
	gsl_matrix_free(*matrixA);
	gsl_matrix_free(*F);

	
}

void CRebuildGraph::printingCompareMatrixResults(float delta,
												 gsl_matrix *F,
												 gsl_matrix* matrixA
												 ){
	CFuncTrace lFuncTrace(false,"CRebuildGraph::printingCompareMatrixResults");
	lFuncTrace.trace("\nDIFERENCIA (delta) -> %f",delta);
	
	//Per presentar-la, definim positiva i normalitzem la matriu F
	if(gsl_matrix_min(F)<0)
		gsl_matrix_add_constant (F, -gsl_matrix_min(F));
	if(gsl_matrix_max(F)>0)
		gsl_matrix_scale (F, 1/gsl_matrix_max(F));
	
	FILE *out;
	out=fopen("sortida.txt","w");
	lFuncTrace.trace("\nResultats en sortida.txt");
	fprintf(out, "DIFERENCIA (delta) -> %f\n\n",delta);
	for(int i=0; i<matrixA->size1; i++){
		for(int j=0; j<matrixA->size1; j++){
			if(gsl_matrix_get(matrixA,i,j)==0){
				fprintf(out," ");
			}else if(gsl_matrix_get(matrixA,i,j)==1){
				fprintf(out,"#");
			}else{
				printf("\nERROR-Matriu no valida");
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
	
	gsl_matrix *U1, *U2,*V1,*V2;
	
	InitMatrix(matrixA->size1,&work,&s,&U1,&U2,&V1,&V2);

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
			   &matrixA,
			   &F );

	return delta;
}


