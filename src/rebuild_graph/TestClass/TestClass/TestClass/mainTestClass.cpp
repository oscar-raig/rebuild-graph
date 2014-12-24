//
//  main.cpp
//  TestClass
//
//  Created by Oscar Raig Colon on 27/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

// SUMMARY:
// Test gslGraph class
//		rebuildGraph class
//		graphindicatorBetweenessCentrality


//#define BOOST_TEST_MODULE MyTest

#include <iostream>
#include <boost/test/unit_test.hpp>

#include "gslGraph.h"
#include "rebuildgraph.h"
#include "graphIndicatorBetweennessCentrality.h"



using namespace boost::unit_test;

int numberOfTests = 0;
int numberOfTestExeccuted = 0;

#define DIR_GRAPHS "/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/"
/*
BOOST_AUTO_TEST_CASE(test_communicability_beetweeness_centrality){

	BOOST_CHECK(0 > 1);

}*/
void UTest_gslGraph_readPythonGraphFile_NULLFile(){
	CFuncTrace trace(true,"UTest_readGraphFromNULLFile");
	
	gslGraph *  generalGraph = new gslGraph();
	BOOST_REQUIRE_THROW(generalGraph->readPythonGraphFile(NULL), std::exception);
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
					++numberOfTestExeccuted,numberOfTests);
}

void UTest_gslGraph_readPythonGraphFile_FileNotExists(){
	CFuncTrace trace(true,"UTest_readGraphFromFileNotExists");
	
	gslGraph *  generalGraph =  new gslGraph();
	BOOST_REQUIRE_THROW(generalGraph->readPythonGraphFile("filenotfound"), std::exception);
	
	delete generalGraph;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}

void UTest_gslGraph_readPythonGraphFile_wheel10(){
	CFuncTrace trace(true,"test_readGraphFromFilewheel10");
	
	gslGraph *  generalGraph = new gslGraph();
	generalGraph->readPythonGraphFile(DIR_GRAPHS "test.gpfc");
	
	int degree = generalGraph->getDegree();
	int order = generalGraph->getOrder();
	
	BOOST_CHECK(degree == 3);
	BOOST_CHECK(order ==7);
	
	delete generalGraph;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}




void Utest_gslGraph_readPythonGraphFile_4nodes(){
	CFuncTrace trace(true,"test_readGraphFromFiletest_4nodes");
	
	gslGraph *  generalGraph =  new gslGraph();
	generalGraph->readPythonGraphFile(DIR_GRAPHS "test_4nodes.gpfc");
	
	int degree = generalGraph->getDegree();
	int order = generalGraph->getOrder();
	
	generalGraph->printGraph();
	BOOST_CHECK(degree == 2);
	BOOST_CHECK(order == 4);
	
	delete generalGraph;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}


#define ORDER_WHEEL14 14
#define DEGREE_CENTRAL_VERTEX_WHEEL14 13
#define DEGREE_EXTERNAL_VERTEX_WHEEL14 3

void UTest_gslGraph_removeVertexNeighbours_wheel14(){
	CFuncTrace trace(true,"test_removeVertexNeighbours");
	
	gslGraph *  generalGraph = new gslGraph();
	generalGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	
	int graphdegree = generalGraph->getDegree();
	int graphorder = generalGraph->getOrder();
	
	BOOST_CHECK( graphorder == ORDER_WHEEL14 );
	BOOST_CHECK( graphdegree == DEGREE_CENTRAL_VERTEX_WHEEL14 );

	for (int i = 0; i< ORDER_WHEEL14; i++) {
		int degree = generalGraph->getDegree(i);
		if ( i == 0 )
			BOOST_CHECK(degree == DEGREE_CENTRAL_VERTEX_WHEEL14);
		else
			BOOST_CHECK(degree == DEGREE_EXTERNAL_VERTEX_WHEEL14);
	}
	
	
	// Vertex 0 in wheel14 is connected to 13 vertex
	// if we remove its neighbours, order remain equal
	// degree becomes 2
	generalGraph->removeVertexNeighbours(0);
	
	graphorder = generalGraph->getOrder();
	graphdegree = generalGraph->getDegree();
	
	BOOST_CHECK(graphdegree == 2);
	
	generalGraph->printGraph();

	BOOST_CHECK( graphorder == ORDER_WHEEL14 );
	
	for (int i = 0; i< ORDER_WHEEL14; i++) {
		int degree = generalGraph->getDegree(i);
		if ( i == 0)
			BOOST_CHECK( degree == 0 );
		else

		BOOST_CHECK( degree == 2 );
	}

	delete generalGraph;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}

