//
//  main.cpp
//  TestIndicatorBetweennessCentrality
//
//  Created by Oscar Raig Colon on 21/2/15.
//  Copyright (c) 2015 Oscar Raig Colon. All rights reserved.
//

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "CTrace.hpp"
#include "gslGraph.h"
#include "graphIndicatorBetweennessCentrality.h"
using namespace boost::unit_test;

int numberOfTestExeccuted = 0;
int numberOfTests = 0;
#define DIR_GRAPHS "/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/"

void UTest_graphIndicatorBetweennessCentrality_getNumberOfNonZeroInVector_logical_not_1010(){
	CFuncTrace trace(true,"UTest_getNumberOfNonZeroInVector_NoNonZeros");
	graphIndicatorBetweennessCentrality *betweennessCentrality =
	new graphIndicatorBetweennessCentrality ( NULL );
	
	gsl_vector *  vector1010= gsl_vector_calloc(4);
	gsl_vector *  vector0101 = betweennessCentrality->logical_not(vector1010);
	
	for ( int i = 0; i < 4; i++){
		bool Iszero10101 = betweennessCentrality->fp_zero(gsl_vector_get(vector1010,i));
		bool Iszero01010 =  betweennessCentrality->fp_zero(gsl_vector_get(vector0101,i));

		BOOST_CHECK( Iszero10101 != Iszero01010);
	}
	
	gsl_vector_free( vector0101 );
	gsl_vector_free( vector1010 );
	delete betweennessCentrality;
	
	trace.trace(CTrace::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}

void UTest_graphIndicatorBetweennessCentrality_getNumberOfNonZeroInVector_NoNonZeros(){
	
	CFuncTrace trace(true,"UTest_getNumberOfNonZeroInVector_NoNonZeros");
	graphIndicatorBetweennessCentrality *betweennessCentrality =
	new graphIndicatorBetweennessCentrality ( NULL );
	
	gsl_vector *  vectorWithAllZeros= gsl_vector_calloc(10);
	int exepectedNoNonZeros = betweennessCentrality->getNumberOfNonZeroInVector(vectorWithAllZeros);
	
	BOOST_CHECK(0 == exepectedNoNonZeros);
	
	gsl_vector_free( vectorWithAllZeros );
	delete betweennessCentrality;
	
	trace.trace(CTrace::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
}

void UTest_graphIndicatorBetweennessCentrality_getNumberOfNonZeroInVector_2NonZeros(){
	CFuncTrace trace(true,"UTest_getNumberOfNonZeroInVector");
	graphIndicatorBetweennessCentrality *betweennessCentrality =
	new graphIndicatorBetweennessCentrality ( NULL );
	
	gsl_vector * vectorWith2NonZeros = gsl_vector_calloc(10);
	gsl_vector_set(vectorWith2NonZeros,0,1);
	gsl_vector_set(vectorWith2NonZeros,9,1);
	int exepected2NonZeros = betweennessCentrality->getNumberOfNonZeroInVector(vectorWith2NonZeros);
	
	BOOST_CHECK(2 == exepected2NonZeros);
	
	gsl_vector_free(vectorWith2NonZeros);
	delete betweennessCentrality;
	
	trace.trace(CTrace::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);
	
}

void UTest_graphIndicatorBetweennessCentrality_BetweennessCentrality_wheel14(){
	
	CFuncTrace trace(true,"UTest_BetweennessCentrality_wheel14");
	
	gslGraph *  generalGraph =  new gslGraph();
	generalGraph->readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
	
	
	double *betweeness_centrality = NULL;
	
	graphIndicatorBetweennessCentrality *betweennessCentrality =
	new graphIndicatorBetweennessCentrality ( generalGraph );
	
	betweeness_centrality = betweennessCentrality->calculateIndicator();
	double expectedBetweenessCentralityForNode0OfWheelGraph = 0.75;
	double expectedBetweenessCentralityForRestOfWheelGraphNodes = 0.00641;
	for (int i = 0; i < generalGraph->getOrder(); i++){
//		std::cout << "Pos " << i << " : " << betweeness_centrality[i] << std::endl;
		BOOST_CHECK(betweeness_centrality[i] != 0 );
		if ( i == 0)
			BOOST_CHECK(abs(betweeness_centrality[i] - expectedBetweenessCentralityForNode0OfWheelGraph) <0.1);
		else{
			BOOST_CHECK(abs(betweeness_centrality[i] - expectedBetweenessCentralityForRestOfWheelGraphNodes) <0.1);
		}
	}
	
	delete generalGraph;
	
	trace.trace(CTrace::TRACE_INFO,"Executing test %d/%d",
				++numberOfTestExeccuted,numberOfTests);

}

typedef void (*func_type)(void);

func_type functions[]={
	UTest_graphIndicatorBetweennessCentrality_BetweennessCentrality_wheel14,
	UTest_graphIndicatorBetweennessCentrality_getNumberOfNonZeroInVector_2NonZeros,
	UTest_graphIndicatorBetweennessCentrality_getNumberOfNonZeroInVector_NoNonZeros,
	UTest_graphIndicatorBetweennessCentrality_getNumberOfNonZeroInVector_logical_not_1010};

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
		trace.trace(CTrace::TRACE_DEBUG,"Adding test %d",numberOfTests++);
		nextTest(&testIterator,&test);
		
	}
	return 0;
}
