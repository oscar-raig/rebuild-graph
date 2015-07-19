//
//  main.cpp
//  TestIndicatorBetweennessCentrality
//
//  Created by Oscar Raig Colon on 21/2/15.
//  Copyright (c) 2015 Oscar Raig Colon. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include <iostream>
#include "CTrace.hpp"
#include "gslGraph.h"
#include "graphIndicatorBetweennessCentrality.h"
#include "readPythonGraphFile.h"


int numberOfTestExeccuted = 0;
int numberOfTests = 0;
#define DIR_GRAPHS "/Users/oscarraigcolon/Arrel/" \
    "git/rebuild-graph/data/example_graphs/"



void UTest_graphIndicatorBC_getNumberOfNonZeroInVector_NoNonZeros() {
    CFuncTrace trace(true, "UTest_getNumberOfNonZeroInVector_NoNonZeros");
    graphIndicatorBetweennessCentrality *betweennessCentrality =
        new graphIndicatorBetweennessCentrality ( NULL );

    gsl_vector *  vectorWithAllZeros = gsl_vector_calloc(10);
    int exepectedNoNonZeros =
        betweennessCentrality->getNumberOfNonZeroInVector(vectorWithAllZeros);

    BOOST_CHECK(0 == exepectedNoNonZeros);

    gsl_vector_free(vectorWithAllZeros);
    delete betweennessCentrality;

    trace.trace(CTrace::TRACE_INFO, "Executing test %d/%d",
                ++numberOfTestExeccuted, numberOfTests);
}

void UTest_graphIndicatorBC_getNumberOfNonZeroInVector_2NonZeros() {
    CFuncTrace trace(true, "UTest_getNumberOfNonZeroInVector");
    graphIndicatorBetweennessCentrality *betweennessCentrality =
    new graphIndicatorBetweennessCentrality (NULL);

    gsl_vector * vectorWith2NonZeros = gsl_vector_calloc(10);
    gsl_vector_set(vectorWith2NonZeros, 0, 1);
    gsl_vector_set(vectorWith2NonZeros, 9, 1);
    int exepected2NonZeros =
        betweennessCentrality->getNumberOfNonZeroInVector(vectorWith2NonZeros);

    BOOST_CHECK(2 == exepected2NonZeros);

    gsl_vector_free(vectorWith2NonZeros);
    delete betweennessCentrality;

    trace.trace(CTrace::TRACE_INFO, "Executing test %d/%d",
                ++numberOfTestExeccuted, numberOfTests);
}


void UTest_graphIndicatorBetweennessCentrality_BetweennessCentrality_wheel5() {
    CFuncTrace trace(true, "UTest_BetweennessCentrality_wheel5");

    gslGraph *  generalGraph =  ReadPythonGraphFile::readPythonGraphFile(DIR_GRAPHS "wheel5.txt");

    double *betweeness_centrality = NULL;

    graphIndicatorBetweennessCentrality *betweennessCentrality =
    new graphIndicatorBetweennessCentrality (generalGraph);

    betweeness_centrality =
        betweennessCentrality->calculateIndicatorWithReescale(true);
    double expectedBetweenessCentralityForNode0OfWheelGraph = 0.11;
    double expectedBetweenessCentralityForRestOfWheelGraphNodes = 0.00641;
    for (int i = 0; i < generalGraph->getOrder(); i++) {
        BOOST_CHECK(betweeness_centrality[i] != 0);
        if (i == 0) {
            BOOST_CHECK(abs(betweeness_centrality[i]
                - expectedBetweenessCentralityForNode0OfWheelGraph) < 0.1);
        } else {
            BOOST_CHECK(abs(betweeness_centrality[i]
                - expectedBetweenessCentralityForRestOfWheelGraphNodes) < 0.1);
        }
    }

    delete generalGraph;

    trace.trace(CTrace::TRACE_INFO, "Executing test %d/%d",
                ++numberOfTestExeccuted, numberOfTests);
}

