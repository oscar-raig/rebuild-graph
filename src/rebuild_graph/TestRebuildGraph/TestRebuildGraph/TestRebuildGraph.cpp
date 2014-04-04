#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include "analitza.h"
#include <math.h>
#include "graphs.h"
#include <fstream>
#include <iterator>
int add( int i, int j ) { return i+j; }

BOOST_AUTO_TEST_CASE( Test_analitza){
	
	/* Begin analitza::llegir_dades -------------------------------------------*/
	
	/* If the function cannot open the file
	* an error -1 should be returned
	*/
	
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
	
	/* End analitza::llegir_dades -------------------------------------------*/

	/* Begin analitza::clustering -------------------------------------------*/

	float res_expected_clusteringresults[7]=
	{1,0.66,1,0.66,1,0,0};
	float res_clustering = 0.0;
	
	for (int nVertex = 0;nVertex < order;nVertex++){
		res_clustering = clustering(nVertex);
		std::cout << res_clustering << std::endl;
		BOOST_CHECK(fabs(res_expected_clusteringresults[nVertex] - res_clustering) < 0.1 );

	}
	/* End analitza::llegir_dades -------------------------------------------*/

}

BOOST_AUTO_TEST_CASE(graph_betweness_centrality){
		int largc = 2;
		const char *largv[2]={"program_name","/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test_4nodes.gpfc"};
		double *TargetBC = NULL;
		double *BestBC = NULL;
		int order = 0;
		fmain(largc,largv,&TargetBC,&BestBC,&order);
		BOOST_CHECK( 4 == order);
		double ExpectedTargetBC[4]={0,0.66,0.66,0};
		double ExpectedBestBC[4]={0,0.66,0.66,0};
		for (int nVertex = 0;nVertex < order;nVertex++){
		
			std::cout << "TargetBC[" << nVertex << "]" << TargetBC[nVertex] << std::endl;
			std::cout << "BestBC[" << nVertex << "]" << BestBC[nVertex] << std::endl;
			
			BOOST_CHECK(fabs(TargetBC[nVertex] - ExpectedTargetBC[nVertex]) < 0.1 );
			BOOST_CHECK(fabs(BestBC[nVertex] - ExpectedBestBC[nVertex]) < 0.1 );
		}
		free(TargetBC);
		free(BestBC);
		largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc";
		order = 0;
		fmain(largc,largv,&TargetBC,&BestBC,&order);
		BOOST_CHECK( 20 == order);
		double expectedTargetBCBarabase[20]={0.0139492459,0.0000000000,0.0121514823,0.0099333301,0.2414517944,
			0.1100452867,	0.1536684219, 0.0866688151,	0.0177944862, 0.0201197438,
			0.0278154537,	0.0564327485,0.0212409795,0.0136259452,0.0165429915,
			0.0048732943,0.0096798368,0.0143987105,0.0103940407,0.0013186561};
	
		double expectedBestBCBarabase[20]={0.0114267149,0.0095261301,0.0000000000, 0.0123688852, 0.2423887879,
			0.1131020634,0.1553697697, 0.0742358347, 0.0212614871,  0.0218641648,
			0.0289211591,  0.0519398988, 0.0234236190, 0.0079434698, 0.0007309942,
			 0.0102339181,  0.0094193818,	 0.0135186823,0.0139920881, 0.0028943546};

	
		for (int nVertex = 0;nVertex < order;nVertex++){
			
//			std::cout << "TargetBC[" << nVertex << "]" << TargetBC[nVertex] << std::endl;
//			std::cout << "BestBC[" << nVertex << "]" << BestBC[nVertex] << std::endl;
			
			BOOST_CHECK(fabs(TargetBC[nVertex] - expectedTargetBCBarabase[nVertex]) < 0.1 );
			BOOST_CHECK(fabs(BestBC[nVertex] - expectedBestBCBarabase[nVertex]) < 0.1 );
		}
		free(TargetBC);
		free(BestBC);
	
	/* Testing files results for barabasi out*/
	 std::ifstream ifs1("/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc.out");
	 std::ifstream ifs2("/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc.out.expected");
	 
	 std::istream_iterator<char> b1(ifs1), e1;
	 std::istream_iterator<char> b2(ifs2), e2;
	 
	 BOOST_CHECK_EQUAL_COLLECTIONS(b1, e1, b2, e2);
	
	/* Testing files results for barabasi res*/
	std::ifstream ifs1res("/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc.res");
	std::ifstream ifs2res("/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc.res.expected");
	
	std::istream_iterator<char> b1res(ifs1res), e1res;
	std::istream_iterator<char> b2res(ifs2res), e2res;
	
	BOOST_CHECK_EQUAL_COLLECTIONS(b1res, e1res, b2res, e2res);

}