void UTest_gslGraph_vertexAreNeighbours(){
	CFuncTrace trace(true,"test_vertexAreNeighbours");
	
	gslGraph *  generalGraph =  new gslGraph();
	generalGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	
	for  ( int i = 1; i < 14; i++ ){
		BOOST_CHECK( generalGraph->vertexAreNeighbours(0,i) == 1);
		BOOST_CHECK( generalGraph->vertexAreNeighbours(i,0) == 1);
	}
	BOOST_CHECK( generalGraph->vertexAreNeighbours(2,4) == 0);
	
	delete generalGraph;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}

void UTest_brandes_comunicability_centrality_exp(){
	CFuncTrace trace(true,"test_brandes_comunicability_centrality_exp");
	
	gslGraph *  generalGraph =  new gslGraph();
	
	generalGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	
	double *bcc_exp = new double[generalGraph->getOrder()];
	generalGraph->brandes_comunicability_centrality_exp(bcc_exp);
	for (int i = 0; i < generalGraph->getOrder(); i++){
		BOOST_CHECK(bcc_exp[i] != 0 );
		if ( i == 0)
			BOOST_CHECK(abs(bcc_exp[i] -42.03) < 0.1);
		else{
			BOOST_CHECK(abs(bcc_exp[i] -7.29) < 0.1);
		}
	}
	
	delete generalGraph;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}


void test_brandes_comunicability_centrality_wheel14(){
	CFuncTrace trace(true,"test_brandes_comunicability_centrality_wheel14");
	
	gslGraph *  generalGraph =  new gslGraph();
	generalGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	
	
	double *betweeness_centrality = NULL;
	
	graphIndicatorBetweennessCentrality *betweennessCentrality =
	new graphIndicatorBetweennessCentrality ( generalGraph );
	
	betweeness_centrality = betweennessCentrality->calculateIndicator();
	
	for (int i = 0; i < generalGraph->getOrder(); i++){
		std::cout << "Pos " << i << " : " << betweeness_centrality[i] << std::endl;
		BOOST_CHECK(betweeness_centrality[i] != 0 );
		if ( i == 0)
			BOOST_CHECK(abs(betweeness_centrality[i] -0.75) <0.1);
		else{
			BOOST_CHECK(abs(betweeness_centrality[i] -0.00641) <0.1);
		}
	}
	
	delete generalGraph;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}




void test_brandes_comunicability_centrality_test_4nodes(){
	CFuncTrace trace(true,"test_brandes_comunicability_centrality_test_4nodes");
	
	gslGraph *  generalGraph =  new gslGraph();
	generalGraph->readPythonGraphFile(DIR_GRAPHS "test_4nodes.gpfc");
	double *betweeness_centrality = NULL;
	
	graphIndicatorBetweennessCentrality *betweennessCentrality =
		new graphIndicatorBetweennessCentrality ( generalGraph );
	
	betweeness_centrality = betweennessCentrality->calculateIndicator();
	for (int i = 0; i < generalGraph->getOrder(); i++){
		std::cout << "Pos " << i << " : " << betweeness_centrality[i] << std::endl;
		if ( i == 0 || i == 3)
			BOOST_CHECK(betweeness_centrality[i] == 0);
		else{
			BOOST_CHECK(betweeness_centrality[i] != 0 );
			BOOST_CHECK(abs(betweeness_centrality[i] -0.66) <0.1);
		}
	}
	
	delete generalGraph;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}

void addVertexAndTestDegreeAndOrder( gslGraph *graph,
										  int vertexOrig,
										  int vertexNeigbour,
										  int ExpectedDegree,
										  int ExpectedOrder)
{
	graph->addVertexNeighbour(vertexOrig,vertexNeigbour);
	int graphDegree = graph->getDegree();
	BOOST_CHECK( graphDegree == ExpectedDegree );
	int graphOrder = graph->getOrder();
	BOOST_CHECK( graphOrder == ExpectedOrder );
}


void removeVertexAndTestDegreeAndOrder( gslGraph *graph,
										  int vertex, int ExpectedDegree,
										  int ExpectedOrder)
{
	graph->removeVertexNeighbours(vertex);
	int graphDegree = graph->getDegree();
	BOOST_CHECK( graphDegree == ExpectedDegree );
	int graphOrder = graph->getOrder();
	BOOST_CHECK( graphOrder == ExpectedOrder );
}


// Code Complete
// layout every test in a paragraph

