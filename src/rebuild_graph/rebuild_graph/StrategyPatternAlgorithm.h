//
//  SrategyPatternAlgorithm.h
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 27/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#ifndef __rebuild_graph__SrategyPatternAlgorithm__
#define __rebuild_graph__SrategyPatternAlgorithm__

#include <iostream>
#include "CSettingsSumulation.h"
#include "CTrace.hpp"
#include "gslGraph.h"
#include "FactoryGraphIndicator.h"
#include "graphIndicatorBetweennessCentrality.h"
#include "graphIndicatorCommunicabilityCentralityUsingMatrixExponential.h"
#include "graphIndicatorCommunicabilityBetweennessCentrality.h"

#define STP_DEBUG CTrace::TRACE_DEBUG
#define STP_INFO CTrace::TRACE_INFO

#define TEMPER_INITIAL_DEFAULT 10
#define TEMPER_MIN_DEFAULT 0.0001
#define NUMBER_MAX_COMBINATIONS_DEFAULT 1000
#define K 0.9
#define TOL 0.0001
#define MAX_ITERATIONS 1000

#ifndef STRING_LENGTH
#define STRING_LENGTH 256
#endif

class StrategyPatternAlgorithm {
	
protected:
	CSettingsSimulation *settingsSimulation;
	bool settingsSimulationAllocated;
	gslGraph *sourceGraph;
	FILE *logFile;
	
public:
	StrategyPatternAlgorithm(CSettingsSimulation *argummentSettingsSimulation)
	{
		CFuncTrace trace(false,"StrategyPatternAlgorithm");
		
		if ( argummentSettingsSimulation){
			settingsSimulationAllocated= false;
			settingsSimulation = argummentSettingsSimulation;
		}
		else{
			settingsSimulationAllocated = true;
			settingsSimulation = new CSettingsSimulation();
		}
	};

	~StrategyPatternAlgorithm(){
		if (sourceGraph)
			delete sourceGraph;
		if (settingsSimulation && settingsSimulationAllocated )
			delete settingsSimulation;
	}
	
	virtual  void Loop(double &costNew,double &costBest,
			gslGraph ** newGraph,double tol,double *newBC, double *bestBC,
			int graphOrder,int &weAreDone, double Tk,int N);
	
	gslGraph * getGraph(){
		return sourceGraph;
	}
	
	void setGraph(gslGraph * graph){
		if ( sourceGraph ){
			delete sourceGraph;
		}
		sourceGraph = graph;
	}
	
	
	static double cost(double *tarjet,double *current,int count);

	void modifyGraph(gslGraph *sourceGraph, int LevelGraph = CTrace::TRACE_DEBUG);
	double generateRandomNumber();
	void generateInitialGraph(int sourceGraphOrder);
	
	int
	regenerateGraph(gslGraph *targetGraph,
					double *&bestBC,
					int &graphOrder,
					double &compareResult,
					double *Tk,
					double *costBest);
	// Rebuilding Graph Main Functions

	virtual void AnnealingAlgorithm(int graphOrder,
							double *&bestBC,double * targetBC,
							double &costBest);
};


class StrategyPatternAlgorithmThresholdAccepting : public StrategyPatternAlgorithm {
public:
	StrategyPatternAlgorithmThresholdAccepting(CSettingsSimulation *argummentSettingsSimulation)
	:StrategyPatternAlgorithm(argummentSettingsSimulation){
		
	}
	
	virtual void Loop(double &costNew,double &costBest,
										gslGraph ** newGraph,double tol,double *newBC, double *bestBC,
										int graphOrder,int &weAreDone, double Tk,int N){
		CFuncTrace lFuncTrace(true,"StrategyPatternAlgorithmThresholdAccepting::Loop");
		
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

	
	virtual void AnnealingAlgorithm(int graphOrder,
													  double *&bestBC,double * targetBC,
													  double &costBest){
		
		CFuncTrace lFuncTrace(true,"StrategyPatternAlgorithmThresholdAccepting::AnnealingAlgorithm");
		//	fprintf(logFile,"CRebuildGraph::AnnealingAlgorithm");
		double temperMin=this->settingsSimulation->tMin;
		double k=K;
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
			Tk*=k;
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

};


#endif /* defined(__rebuild_graph__SrategyPatternAlgorithm__) */
