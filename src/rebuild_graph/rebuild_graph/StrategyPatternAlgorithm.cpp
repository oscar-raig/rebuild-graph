	//
//  SrategyPatternAlgorithm.cpp
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 27/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include "StrategyPatternAlgorithm.h"
#include "GraphFactory.h"

#define STP_DEBUG CTrace::level::TRACE_DEBUG
#define STP_INFO CTrace::level::TRACE_INFO



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
void StrategyPatternAlgorithm::modifyGraph(GeneralGraph *sourceGraph,int &random_value_x,int &random_value_y,int &random_value_z){
	int i,j;
	int vertex2change;
	int myOrder=sourceGraph->getOrder();
	int myNewNeighbour;
	int myNewNumberOfNeighbours;
	int newNeighbours[myOrder];
	int found;
	
	CFuncTrace lFuncTrace(false,"StrategyPatternAlgorithm::modifyGraph");
	
	// Select vertex to change
	vertex2change=(int)(generateRandomNumber(random_value_x,random_value_y,random_value_z)*(myOrder));
	// vertex2change is in [0,n-1]
	// Disconnect vertex2change
	//printf("modifyGraph\n");
	//sourceGraph->printGraph();
	//printf("modifyGraph remove vertex %d\n",vertex2change);
	sourceGraph->removeVertexNeighbours(vertex2change);
	sourceGraph->printGraph();
	lFuncTrace.trace(CTrace::level::TRACE_DEBUG,"modifyGraph vertex removed\n");
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
			 sourceGraph->printGraph();
		}
		//    sourceGraph->printGraph();
	} while (sourceGraph->graphNotConnected(&vertex2change));
	//sourceGraph->printGraph();
	//printf("modifyGraph vertex reconnected\n");
}



//-----------------------------------------------------------------------------
// GLOBAL operation generateRandomNumber()
// Wichmann-Hill method to generate random numbers (needs 3 seed numbers)
// RETURNS a random double [0,1(
//-----------------------------------------------------------------------------
double
StrategyPatternAlgorithm::generateRandomNumber(int &random_value_x,int &random_value_y, int &random_value_z){
	
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
GeneralGraph *StrategyPatternAlgorithm::generateInitialGraph(int sourceGraphOrder,int &random_value_x,int &random_value_y,int &random_value_z){
	int i,j;
	int newNeighbour;
	int newDegree;
	CFuncTrace trace (false,"StrategyPatternAlgorithm::generateInitialGraph");
	GeneralGraph *result=GraphFactory::createGraph(USED_GRAPH,sourceGraphOrder);
	
	for(i=0; i<sourceGraphOrder; i++){
		trace.trace(STP_DEBUG,"Iteration %d",i);
		trace.trace(STP_DEBUG,"Iteration %d sourceGraphOrder %d Degree %d",
						i,sourceGraphOrder,result->getDegree(i));
		// The new vertex degree is to be
		//   at least 1 (the graphs needs to be connected)
		//   at most n-1 (the vertex is connected to every other vertex
		if(result->getDegree(i)==0){
			// vertex i has no neighbours yet
			newDegree=1+(int)(generateRandomNumber(random_value_x,random_value_y,random_value_z)*(sourceGraphOrder-1));
			// newDegre is in [1,n-1]
		} else if(result->getDegree(i)<(sourceGraphOrder-1)){
			// vertex i is connected to some other vertex
			newDegree=(int)(generateRandomNumber(random_value_x,random_value_y,random_value_z)*
							(sourceGraphOrder-result->getDegree(i)));
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
				newNeighbour=(int)(generateRandomNumber(random_value_x,random_value_y,random_value_z)*(sourceGraphOrder));
				// newNeighbour is in [0,n-1]
				trace.trace(STP_DEBUG,"Is Neighbour %d to %d",
							newNeighbour,i);
				vertexAreNighbours = result->vertexAreNeighbours(i,newNeighbour);
				if (vertexAreNighbours)
					trace.trace(STP_DEBUG,"ATT : VERTEX Are Nighbours");
				// Added linia 2014-11-27
				vertexAreNighbours = ( i == newNeighbour);
			} while(vertexAreNighbours);
			trace.trace(STP_DEBUG,"Adding newNeighbour %d to %d", i,
						newNeighbour);
			result->addVertexNeighbour(i,newNeighbour);
		}
	}
	return result;
}



