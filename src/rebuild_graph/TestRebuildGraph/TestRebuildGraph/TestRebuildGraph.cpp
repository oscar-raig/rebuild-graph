#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include "analitza.h"
#include <math.h>

int add( int i, int j ) { return i+j; }

BOOST_AUTO_TEST_CASE( Test_analitza){
	
	/* If the function cannot open the file
	* an error -1 should be returned
	*/
	// int	llegir_dades(const char* nom, int &linies,int &maxveins,int &minveins, float &mitja )
	
	int order,maxdegree,mindegree;
	float avaragedegree;
	int res_llegir =llegir_dades("test",order,maxdegree,mindegree,avaragedegree);
	BOOST_CHECK( res_llegir == -1 );

	const char *szFileGraph = "/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test.gpfc";
	res_llegir =llegir_dades(szFileGraph,order,maxdegree,mindegree,avaragedegree);
	BOOST_CHECK( res_llegir != -1 );
    // In this example order:7, maxim grau 3, minim grau 1, grau mitja 2.13
	BOOST_CHECK(order == 7);
	BOOST_CHECK(maxdegree == 3);
	BOOST_CHECK(mindegree == 1);
	BOOST_CHECK(fabs(avaragedegree - 2.142857) < 0.1 );
	float res_expected_clusteringresults[7]=
	{1,0.66,1,0.66,1,0,0};
	float res_clustering = 0.0;
	
	for (int nVertex = 0;nVertex < order;nVertex++){
		res_clustering = clustering(nVertex);
		std::cout << res_clustering << std::endl;
		BOOST_CHECK(fabs(res_expected_clusteringresults[nVertex] - res_clustering) < 0.1 );

	}

	

}


BOOST_AUTO_TEST_CASE( my_test )
{
    // seven ways to detect and report the same error:
    BOOST_CHECK( add( 2,2 ) == 3 );        // #1 continues on error
	
    BOOST_REQUIRE( add( 2,2 ) == 4 );      // #2 throws on error
	
    if( add( 2,2 ) != 3 )
		BOOST_ERROR( "Ouch..." );            // #3 continues on error
	
    if( add( 2,2 ) != 4 )
		BOOST_FAIL( "Ouch..." );             // #4 throws on error
	
    if( add( 2,2 ) != 4 ) throw "Ouch..."; // #5 throws on error
	
    BOOST_CHECK_MESSAGE( add( 2,2 ) == 3,  // #6 continues on error
						"add(..) result: " << add( 2,2 ) );
	
    BOOST_CHECK_EQUAL( add( 2,2 ), 4 );	  // #7 continues on error
}

#include <boost/test/output_test_stream.hpp>
using boost::test_tools::output_test_stream;

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test )
{
    output_test_stream output( "pattern_file", true );
    int i=2;
    output << "i=" << i;
    BOOST_CHECK( output.match_pattern() );
	
    output << "\nFile: " << __FILE__ << " Line: " << __LINE__;
    BOOST_CHECK( output.match_pattern() );
}
