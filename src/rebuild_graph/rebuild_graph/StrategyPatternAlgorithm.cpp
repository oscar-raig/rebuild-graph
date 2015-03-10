	//
//  SrategyPatternAlgorithm.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 27/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include "FactoryGraphIndicator.h"
#include "StrategyPatternAlgorithm.h"
#include "graphIndicatorBetweennessCentrality.h"
#include "graphIndicatorCommunicabilityCentralityUsingMatrixExponential.h"
#include "graphIndicatorCommunicabilityBetweennessCentrality.h"






//-----------------------------------------------------------------------------
// GLOBAL operation cost(double *tarjet,double *current,int count)
// RETURNS the cost
//-----------------------------------------------------------------------------
double StrategyPatternAlgorithm::cost(double *tarjet,double *current,int count){
	int i;
	double result=0.0;
	
	for(i=0; i<count; i++) {
		result+=pow((tarjet[i]-current[i]),2);
	}
	return pow(result,0.5);
}


//-----------------------------------------------------------------------------
// GLOBAL operation modifyGraph(sourceGraph)
// MODIFIES a randon sourceGraph vertex's connections
//-----------------------------------------------------------------------------
void StrategyPatternAlgorithm::modifyGraph(gslGraph *sourceGraph, int LevelGraph){
	int i,j;
	int vertex2change;
	int myOrder=sourceGraph->getOrder();
	int myNewNeighbour;
	int myNewNumberOfNeighbours;
	int newNeighbours[myOrder];
	int found;
	
//	CFuncTrace lFuncTrace(false,"StrategyPatternAlgorithm::modifyGraph");
	
	// Select vertex to change

	vertex2change=(int)(generateRandomNumber()*(myOrder));
	sourceGraph->removeVertexNeighbours(vertex2change);
//	sourceGraph->printGraph(LevelGraph);
//	lFuncTrace.trace(LevelGraph,"modifyGraph vertex removed %d\n",vertex2change);
	do{
		//Choose new vertex degree
		myNewNumberOfNeighbours=1+(int)(generateRandomNumber()*(myOrder-1));
//		lFuncTrace.trace(LevelGraph,"New Degree %d for vertex   %d\n",myNewNumberOfNeighbours,vertex2change);
		// myNewNumberOfNeighbours is in [1,n-1]
		// Connect new neighbours
		for(i=0; i<myNewNumberOfNeighbours; i++){
			do{
				found=false;
				myNewNeighbour=(int)(generateRandomNumber()*(myOrder));
				// myNewNeighbour is in [0,n-1]
				for(j=0;j<i;j++){
					if((myNewNeighbour==newNeighbours[j])
					   ||(myNewNeighbour==vertex2change)){
						found=true;
					}
				}
			} while(found||
					(sourceGraph->vertexAreNeighbours(vertex2change,myNewNeighbour)));
			newNeighbours[i]=myNewNeighbour;
			sourceGraph->addNewVertexNeighbour(vertex2change,myNewNeighbour);
		}
	} while (sourceGraph->graphNotConnected(&vertex2change));
}



//-----------------------------------------------------------------------------
// GLOBAL operation generateRandomNumber()
// Wichmann-Hill method to generate random numbers (needs 3 seed numbers)
// RETURNS a random double [0,1(
//-----------------------------------------------------------------------------
double
StrategyPatternAlgorithm::generateRandomNumber(){
	
	double temp;
	settingsSimulation->random_value_x=171*(settingsSimulation->random_value_x%177)-2*(settingsSimulation->random_value_x/177);
	if(settingsSimulation->random_value_x<0)
		settingsSimulation->random_value_x+=30269;
	settingsSimulation->random_value_y=172*(settingsSimulation->random_value_y%176)-35*(settingsSimulation->random_value_y/176);
	if(settingsSimulation->random_value_y<0)
		settingsSimulation->random_value_y+=30307;
	settingsSimulation->random_value_z=170*(settingsSimulation->random_value_z%178)-63*(settingsSimulation->random_value_z/178);
	if(settingsSimulation->random_value_z<0)
		settingsSimulation->random_value_z+=30323;
	temp=settingsSimulation->random_value_x/30269.0+settingsSimulation->random_value_y/30307.0+settingsSimulation->random_value_z/30323.0;
	return(temp-(int)temp);
}

