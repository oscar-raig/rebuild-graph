//
//  main.cpp
//  TestIndicatorBetweennessCentrality
//
//  Created by Oscar Raig Colon on 21/2/15.
//  Copyright (c) 2015 Oscar Raig Colon. All rights reserved.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <iostream> 
#include "gslGraph.h"
#include "graphIndicatorBetweennessCentrality.h"
#include "readPythonGraphFile.h"



#define DIR_GRAPHS "./data/"



BOOST_AUTO_TEST_CASE(UTest_graphIndicatorBC_getNumberOfNonZeroInVector_NoNonZeros) {

    graphIndicatorBetweennessCentrality *betweennessCentrality =
        new graphIndicatorBetweennessCentrality ( NULL );

    gsl_vector *  vectorWithAllZeros = gsl_vector_calloc(10);
    int exepectedNoNonZeros =
        betweennessCentrality->getNumberOfNonZeroInVector(vectorWithAllZeros);

    BOOST_CHECK(0 == exepectedNoNonZeros);

    gsl_vector_free(vectorWithAllZeros);
    delete betweennessCentrality;


}

BOOST_AUTO_TEST_CASE(UTest_graphIndicatorBC_getNumberOfNonZeroInVector_2NonZeros) {
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

}


BOOST_AUTO_TEST_CASE( UTest_graphIndicatorBetweennessCentrality_BetweennessCentrality_wheel5) {

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

}

BOOST_AUTO_TEST_CASE( UTest_graphIndicatorBetweennessCentrality_BetweennessCentrality_wheel14) {
 

    gslGraph *  generalGraph =  ReadPythonGraphFile::readPythonGraphFile(DIR_GRAPHS "wheel14.txt");


    double *betweeness_centrality = NULL;

    graphIndicatorBetweennessCentrality *betweennessCentrality =
    new graphIndicatorBetweennessCentrality ( generalGraph );

    betweeness_centrality = betweennessCentrality->calculateIndicatorWithReescale(true);
    double expectedBetweenessCentralityForNode0OfWheelGraph = 0.75;
    double expectedBetweenessCentralityForRestOfWheelGraphNodes = 0.00641;
    for (int i = 0; i < generalGraph->getOrder(); i++){

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
    


}



BOOST_AUTO_TEST_CASE( UTest_gslGraph_CommunicabilityCentrality_krackhardt_kite){
    

    
    gslGraph *krackhardtKiteGraph = ReadPythonGraphFile::readPythonGraphFile( DIR_GRAPHS "krackhardt_kite_grap.adjlist" );
    
    
    double *betweeness_centrality = NULL;
    double expectedbetweenessCentrality[10]={ 0.023,0.023,0.000,0.102,
        0.000,0.231,0.231,0.389,0.222,0.000};
    
    
    graphIndicatorBetweennessCentrality *betweennessCentrality =
    new graphIndicatorBetweennessCentrality ( krackhardtKiteGraph );
    
    betweeness_centrality = betweennessCentrality->calculateIndicatorWithReescale(true);
    for (int i = 0; i < krackhardtKiteGraph->getOrder(); i++){
       
        BOOST_CHECK( abs(betweeness_centrality[i] - expectedbetweenessCentrality[i])< 0.01);
    }
    
    delete krackhardtKiteGraph;
    delete betweeness_centrality;
    delete betweennessCentrality;
    

}



