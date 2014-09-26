//
//  main.cpp
//  TestClass
//
//  Created by Oscar Raig Colon on 27/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

//#define BOOST_TEST_MODULE MyTest

#include <iostream>
#include <boost/test/unit_test.hpp>

#include "GeneralGraph.h"
#include "rebuildgraph.h"
#include "GraphFactory.h"

using namespace boost::unit_test;




#define DIR_GRAPHS "/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/"
/*
BOOST_AUTO_TEST_CASE(test_communicability_beetweeness_centrality){

	BOOST_CHECK(0 > 1);

}*/
void test_readGraphFromNULLFile(){
	GeneralGraph *  generalGraph = GraphFactory::createGraph(USED_GRAPH);
	BOOST_REQUIRE_THROW(generalGraph->readPythonGraphFile(NULL), std::exception);
}

void test_readGraphFromFileNotExists(){
	GeneralGraph *  generalGraph = GraphFactory::createGraph(USED_GRAPH);
	BOOST_REQUIRE_THROW(generalGraph->readPythonGraphFile("filenotfound"), std::exception);
}

void test_readGraphFromFilewheel10(){
	GeneralGraph *  generalGraph = GraphFactory::createGraph(USED_GRAPH);
	generalGraph->readPythonGraphFile(DIR_GRAPHS "test.gpfc");
	int degree = generalGraph->getDegree();
	int order = generalGraph->getOrder();
	BOOST_CHECK(degree == 3);
	BOOST_CHECK(order ==7);
}


void test_removeVertexNeighbours(){
	GeneralGraph *  generalGraph = GraphFactory::createGraph(USED_GRAPH);
	generalGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	int degree = generalGraph->getDegree();
	int order = generalGraph->getOrder();

	for (int i = 0; i< 14; i++) {
		int degree = generalGraph->getDegree(i);
		if ( i == 0)
			BOOST_CHECK(degree == 13);
		else
			BOOST_CHECK(degree ==3);
	}
	
	BOOST_CHECK(order ==14);
	generalGraph->removeVertexNeighbours(0);
	
	order = generalGraph->getOrder();

	BOOST_CHECK(order ==14);
	for (int i = 0; i< 14; i++) {
		int degree = generalGraph->getDegree(i);
		if ( i == 0)
			BOOST_CHECK(degree == 0);
		else

		BOOST_CHECK(degree ==2);
	}

	delete generalGraph;
}
void test_vertexAreNeighbours(){
	GeneralGraph *  generalGraph = GraphFactory::createGraph(USED_GRAPH);
	generalGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	for  ( int i = 1; i < 14; i++){
		BOOST_CHECK(generalGraph->vertexAreNeighbours(0,i )==1);
		BOOST_CHECK(generalGraph->vertexAreNeighbours(i,0)==1);
	}
	BOOST_CHECK(generalGraph->vertexAreNeighbours(2,4)==0);
}

void test_brandes_comunicability_centrality_exp(){
	GeneralGraph *  generalGraph = GraphFactory::createGraph(USED_GRAPH);
	generalGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	double bcc_exp[generalGraph->getOrder()];
	generalGraph->brandes_comunicability_centrality_exp(bcc_exp);
	for (int i = 0; i < generalGraph->getOrder(); i++){
//		std::cout << bcc_exp[i] << std::endl;
		BOOST_CHECK(bcc_exp[i] != 0 );

		if ( i == 0)
			BOOST_CHECK(abs(bcc_exp[i] -42.03) <0.1);
		else{
			BOOST_CHECK(abs(bcc_exp[i] -7.29) <0.1);
		}
	}
}


void test_brandes_comunicability_centrality_wheel14(){
	GeneralGraph *  generalGraph = GraphFactory::createGraph(USED_GRAPH);
	generalGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	double bcc_exp[generalGraph->getOrder()];
	
	generalGraph->brandes_betweenness_centrality(bcc_exp);
	for (int i = 0; i < generalGraph->getOrder(); i++){
		std::cout << "Pos " << i << " : " << bcc_exp[i] << std::endl;
		BOOST_CHECK(bcc_exp[i] != 0 );
		if ( i == 0)
			BOOST_CHECK(abs(bcc_exp[i] -0.75) <0.1);
		else{
			BOOST_CHECK(abs(bcc_exp[i] -0.00641) <0.1);
		}
	}
}