//-----------------------------------------------------------------------------
// GLOBAL operation generateInitialGraph(int sourceGraphOrder)
// RETURNS a random graph with sourceGraphOrder vertex
//-----------------------------------------------------------------------------
void StrategyPatternAlgorithm::generateInitialGraph(int sourceGraphOrder){
	int i,j;
	int newNeighbour;
	int newDegree;
	CFuncTrace trace (false,"StrategyPatternAlgorithm::generateInitialGraph");
	
	this->sourceGraph=new gslGraph(sourceGraphOrder);
	
	for(i=0; i<sourceGraphOrder; i++){
		trace.trace(STP_DEBUG,"Iteration %d",i);
		trace.trace(STP_DEBUG,"Iteration %d sourceGraphOrder %d Degree %d",
						i,sourceGraphOrder,this->sourceGraph->getDegree(i));
		// The new vertex degree is to be
		//   at least 1 (the graphs needs to be connected)
		//   at most n-1 (the vertex is connected to every other vertex
		if(this->sourceGraph->getDegree(i)==0){
			// vertex i has no neighbours yet
//			settingsSimulation->random_value_z = settingsSimulation->random_value_z->
			newDegree=1+(int)(generateRandomNumber()*(sourceGraphOrder-1));
			// newDegre is in [1,n-1]
		} else if(this->sourceGraph->getDegree(i)<(sourceGraphOrder-1)){
			// vertex i is connected to some other vertex
			newDegree=(int)(generateRandomNumber()*
							(sourceGraphOrder-this->sourceGraph->getDegree(i)));
			// newDegree is in [0,n-1-degree]
		} else {
			// vertex i is already connected to all possible neighbours
			newDegree=0;
		}
		trace.trace(STP_DEBUG,"Iteration %d NewDegree for node %i is %d",
					i,i,newDegree);
		int vertexAreNighbours = 0;
		for(j=0;j<newDegree;j++){
			do{
				newNeighbour=(int)(generateRandomNumber()*(sourceGraphOrder));
				// newNeighbour is in [0,n-1]
				trace.trace(STP_DEBUG,"Is Neighbour %d to %d",
							newNeighbour,i);
				vertexAreNighbours = this->sourceGraph->vertexAreNeighbours(i,newNeighbour);
				if (vertexAreNighbours)
					trace.trace(STP_DEBUG,"ATT : VERTEX Are Nighbours");
				// Added linia 2014-11-27
				//vertexAreNighbours = ( i == newNeighbour);
			} while(vertexAreNighbours);
			trace.trace(STP_DEBUG,"Adding newNeighbour %d to %d", i,
						newNeighbour);
			this->sourceGraph->addVertexNeighbour(i,newNeighbour);
		}
	}
	
}

void StrategyPatternAlgorithm::Loop(double &costNew,double &costBest,
									gslGraph ** newGraph,double tol,double *newBC, double *bestBC,
									int graphOrder,int &weAreDone, double Tk,int N){
	CFuncTrace lFuncTrace(false,"StrategyPatternAlgorithm::Loop");
	
	if(costNew<costBest){
		costBest=costNew;
		this->setGraph( (*newGraph)->copyGraph() );
		
		memcpy(bestBC,newBC,graphOrder*sizeof(double));
		if(costBest<=tol){
			lFuncTrace.trace(STP_INFO,"We are Done costBest < tol %d",N);
			weAreDone=true;
			return;
		}
		lFuncTrace.trace(CTrace::TRACE_DEBUG,".");
		fprintf(logFile,".");
	} else if(exp((costBest-costNew)/Tk)>generateRandomNumber()){
		// if newCost not is better than oldCost,
		// we still accept it if exp(df/T_k)<rand()
		lFuncTrace.trace(CTrace::TRACE_DEBUG,"o");
		fprintf(logFile,"o");
	} else {
		//otherwise we don't accept the new graph
		if (*newGraph)
			delete *newGraph;
		//newGraph = sourceGraph->copyGraph();
		*newGraph = this->getGraph()->copyGraph();
		lFuncTrace.trace(CTrace::TRACE_DEBUG,"Al loro que hi havia aqui un erro, copiant to newgraph sourcegrAPH");
		lFuncTrace.trace(CTrace::TRACE_DEBUG,"x");
		fprintf(logFile,"x");
	}
	
}