void UTest_gslGraph_adNewVertexNeighbour_and_check_order_and_degree(){
	CFuncTrace trace( true, "UTest_gslGraph_adNewVertexNeighbour_and_check_order_and_degree");
	
	gslGraph *graph = NULL;
	int degree = 0;
	int order = 0;
	
	graph =  new gslGraph( 5 );
	degree = graph->getDegree();
	order = graph->getOrder();
	BOOST_CHECK( degree == 0 );
	BOOST_CHECK( order == 5 );
	
	// adding a conection between 0 and 1
	addVertexAndTestDegreeAndOrder( graph,0,1,1,5 );
	
	// removing the connection between 0 and 1
	removeVertexAndTestDegreeAndOrder(graph,0,0,5);
	
	// removing a removed connection
	graph->removeVertexNeighbours(1);
	
	degree = graph->getDegree();
	order = graph->getOrder();
	BOOST_CHECK( degree == 0 );
	BOOST_CHECK( order == 5 );
	
	//removing a inexisting connection
	removeVertexAndTestDegreeAndOrder(graph,1,0,5);


	// adding a triangle
	graph->addNewVertexNeighbour( 0,1 );
	graph->addNewVertexNeighbour( 0,3 );
	addVertexAndTestDegreeAndOrder( graph,3,1,2,5 );
	
	// adding a new Vertex
	addVertexAndTestDegreeAndOrder( graph,5,0,3,6 );
	
	

	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}
void test_setAllVertexneigbours(){
	CFuncTrace trace(true,"test_setAllVertexneigbours");
	
	gslGraph *  generalGraph =  new gslGraph();
	generalGraph->readPythonGraphFile( DIR_GRAPHS "wheel14.txt" );
	gslGraph * newGraph =   new gslGraph();
	newGraph->readPythonGraphFile( DIR_GRAPHS "wheel14.txt" );
    newGraph->printGraph();
	newGraph->printGraph();
	
	newGraph =	generalGraph->copyGraph();
	
//	generalGraph->copyGraph(newGraph);
	gsl_matrix *targetGraphGsl = gsl_matrix_alloc(generalGraph->getOrder(), generalGraph->getOrder());
	gsl_matrix *bestGraphGsl = gsl_matrix_alloc(newGraph->getOrder(), newGraph->getOrder());
	
	generalGraph->graphToGsl(targetGraphGsl);
	newGraph->graphToGsl(bestGraphGsl);
	
	double compare = CRebuildGraph::compareMatrix(targetGraphGsl, bestGraphGsl);
	
	
	BOOST_CHECK(compare < 0.00001);
	printf ("Compare  = %f",compare);
	
	newGraph->printGraph();
	
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

	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);

}

void UTest_2copyGraph(){
	CFuncTrace trace(true,"UTest_2copyGraph");
	
	gslGraph *general =  new gslGraph();
	general->readPythonGraphFile(DIR_GRAPHS "test_4nodes.gpfc");
	
	gslGraph *copy1 = general->copyGraph();
	
	gslGraph *copy2 = general->copyGraph();
	
	gsl_matrix *copy1matrix = gsl_matrix_alloc(copy1->getOrder(), copy1->getOrder());
	copy1->graphToGsl(copy1matrix);
	
	gsl_matrix *copy2matrix = gsl_matrix_alloc(copy2->getOrder(), copy2->getOrder());
	copy2->graphToGsl(copy2matrix);

	double compare = CRebuildGraph::compareMatrix(copy1matrix, copy2matrix);
	BOOST_CHECK(compare < 0.00001);
	
	delete general;
	delete copy1;
	delete copy2;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);

}

void
UTest_gslGraph_compare_bad_parameter_1(void){
	CFuncTrace trace(true,"UTest_gslGraph_compare_bad_parameter_1");
	
	gslGraph *general =  new gslGraph();
	general->readPythonGraphFile(DIR_GRAPHS "test_4nodes.gpfc");

	BOOST_REQUIRE_THROW(general->compare(NULL, general), std::exception);
	
	delete general;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
	
}
void
UTest_gslGraph_compare_bad_parameter_2(void){
	CFuncTrace trace(true,"UTest_gslGraph_compare_bad_parameter_2");
	
	gslGraph *general =  new gslGraph();
	general->readPythonGraphFile(DIR_GRAPHS "test_4nodes.gpfc");
	
	BOOST_REQUIRE_THROW( general->compare(NULL, general), std::exception);
	
	delete general;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
	
}
void
UTest_gslGraph_compare_different_graphs(void){
	
	CFuncTrace trace(true,"UTest_gslGraph_compare_bad_parameter_2");
	
	gslGraph *graph1 =  new gslGraph();
	graph1->readPythonGraphFile(DIR_GRAPHS "test_4nodes.gpfc");
	
	gslGraph *graph2 =   new gslGraph();
	graph2->readPythonGraphFile( DIR_GRAPHS "wheel14.txt" );
	
	int areGraphEquals = gslGraph::compare(graph1, graph2);
	
	BOOST_CHECK( areGraphEquals == 0 );
	
	
	delete graph1;
	delete graph2;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}