void test_brandes_comunicability_centrality_test_4nodes(){
	GeneralGraph *  generalGraph = GraphFactory::createGraph(USED_GRAPH);
	generalGraph->readPythonGraphFile(DIR_GRAPHS "test_4nodes.gpfc");
	double bcc_exp[generalGraph->getOrder()];
	
	generalGraph->brandes_betweenness_centrality(bcc_exp);
	for (int i = 0; i < generalGraph->getOrder(); i++){
		std::cout << "Pos " << i << " : " << bcc_exp[i] << std::endl;
		if ( i == 0 || i == 3)
			BOOST_CHECK(bcc_exp[i] == 0);
		else{
			BOOST_CHECK(bcc_exp[i] != 0 );
			BOOST_CHECK(abs(bcc_exp[i] -0.66) <0.1);
		}
	}
}



void test_setAllVertexneigbours(){
	GeneralGraph *  generalGraph = GraphFactory::createGraph(USED_GRAPH);
	generalGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	GeneralGraph * newGraph =  GraphFactory::createGraph(USED_GRAPH);
	newGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
    newGraph->printGraph();
	newGraph->setAllVertexNeighbours();
	newGraph->printGraph();
	if (generalGraph->GetType() == GRAPH )
		((graph *)generalGraph)->copyGraph((graph*)newGraph);
	else
		((gslGraph *)generalGraph)->copyGraph((gslGraph*)newGraph);
	
	gsl_matrix *targetGraphGsl = gsl_matrix_alloc(generalGraph->getOrder(), generalGraph->getOrder());
	gsl_matrix *bestGraphGsl = gsl_matrix_alloc(newGraph->getOrder(), newGraph->getOrder());
	
	generalGraph->graphToGsl(targetGraphGsl);
	newGraph->graphToGsl(bestGraphGsl);
	
	double compare = CRebuildGraph::compareMatrix(targetGraphGsl, bestGraphGsl);
	
	
	BOOST_CHECK(compare < 0.00001);
	printf ("Compare  = %f",compare);
	
	newGraph->printGraph();
	newGraph->setAllVertexNeighbours();
	
	newGraph->graphToGsl(bestGraphGsl);
	compare = CRebuildGraph::compareMatrix(targetGraphGsl, bestGraphGsl);
	BOOST_CHECK(compare < 0.00001);
	printf ("Compare  = %f",compare);
	
	newGraph->removeVertexNeighbours(0);
	newGraph->graphToGsl(bestGraphGsl);
	
	
	generalGraph->removeVertexNeighbours(1);
	generalGraph->graphToGsl(targetGraphGsl);
	
	compare = CRebuildGraph::compareMatrix(targetGraphGsl, bestGraphGsl);
	newGraph->graphToGsl(bestGraphGsl);
	BOOST_CHECK(compare > 0.00001);
	printf ("Compare  = %f",compare);


}



typedef void (*func_type)(void);

func_type functions[]={
	test_readGraphFromNULLFile,
	test_readGraphFromFileNotExists,
	test_readGraphFromFilewheel10,
	test_removeVertexNeighbours,
	test_vertexAreNeighbours,
	test_brandes_comunicability_centrality_exp,
	test_setAllVertexneigbours,
	test_brandes_comunicability_centrality_wheel14,
	test_brandes_comunicability_centrality_test_4nodes
	
};

void getFirstTest(int * iTerator, func_type * test){
	*iTerator = 0;
	*test = functions[*iTerator];
}

void addTest( func_type  test ){
	
	framework::master_test_suite().
	add( BOOST_TEST_CASE( test ));
}

void nextTest(int *iTerator, func_type * test){
	(*iTerator)++;
	*test = functions[*iTerator];
}

int isLastTest(int iTerator){
	return iTerator < (sizeof(functions)/sizeof(func_type));
}

test_suite*
init_unit_test_suite( int argc, char* argv[] )
{
	func_type  test;
	int testIterator = 0;
	getFirstTest(&testIterator,&test );
	while(isLastTest (testIterator) ){
		addTest(test);
		nextTest(&testIterator,&test);
	
	}
    return 0;
}