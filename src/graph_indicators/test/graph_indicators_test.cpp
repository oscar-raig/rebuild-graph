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

#include "gslGraph.hpp"
#include "graphIndicatorBetweennessCentrality.h"
#include "FactoryMethodGraphIndicator.h"
#include "readPythonGraphFile.h"

int ORDER_WHEEL14 = 14;

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


BOOST_AUTO_TEST_CASE( UTest_gslGraph_CommunicabilityBetweennessCentrality_krackhardt_kite){
    
   
    gslGraph *krackhardtKiteGraph =   ReadPythonGraphFile::readPythonGraphFile( DIR_GRAPHS "krackhardt_kite_grap.adjlist" );
    
    
    double *communicability_betweeness_centrality = NULL;
    double expectedCommunicabilitybetweenessCentrality[10]={ 0.259,0.259,0.164,0.497,
        0.164,0.497,0.497,0.451,0.233,0.010};
    
    
    graphIndicator *communicabilityBetweennessCentrality = FactoryMethodGraphIndicator::createGraphIndicator(COMMUNICABILITY_BETWEENESS_CENTRALITY,krackhardtKiteGraph);
    
    communicability_betweeness_centrality = communicabilityBetweennessCentrality->calculateIndicatorWithReescale(true);
    for (int i = 0; i < krackhardtKiteGraph->getOrder(); i++){
        BOOST_CHECK( abs(communicability_betweeness_centrality[i] - expectedCommunicabilitybetweenessCentrality[i])< 0.01);
    }
    
    delete krackhardtKiteGraph;
    delete communicability_betweeness_centrality;
    delete communicabilityBetweennessCentrality;
    
   
}

BOOST_AUTO_TEST_CASE( UTest_gslGraph_CommunicabilityBetweennessCentrality_test_4nodes){
    
   
    
    gslGraph *krackhardtKiteGraph =   ReadPythonGraphFile::readPythonGraphFile( DIR_GRAPHS "test_4nodes.gpfc" );
    
    
    double *communicability_betweeness_centrality = NULL;
    double expectedCommunicabilitybetweenessCentrality[4]={ 0.07017,0.71565,0.71565,0.07017};
        
    graphIndicator *communicabilityBetweennessCentrality = FactoryMethodGraphIndicator::createGraphIndicator(COMMUNICABILITY_BETWEENESS_CENTRALITY,krackhardtKiteGraph);
    
    communicability_betweeness_centrality = communicabilityBetweennessCentrality->calculateIndicatorWithReescale(true);
    for (int i = 0; i < krackhardtKiteGraph->getOrder(); i++){
        BOOST_CHECK( abs(communicability_betweeness_centrality[i] - expectedCommunicabilitybetweenessCentrality[i])< 0.01);
    }
    
    delete krackhardtKiteGraph;
    delete communicability_betweeness_centrality;
    delete communicabilityBetweennessCentrality;
    
    
}

BOOST_AUTO_TEST_CASE(UTest_graphIndicatorBetweennessCentrality_anyUnconnectedVertex){
    
    
    graphIndicatorBetweennessCentrality *BetweennessCentrality =
    new graphIndicatorBetweennessCentrality(NULL);

    
    gslGraph *generalGraph =   ReadPythonGraphFile::readPythonGraphFile( DIR_GRAPHS "wheel14.txt" );
    // Vertex 0 in wheel14 is connected to 13 vertex
    // if we remove its neighbours, node 0 is unconnected
    
    generalGraph->removeVertexNeighbours(0);

    gsl_vector * vector = BetweennessCentrality->anyUnconnectedVertex(generalGraph->getGslMatrix());
    
    BOOST_CHECK(gsl_vector_get(vector,0)==0);
    for (int i = 1; i < ORDER_WHEEL14 ;i++){
        BOOST_CHECK(gsl_vector_get(vector,i)==1);
    }
    
    generalGraph->removeVertexNeighbours(1);
    
    gsl_vector_free(vector);
    vector = BetweennessCentrality->anyUnconnectedVertex(generalGraph->getGslMatrix());
    
    BOOST_CHECK(gsl_vector_get(vector,0)==0);
    BOOST_CHECK(gsl_vector_get(vector,1)==0);
    for (int i = 2; i < ORDER_WHEEL14 ;i++){
        BOOST_CHECK(gsl_vector_get(vector,i)==1);
    }
      
}