void
UTest_gslGraph_compare_equal_graphs(void){
	CFuncTrace trace(true,"UTest_gslGraph_compare_bad_parameter_2");
	
	gslGraph *graph1 =  new gslGraph();
	graph1->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	
	gslGraph *graph2 =   new gslGraph();
	graph2->readPythonGraphFile( DIR_GRAPHS "wheel14.txt" );
	
	int areGraphEquals = gslGraph::compare(graph1, graph2);
	
	BOOST_CHECK( areGraphEquals == gslGraph::COMPARE::GRAPH_EQUALS);
	
	
	delete graph1;
	delete graph2;
	
	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);

	
}


void UTest_graphIndicatorBetweennessCentrality_ordinal_index(){
	CFuncTrace trace(true,"UTest_graphIndicatorBetweennessCentrality_ordinal_index");

	graphIndicatorBetweennessCentrality *BetweennessCentrality =
	new graphIndicatorBetweennessCentrality(NULL);
	
	BOOST_REQUIRE_THROW( BetweennessCentrality->submatrix(NULL,NULL,NULL), std::exception);
	
	gslGraph *graphwheel =   new gslGraph();
	graphwheel->readPythonGraphFile( DIR_GRAPHS "wheel4.txt" );
	
	gsl_matrix *matrix = NULL;
	matrix = gsl_matrix_alloc(graphwheel->getOrder(), graphwheel->getOrder());
	graphwheel->graphToGsl( matrix );
	
//	BetweennessCentrality->submatrix(matrix,NULL,NULL); // for test the exception
	BOOST_REQUIRE_THROW( BetweennessCentrality->submatrix(matrix,NULL,NULL), std::exception);
	
	gsl_vector *rows = gsl_vector_alloc(3);
	
	//BetweennessCentrality->submatrix(matrix,rows,NULL); // for test the exception
	BOOST_REQUIRE_THROW( BetweennessCentrality->submatrix(matrix,rows,NULL), std::exception);

	gsl_vector *columns = gsl_vector_alloc(3);
	
	gsl_vector_set(rows,0,0);
	gsl_vector_set(rows,1,1);
	gsl_vector_set(rows,2,2);
	
	gsl_vector_set(columns,0,0);
	gsl_vector_set(columns,1,1);
	gsl_vector_set(columns,2,2);
	
	
	gsl_matrix *result_3x3_up_left = BetweennessCentrality->submatrix(matrix,rows,columns);
	
	
	
	CRebuildGraph::printGslMatrix( matrix );
	CRebuildGraph::printGslMatrix( result_3x3_up_left );
	
	
	
	gsl_vector_free(rows);
	gsl_vector_free(columns);
	gsl_matrix_free(result_3x3_up_left);
	
	rows = gsl_vector_alloc(2);
	columns = gsl_vector_alloc(2);
	
	gsl_vector_set(rows,0,1);
	gsl_vector_set(rows,1,2);
	
	gsl_vector_set(columns,0,0);
	gsl_vector_set(columns,1,1);
	
	gsl_matrix *result_2x2_middle_left = BetweennessCentrality->submatrix(matrix,rows,columns);
	
	
	CRebuildGraph::printGslMatrix( matrix );
	CRebuildGraph::printGslMatrix( result_2x2_middle_left );

	trace.trace(CTrace::level::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}


typedef void (*func_type)(void);

func_type functions[]={
	UTest_gslGraph_readPythonGraphFile_NULLFile,
	UTest_gslGraph_readPythonGraphFile_FileNotExists,
	UTest_gslGraph_readPythonGraphFile_wheel10,
	Utest_gslGraph_readPythonGraphFile_4nodes,
	UTest_gslGraph_removeVertexNeighbours_wheel14,
	UTest_gslGraph_vertexAreNeighbours,
	UTest_gslGraph_compare_bad_parameter_1,
	UTest_gslGraph_compare_bad_parameter_2,
	UTest_gslGraph_compare_different_graphs,
	UTest_gslGraph_compare_equal_graphs,
	UTest_gslGraph_adNewVertexNeighbour_and_check_order_and_degree,

	UTest_brandes_comunicability_centrality_exp,
	test_setAllVertexneigbours,
	test_brandes_comunicability_centrality_wheel14,
	test_brandes_comunicability_centrality_test_4nodes,
		UTest_2copyGraph,
	
	UTest_graphIndicatorBetweennessCentrality_ordinal_index
	
	
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
	CFuncTrace trace(false,"init_unit_test_suite");
	func_type  test;
	int testIterator = 0;
	getFirstTest(&testIterator,&test );
	while(isLastTest (testIterator) ){
		addTest(test);
		trace.trace(CTrace::level::TRACE_DEBUG,"Adding test %d",numberOfTests++);
		nextTest(&testIterator,&test);
	
	}
    return 0;
}