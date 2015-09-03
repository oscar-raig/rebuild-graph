//
//  main.cpp
//  TestClass
//
//  Created by Oscar Raig Colon on 27/07/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

// SUMMARY:
// Test gslGraph class

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest

#include <iostream>
#include "gslGraph.hpp"
#include "readPythonGraphFile.h"

#include <boost/test/auto_unit_test.hpp>

using namespace boost::unit_test;


#define DIR_GRAPHS "./data/"

BOOST_AUTO_TEST_CASE( UTest_gslGraph_readPythonGraphFile_NULLFile){
    
    gslGraph *  generalGraph = new gslGraph();
    
    BOOST_REQUIRE_THROW(ReadPythonGraphFile::readPythonGraphFile(NULL), std::exception);
    
    delete generalGraph;
    
}

BOOST_AUTO_TEST_CASE(UTest_gslGraph_readPythonGraphFile_FileNotExists){
    
    gslGraph *  generalGraph =  new gslGraph();
    BOOST_REQUIRE_THROW(ReadPythonGraphFile::readPythonGraphFile("filenotfound"), std::exception);
    
    delete generalGraph;
    
}

BOOST_AUTO_TEST_CASE(UTest_gslGraph_readPythonGraphFile_wheel10){
    
    gslGraph *  generalGraph = ReadPythonGraphFile::readPythonGraphFile(DIR_GRAPHS "test.gpfc");
    
    int degree = generalGraph->getDegree();
    int order = generalGraph->getOrder();
    
    
    BOOST_CHECK(degree == 3);
    BOOST_CHECK(order ==7);
    
    delete generalGraph;
    
}


BOOST_AUTO_TEST_CASE(Utest_gslGraph_readPythonGraphFile_4nodes){
    
    gslGraph *  generalGraph =  ReadPythonGraphFile::readPythonGraphFile(DIR_GRAPHS "test_4nodes.gpfc");
    
    int degree = generalGraph->getDegree();
    int order = generalGraph->getOrder();
    
   
    BOOST_CHECK(degree == 2);
    BOOST_CHECK(order == 4);
    
    delete generalGraph;
    
}


#define ORDER_WHEEL14 14
#define DEGREE_CENTRAL_VERTEX_WHEEL14 13
#define DEGREE_EXTERNAL_VERTEX_WHEEL14 3

BOOST_AUTO_TEST_CASE( UTest_gslGraph_removeVertexNeighbours_wheel14){
    
    gslGraph *  generalGraph = ReadPythonGraphFile::readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
    
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
    

    BOOST_CHECK( graphorder == ORDER_WHEEL14 );
    
    for (int i = 0; i< ORDER_WHEEL14; i++) {
        int degree = generalGraph->getDegree(i);
        if ( i == 0)
            BOOST_CHECK( degree == 0 );
        else

        BOOST_CHECK( degree == 2 );
    }

    delete generalGraph;
    
}

BOOST_AUTO_TEST_CASE( UTest_gslGraph_vertexAreNeighbours){
    
    gslGraph *  wheel14Graph =  ReadPythonGraphFile::readPythonGraphFile(DIR_GRAPHS "wheel14.txt");
    
    int numberOfVertexForWheel14Graph = 14;
    for  ( int i = 1; i < numberOfVertexForWheel14Graph; i++ ){
        BOOST_CHECK( wheel14Graph->vertexAreNeighbours(0,i) == gslGraph::VERTEX_CONNCTED);
        BOOST_CHECK( wheel14Graph->vertexAreNeighbours(i,0) == gslGraph::VERTEX_CONNCTED);
    }
    BOOST_CHECK( wheel14Graph->vertexAreNeighbours(2,4) == gslGraph::VERTEX_DISCONNECTED);
    
    delete wheel14Graph;
    
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

BOOST_AUTO_TEST_CASE( UTest_gslGraph_adNewVertexNeighbour_and_check_order_and_degree) {
    
    gslGraph *graph = NULL;
    int degree = 0;
    int order = 0;
    
    graph =  new gslGraph( 5 );
    degree = graph->getDegree();
    order = graph->getOrder();
    BOOST_CHECK( degree == 0 );
    BOOST_CHECK( order == 5 );
    
    
    {
        // adding a conection between vertex 0 and 1
        int expectedDegree = 1;
        int expectedOrder = 5;
        addVertexAndTestDegreeAndOrder( graph,0,1,expectedDegree,expectedOrder );
    
    }
    {
        int expectedDegree = 0;
        int expectedOrder = 5;
        int  vertexToDelete = 0;
        // removing the connection between 0 and 1
        removeVertexAndTestDegreeAndOrder(graph,vertexToDelete,expectedDegree,expectedOrder);
    }
    {
        int vertexToRemove = 1;
        // removing a removed connection
        graph->removeVertexNeighbours(vertexToRemove);
    
        degree = graph->getDegree();
        order = graph->getOrder();
        int expectedDegree = 0;
        int expectedOrder = 5;
        BOOST_CHECK( degree == expectedDegree );
        BOOST_CHECK( order == expectedOrder);
    }
    //removing a inexisting connection
    removeVertexAndTestDegreeAndOrder(graph,1,0,5);


    // adding a triangle
    graph->addNewVertexNeighbour( 0,1 );
    graph->addNewVertexNeighbour( 0,3 );
    addVertexAndTestDegreeAndOrder( graph,3,1,2,5 );
    
    // adding a new Vertex
    addVertexAndTestDegreeAndOrder( graph,5,0,3,6 );

}
