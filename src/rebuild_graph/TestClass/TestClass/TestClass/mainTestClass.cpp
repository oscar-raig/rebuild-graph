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
#include "graphs.h"

using namespace boost::unit_test;

/*
BOOST_AUTO_TEST_CASE(test_communicability_beetweeness_centrality){

	BOOST_CHECK(0 > 1);

}*/
void readGraphFromNULLFile(){
	graph * testGraph = new graph();
	BOOST_REQUIRE_THROW(testGraph->readPythonGraphFile(NULL), std::exception);
}

void readGraphFromFileNotExists(){
	graph * testGraph = new graph();
	BOOST_REQUIRE_THROW(testGraph->readPythonGraphFile("filenotfound"), std::exception);
}

test_suite*
init_unit_test_suite( int argc, char* argv[] )
{
    if(1)
        framework::master_test_suite().
		add( BOOST_TEST_CASE( &readGraphFromNULLFile ) );
	if(1)
        framework::master_test_suite().
		add( BOOST_TEST_CASE( &readGraphFromFileNotExists ) );
	
    return 0;
}