void UTest_graphIndicatorBetweennessCentrality_BetweennessCentrality_wheel14() {
    CFuncTrace trace(true,"UTest_BetweennessCentrality_wheel14");

    gslGraph *  generalGraph =  ReadPythonGraphFile::readPythonGraphFile(DIR_GRAPHS "wheel14.txt");


    double *betweeness_centrality = NULL;

    graphIndicatorBetweennessCentrality *betweennessCentrality =
    new graphIndicatorBetweennessCentrality ( generalGraph );

    betweeness_centrality = betweennessCentrality->calculateIndicatorWithReescale(true);
    double expectedBetweenessCentralityForNode0OfWheelGraph = 0.75;
    double expectedBetweenessCentralityForRestOfWheelGraphNodes = 0.00641;
    for (int i = 0; i < generalGraph->getOrder(); i++){
        std::cout << "Pos " << i << " : " << betweeness_centrality[i] << std::endl;
        BOOST_CHECK(betweeness_centrality[i] != 0 );
        if ( i == 0)
            BOOST_CHECK(abs(betweeness_centrality[i]
                            - expectedBetweenessCentralityForNode0OfWheelGraph) < 0.001);
        else{
            BOOST_CHECK(abs(betweeness_centrality[i]
                            - expectedBetweenessCentralityForRestOfWheelGraphNodes) < 0.001);
        }
    }
    
    delete generalGraph;
    
    trace.trace(CTrace::TRACE_INFO,"Executing test %d/%d",
                ++numberOfTestExeccuted,numberOfTests);

}



void UTest_gslGraph_CommunicabilityCentrality_krackhardt_kite(){
    
    CFuncTrace trace(true,"UTest_gslGraph_BetweennessCentrality_krackhardt_kite");
    
    gslGraph *krackhardtKiteGraph = ReadPythonGraphFile::readPythonGraphFile( DIR_GRAPHS "krackhardt_kite_grap.adjlist" );
    
    
    double *betweeness_centrality = NULL;
    double expectedbetweenessCentrality[10]={ 0.023,0.023,0.000,0.102,
        0.000,0.231,0.231,0.389,0.222,0.000};
    
    
    graphIndicatorBetweennessCentrality *betweennessCentrality =
    new graphIndicatorBetweennessCentrality ( krackhardtKiteGraph );
    
    betweeness_centrality = betweennessCentrality->calculateIndicatorWithReescale(true);
    for (int i = 0; i < krackhardtKiteGraph->getOrder(); i++){
        std::cout << "Pos " << i << " : " << betweeness_centrality[i] << std::endl;
        std::cout << "Pos " << i << " : " << expectedbetweenessCentrality[i] << std::endl;
        BOOST_CHECK( abs(betweeness_centrality[i] - expectedbetweenessCentrality[i])< 0.01);
    }
    
    delete krackhardtKiteGraph;
    delete betweeness_centrality;
    delete betweennessCentrality;
    
    trace.trace(CTrace::TRACE_INFO,"Executing test %d/%d",
                ++numberOfTestExeccuted,numberOfTests);
}



typedef void (*func_type)(void);

func_type functions[]={
    UTest_graphIndicatorBetweennessCentrality_BetweennessCentrality_wheel5
    ,UTest_graphIndicatorBetweennessCentrality_BetweennessCentrality_wheel14
    ,UTest_graphIndicatorBC_getNumberOfNonZeroInVector_2NonZeros
    ,UTest_graphIndicatorBC_getNumberOfNonZeroInVector_NoNonZeros
    ,UTest_gslGraph_CommunicabilityCentrality_krackhardt_kite
};

void getFirstTest(int * iTerator, func_type * test){
    *iTerator = 0;
    *test = functions[*iTerator];
}

void addTest( func_type  test ){
    
    boost::unit_test::framework::master_test_suite().
    add( BOOST_TEST_CASE( test ));
}

void nextTest(int *iTerator, func_type * test){
    (*iTerator)++;
    *test = functions[*iTerator];
}

int isLastTest(int iTerator){
    return iTerator < (sizeof(functions)/sizeof(func_type));
}


boost::unit_test::test_suite*
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
