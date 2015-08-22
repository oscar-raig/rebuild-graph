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
#include "SettingsSimulation.h"
#include "CTrace.hpp"
#include "gslGraph.hpp"
#include "FactoryMethodGraphIndicator.h"
#include "graphIndicatorBetweennessCentrality.h"
#include "graphIndicatorCommunicabilityCentralityUsingMatrixExponential.h"
#include "graphIndicatorCommunicabilityBetweennessCentrality.h"

#define STP_DEBUG CTrace::TRACE_DEBUG
#define STP_INFO CTrace::TRACE_DEBUG

#ifndef STRING_LENGTH
#define STRING_LENGTH 256
#endif

class StrategyPatternAlgorithm {
	
protected:
	SettingsSimulation *settingsSimulation;
	bool settingsSimulationAllocated;
	gslGraph *sourceGraph;
	FILE *logFile;
	
public:
	StrategyPatternAlgorithm(SettingsSimulation *argummentSettingsSimulation)
	{
		CFuncTrace trace(false,"StrategyPatternAlgorithm");
		
		if ( argummentSettingsSimulation){
			settingsSimulationAllocated= false;
			settingsSimulation = argummentSettingsSimulation;
		}
		else{
			settingsSimulationAllocated = true;
			settingsSimulation = new SettingsSimulation();
		}
	};

	~StrategyPatternAlgorithm(){
		if (sourceGraph)
			delete sourceGraph;
		if (settingsSimulation && settingsSimulationAllocated )
			delete settingsSimulation;
	}

	
	
	void acceptChangesInGraph(double &costBest,double costNew,gslGraph * newGraph,double *newBC, double *bestBC){
		costBest=costNew;
		this->setGraph( newGraph->copyGraph() );
		memcpy(bestBC,newBC,newGraph->getOrder()*sizeof(double));
	}
	
	void discardChangeInGraph( gslGraph ** newGraph ){
		if (*newGraph)
			delete *newGraph;
		//newGraph = sourceGraph->copyGraph();
		*newGraph = this->getGraph()->copyGraph();
	}
	
	double * CalculateIndicator(gslGraph *Graph);
	
	virtual bool AreChangesAccepted( double costNew, double costBest,double Tk){
		CFuncTrace lFuncTrace(false,"StrategyPatternAlgorithm::AreChangesAccepted");
		return (costNew<costBest);
	}
	virtual bool AreChangesAcceptedRandomly(double costNew, double costBest,double Tk ){
		CFuncTrace lFuncTrace(false,"StrategyPatternAlgorithm::AreChangesAcceptedRandomly");
		
		return (exp((costBest-costNew)/Tk)>generateRandomNumber());
	}
	
	void Loop(double &costNew,double &costBest,
					  gslGraph ** newGraph,double *newBC, double *bestBC,
					  int graphOrder,int &weAreDone, double Tk){
		CFuncTrace lFuncTrace(false,"StrategyPatternAlgorithm::Loop");
	//	double Tk=settingsSimulation->tMin;
		if(AreChangesAccepted(costNew,costBest,Tk)){
			acceptChangesInGraph(costBest,costNew,*newGraph,newBC,bestBC);
			fprintf(logFile,".");
		} else if(AreChangesAcceptedRandomly(costNew,costBest,Tk)){
			// if newCost not is better than oldCost,
			// we still accept it if exp(df/T_k)<rand()
			fprintf(logFile,"o");
		} else {
			discardChangeInGraph( newGraph );
			fprintf(logFile,"x");
		}
		if(costBest<=settingsSimulation->tMin){
			lFuncTrace.trace(STP_INFO,"We are Done costBest < tol");
			weAreDone=true;
			return;
		}
		
	}
	
	
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

	void AnnealingAlgorithm(int graphOrder,
							double *&bestBC,double * targetBC,
							double &costBest);
};


class StrategyPatternAlgorithmThresholdAccepting : public StrategyPatternAlgorithm {
public:
	
	StrategyPatternAlgorithmThresholdAccepting(SettingsSimulation *argummentSettingsSimulation)
	:StrategyPatternAlgorithm(argummentSettingsSimulation){
		
		
	}
	
	virtual bool AreChangesAccepted( double costNew, double costBest,double Tk){
		
		CFuncTrace lFuncTrace(false,"StrategyPatternAlgorithmThresholdAccepting::AreChangesAccepted");
		

		if (costNew < costBest)
			return true;
		else
			return false;
		
	}
	virtual bool AreChangesAcceptedRandomly(double costNew, double costBest,double Tk ){
		CFuncTrace lFuncTrace(false,"StrategyPatternAlgorithmThresholdAccepting::AreChangesAcceptedRandomly");
		double TRHESHOLD = Tk;
		return ((costNew -costBest  )< ( TRHESHOLD * costBest));
	}
	

	
	
};


#endif /* defined(__rebuild_graph__SrategyPatternAlgorithm__) */