void StrategyPatternAlgorithm::AnnealingAlgorithm(int graphOrder,
									   double *&bestBC,double * targetBC,
									   double &costBest){
	
	CFuncTrace lFuncTrace(true,"StrategyPatternAlgorithm::AnnealingAlgorithm");
//	fprintf(logFile,"CRebuildGraph::AnnealingAlgorithm");
	double temperMin=this->settingsSimulation->tMin;
	int iterations=0;
	double tol=TOL;
	int weAreDone=0;
	costBest=0.0;
	double costOld=0.0;
	double costNew=0.0;
	long int N=0;
	double * newBC = new double [graphOrder];
	double Tk;
	gslGraph *newGraph=NULL;
	
	for(int i=0;i<graphOrder;i++){
		bestBC[i]=0.0;
		newBC[i]=0.0;
	}
	
	// STARTING SIMMULATED ANNEALING
	Tk=settingsSimulation->To;
	generateInitialGraph(graphOrder);

	{
		graphIndicator * graphIndicator = FactoryGraphIndicator::CreategraphIndicator(settingsSimulation->graphProperty,sourceGraph);
		bestBC = graphIndicator->calculateIndicator();
		delete  graphIndicator;
	}
	costBest=cost(targetBC,bestBC,graphOrder);
	costOld=2.0*costBest;
	costNew=costOld;
	newGraph=sourceGraph->copyGraph();
	newGraph->printGraph();
	lFuncTrace.trace(STP_INFO,"Cost Best=%2.15f Cost New %2.15f Cost Old %2.15f\n",
					 costBest,costNew,costOld);
	do{
		/* Repeat NMAX times */
		for(N=0;(N<settingsSimulation->nMax)&&(!weAreDone);N++){
			lFuncTrace.trace(STP_DEBUG,"Iteration N %d",N);
			
			modifyGraph(newGraph);

			{
				graphIndicator * graphIndicator = FactoryGraphIndicator::CreategraphIndicator(settingsSimulation->graphProperty,newGraph);
				if ( newBC)
					delete newBC;
				newBC = graphIndicator->calculateIndicator();
				delete  graphIndicator;
			}
			// Update cost variables (new and old graphs)
			costOld=costNew;
			costNew=cost(targetBC,newBC,graphOrder);
			lFuncTrace.trace(STP_DEBUG,"N %d Cost New %f Best Cost  %f",N,costNew,costBest);
			Loop(costNew,costBest,&newGraph,tol,newBC,bestBC,graphOrder,weAreDone,Tk,N);
		}
		fprintf(logFile,"\n");
		lFuncTrace.trace(STP_INFO,"Tk=%2.15f\tBest Cost=%2.15f EXIT=%d Iterations=%d\n",
						 Tk,costBest,weAreDone,iterations);
		fprintf(logFile,"Tk=%2.15f\tBest Cost=%2.15f EXIT=%d Iterations=%d\n",
				Tk,costBest,weAreDone,iterations);
		// Lower temperature: T(k)=k*T(k-1)
		Tk*=this->settingsSimulation->k;
		// Update number of iterations
		iterations++;
	}while((Tk>=temperMin)&&(!weAreDone)&&(iterations!= settingsSimulation->maxIterations));
	
	lFuncTrace.trace(STP_DEBUG,"Tk=%2.15f\tBest Cost=%2.15f EXIT=%d Iterations=%d\n",
					 Tk,costBest,weAreDone,iterations);
	if (newGraph)
    		delete newGraph;
	if ( newBC)
                delete newBC;
}

int
StrategyPatternAlgorithm::regenerateGraph(gslGraph *targetGraph,
							   double *&bestBC,
							   int &graphOrder,
							   double &compareResult,
							  double *Tk,
								double *costBest){
	CFuncTrace trace (true,"StrategyPatternAlgorithm::regenerateGraph");
	
	try {
		char inputGraphFilename[STRING_LENGTH];
		char logFilename[STRING_LENGTH];
		
		
		// Simmulated Annealing variables
		
		if ( settingsSimulation == NULL)
			throw std::runtime_error("settingsSimulation is NULL");
		
		graphOrder=0;
		graphOrder=targetGraph->getOrder();
		double * targetBC = NULL;
		targetBC =(double*) malloc(graphOrder*sizeof(double));
		bestBC=(double*) malloc(graphOrder*sizeof(double));
		
		for(int i=0;i<graphOrder;i++){
			bestBC[i]=0.0;
			targetBC[i]=0.0;
		}
		
		{
			graphIndicator * graphIndicator = FactoryGraphIndicator::CreategraphIndicator(settingsSimulation->graphProperty,targetGraph);
			if ( targetBC)
			     delete targetBC;
			targetBC = graphIndicator->calculateIndicator();
			delete  graphIndicator;
		}

		
		strcpy(inputGraphFilename,settingsSimulation->inputFileName.c_str());
		strcat(inputGraphFilename,".in");
		targetGraph->printMyGraph(inputGraphFilename,
								  settingsSimulation->outputFormatGraphResultAdjList);
		strcpy(logFilename,settingsSimulation->inputFileName.c_str());
		strcat(logFilename,".log");
		logFile=fopen(logFilename,"w");
		if(logFile==NULL){
			printf("Cannot open log file %s for writting \n",logFilename);
			exit(-1);
		}
		
		AnnealingAlgorithm( graphOrder,
						   bestBC,targetBC, *costBest);
		

	}
	catch ( exception& e)
	{
		std::cout << "ERROR :" << e.what()<< std::endl;
		throw;
		return -1;
	}
    return 1;
}