void StrategyPatternAlgorithm::AnnealingAlgorithm(double &Tk, GeneralGraph **pbestGraph,int graphOrder,
									   double *bestBC,double *targetBC,
									   FILE *logFile,double &costBest,
									   CSettingsSimulation settingSimulation){
	
	CFuncTrace lFuncTrace(false,"StrategyPatternAlgorithm::AnnealingAlgorithm");
//	fprintf(logFile,"CRebuildGraph::AnnealingAlgorithm");
	double temperMin=TEMPER_MIN_DEFAULT;
	double k=K;
	int iterations=0;
	double tol=TOL;
	int weAreDone=0;
	costBest=0.0;
	double costOld=0.0;
	double costNew=0.0;
	long int N=0;
	GeneralGraph * bestGraph= NULL;
	double newBC [graphOrder];
	GeneralGraph *newGraph=NULL;
	
	for(int i=0;i<graphOrder;i++){
		bestBC[i]=0.0;
		newBC[i]=0.0;
	}
	
	// STARTING SIMMULATED ANNEALING
	Tk=settingSimulation.To;
	bestGraph=generateInitialGraph(graphOrder,settingSimulation.random_value_x,settingSimulation.random_value_y,settingSimulation.random_value_z);
	bestGraph->printGraph();
	*pbestGraph= bestGraph;
	bestGraph->setAllVertexNeighbours();
	
	if( settingSimulation.graphProperty == BETWEENNESS_CENTRALITY )
		bestGraph->brandes_betweenness_centrality(bestBC);
	else if ( settingSimulation.graphProperty == COMMUNICABILITY_BETWEENESS )
		bestGraph->brandes_comunicability_centrality_exp(bestBC);
	else
		bestGraph->communicability_betweenness_centrality(bestBC);
	
	costBest=cost(targetBC,bestBC,graphOrder);
	costOld=2.0*costBest;
	costNew=costOld;
	newGraph=bestGraph->copyGraph();
	newGraph->setAllVertexNeighbours();
	newGraph->printGraph();
	int okTrue=0;
	int okFalse=0;
	int notOk=0;
	lFuncTrace.trace(STP_INFO,"Cost Best=%2.15f Cost New %2.15f Cost Old %2.15f\n",
					 costBest,costNew,costOld);
	do{
		/* Repeat NMAX times */
		for(N=0;(N<settingSimulation.nMax)&&(!weAreDone);N++){
			lFuncTrace.trace(STP_DEBUG,"Iteration N %d",N);
			
			modifyGraph(newGraph,settingSimulation.random_value_x,settingSimulation.random_value_y,settingSimulation.random_value_z);
			// Evaluate newGraph's vertex betweenness centrality
			if( settingSimulation.graphProperty == BETWEENNESS_CENTRALITY )
				newGraph->brandes_betweenness_centrality(newBC);
			else if ( settingSimulation.graphProperty == COMMUNICABILITY_BETWEENESS )
				newGraph->brandes_comunicability_centrality_exp(newBC);
			else
				newGraph->communicability_betweenness_centrality(newBC);
			// Update cost variables (new and old graphs)
			costOld=costNew;
			costNew=cost(targetBC,newBC,graphOrder);
			lFuncTrace.trace(STP_DEBUG,"Cost New %f Best Cost  %f",costNew,costBest);
			if(costNew<costBest){
				costBest=costNew;
			
				if (newGraph->GetType() == GRAPH )
					((graph *)newGraph)->copyGraph((graph*)bestGraph);
				else{
					((gslGraph *)newGraph)->copyGraph((gslGraph*)bestGraph);
					//bestGraph = newGraph->copyGraph();
									//	bestGraph->printGraph();
					int res = gslGraph::compare((gslGraph*)bestGraph, (gslGraph*)newGraph);
					if (!res )
						res++;
				}
				
			 	//newGraph->copyGraph(bestGraph);
				//bestGraph = newGraph->copyGraph();
				int res = gslGraph::compare((gslGraph*)bestGraph, (gslGraph*)newGraph);
				if (!res )
					res++;

				
				memcpy(bestBC,newBC,graphOrder*sizeof(double));
				if(costBest<=tol){
					lFuncTrace.trace(STP_INFO,"We are Done costBest < tol %d",N);
					weAreDone=true;
					break;
				}
				okTrue++;
				lFuncTrace.trace(CTrace::level::TRACE_DEBUG,".");
				fprintf(logFile,".");
			} else if(exp((costBest-costNew)/Tk)>generateRandomNumber(settingSimulation.random_value_x,settingSimulation.random_value_y,settingSimulation.random_value_z)){
				// if newCost not is better than oldCost,
				// we still accept it if exp(df/T_k)<rand()
				okFalse++;
				lFuncTrace.trace(CTrace::level::TRACE_DEBUG,"o");
				fprintf(logFile,"o");
			} else {
				//otherwise we don't accept the new graph
				if (newGraph->GetType() == GRAPH ){
					 ((graph*)bestGraph)->copyGraph((graph*)newGraph);
					//newGraph = ((graph*)bestGraph)->copyGraph();
				}else{
					
					 ((gslGraph *)bestGraph)->copyGraph((gslGraph*)newGraph);
				//	newGraph= ((gslGraph *)bestGraph)->copyGraph();
				}
			//	newGraph = bestGraph->copyGraph();
				notOk++;
				lFuncTrace.trace(CTrace::level::TRACE_DEBUG,"x");
				fprintf(logFile,"x");
			}
		}
		fprintf(logFile,"\n");
		lFuncTrace.trace(STP_INFO,"Tk=%2.15f\tBest Cost=%2.15f EXIT=%d Iterations=%d\n",
						 Tk,costBest,weAreDone,iterations);
		fprintf(logFile,"Tk=%2.15f\tBest Cost=%2.15f EXIT=%d Iterations=%d\n",
				Tk,costBest,weAreDone,iterations);
		// Lower temperature: T(k)=k*T(k-1)
		Tk*=k;
		// Update number of iterations
		iterations++;
	}while((Tk>=temperMin)&&(!weAreDone)&&(iterations!= 1 /*settingSimulation.maxIterations*/));
	
	lFuncTrace.trace(STP_DEBUG,"Tk=%2.15f\tBest Cost=%2.15f EXIT=%d Iterations=%d\n",
					 Tk,costBest,weAreDone,iterations);

}

int
StrategyPatternAlgorithm::regenerateGraph(CSettingsSimulation *settingsSimulation,
							   GeneralGraph *targetGraph,
							   char *inputFilename,
							   double *&targetBC,
							   double *&bestBC,
							   int &graphOrder,
							   double &compareResult,
							  double *Tk,
								double *costBest,GeneralGraph **bestGraph){
	CFuncTrace trace (true,"StrategyPatternAlgorithm::regenerateGraph");
	
	try {
		char inputGraphFilename[STRING_LENGTH];
		char logFilename[STRING_LENGTH];
		FILE *logFile=NULL;
		
		// Simmulated Annealing variables
		double temperMin=TEMPER_MIN_DEFAULT;
				double k=K;
		
		if ( settingsSimulation == NULL)
			throw std::runtime_error("settingsSimulation is NULL");
		
		k = settingsSimulation->k;
		temperMin = settingsSimulation->tMin;

		graphOrder=0;
		graphOrder=targetGraph->getOrder();
		
		targetBC =(double*) malloc(graphOrder*sizeof(double));
		bestBC=(double*) malloc(graphOrder*sizeof(double));
		
		for(int i=0;i<graphOrder;i++){
			targetBC[i]=0.0;
			bestBC[i]=0.0;
		}
		
//		targetGraph->printGraph();
		targetGraph->setAllVertexNeighbours();
		
		if( settingsSimulation->graphProperty == BETWEENNESS_CENTRALITY )
			targetGraph->brandes_betweenness_centrality(targetBC);
		else if ( settingsSimulation->graphProperty == COMMUNICABILITY_BETWEENESS )
			targetGraph->brandes_comunicability_centrality_exp(targetBC);
		else if ( settingsSimulation->graphProperty == COMMUNICABILITY_BETWEENESS_CENTRALITY )
			targetGraph->communicability_betweenness_centrality(targetBC);
		else
		{
			std::cout << " graphProperty is not set" << std::endl;
			return -1;
		}
		strcpy(inputGraphFilename,inputFilename);
		strcat(inputGraphFilename,".in");
		targetGraph->printMyGraph(inputGraphFilename);
		strcpy(logFilename,inputFilename);
		strcat(logFilename,".log");
		logFile=fopen(logFilename,"w");
		if(logFile==NULL){
			printf("Cannot open log file %s for writting \n",logFilename);
			exit(-1);
		}
		
		AnnealingAlgorithm( *Tk, bestGraph,graphOrder,
						   bestBC,targetBC, logFile,*costBest,*settingsSimulation);
		

	}
	catch ( exception& e)
	{
		std::cout << "ERROR :" << e.what()<< std::endl;
		throw;
		return -1;
	}
    return 1;
}

