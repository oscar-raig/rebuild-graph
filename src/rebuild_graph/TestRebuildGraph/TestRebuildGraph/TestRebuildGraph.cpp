#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include "analitza.h"
#include <math.h>
#include "graphs.h"
#include <fstream>
#include <iterator>
#include "CSettingsSumulation.h"
#include "rebuildgraph.h"

BOOST_AUTO_TEST_CASE( Test_Beetweeness_exp)
{/*
	const char *largv[2]={"program_name","/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test_4nodes.gpfc"};
	largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test.gpfc";
	int graphOrder = 0;
	CSettingsSimulation *settingSimulation = new CSettingsSimulation() ;
	
	settingSimulation->inputFileName = largv[1];
	double compareResult = 0.0;
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	double *TargetBC = NULL;
	double *BestBC = NULL;
	//rebuildGraph->regenerateGraph(*settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
	
	//BOOST_CHECK(abs(compareResult-0.061915)< 0.1);
	
	 graphOrder = 0;
	 //largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc";
	 largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test.gpfc";
	 settingSimulation->inputFileName = largv[1];
	 settingSimulation->graphProperty = COMMUNICABILITY_BETWEENESS;
	 compareResult = 0.0;
	 TargetBC = NULL;
	 BestBC = NULL;
	 rebuildGraph->regenerateGraph(*settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
	printf("Compare Result %f",compareResult);
	 BOOST_CHECK(abs(compareResult-0.298288)< 0.1);
	 
	exit(1);
	*/
}



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
		
		const char *largv[2]={"program_name","/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test_4nodes.gpfc"};
		double *TargetBC = NULL;
		double *BestBC = NULL;
		int graphOrder = 0;
	CSettingsSimulation *settingSimulation = new CSettingsSimulation() ;
	settingSimulation->inputFileName =largv[1];
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	double compareResult = 0.0;
	rebuildGraph->regenerateGraph(*settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
		BOOST_CHECK( 4 == graphOrder);
		double ExpectedTargetBC[4]={0,0.66,0.66,0};
		double ExpectedBestBC[4]={0,0.66,0.66,0};
		for (int nVertex = 0;nVertex < graphOrder;nVertex++){
		
			std::cout << "TargetBC[" << nVertex << "]" << TargetBC[nVertex] << std::endl;
			std::cout << "BestBC[" << nVertex << "]" << BestBC[nVertex] << std::endl;
			
			BOOST_CHECK(fabs(TargetBC[nVertex] - ExpectedTargetBC[nVertex]) < 0.1 );
			BOOST_CHECK(fabs(BestBC[nVertex] - ExpectedBestBC[nVertex]) < 0.1 );
		}
		free(TargetBC);
	free(BestBC);
	free(settingSimulation);
	settingSimulation = new CSettingsSimulation() ;
	
		largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc";
		graphOrder = 0;
	settingSimulation->inputFileName = largv[1];
		compareResult = 0.0;
		rebuildGraph->regenerateGraph(*settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
		BOOST_CHECK( 20 == graphOrder);
		double expectedTargetBCBarabase[20]={0.0139492459,0.0000000000,0.0121514823,0.0099333301,0.2414517944,
			0.1100452867,	0.1536684219, 0.0866688151,	0.0177944862, 0.0201197438,
			0.0278154537,	0.0564327485,0.0212409795,0.0136259452,0.0165429915,
			0.0048732943,0.0096798368,0.0143987105,0.0103940407,0.0013186561};
	
		double expectedBestBCBarabase[20]={0.0114267149,0.0095261301,0.0000000000, 0.0123688852, 0.2423887879,
			0.1131020634,0.1553697697, 0.0742358347, 0.0212614871,  0.0218641648,
			0.0289211591,  0.0519398988, 0.0234236190, 0.0079434698, 0.0007309942,
			 0.0102339181,  0.0094193818,	 0.0135186823,0.0139920881, 0.0028943546};

	
		for (int nVertex = 0;nVertex < graphOrder;nVertex++){
			
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


BOOST_AUTO_TEST_CASE(compara){
	
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	gsl_matrix * matrixA;
	gsl_matrix * matrixB;
	graph* graph = NULL;
	
	const char *largv[2]={"program_name","/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test_4nodes.gpfc"};
	graph = rebuildGraph->GetGraphfromFile(largv);
	
	matrixA = gsl_matrix_alloc(graph->getOrder(),graph->getOrder());
	matrixB = gsl_matrix_alloc(graph->getOrder(),graph->getOrder());
	
	rebuildGraph->graphToGsl(graph, matrixA );
	rebuildGraph->graphToGsl(graph,matrixB);
	
	
	float result = rebuildGraph->compareMatrix(matrixA,matrixB);
	BOOST_CHECK( abs(result)  < 0.00001);

	
}

BOOST_AUTO_TEST_CASE(regenerate_compare){
	const char *largv[2]={"program_name","/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test_4nodes.gpfc"};
	largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc";
	int graphOrder = 0;
	CSettingsSimulation *settingSimulation = new CSettingsSimulation() ;
	settingSimulation->inputFileName = largv[1];
	double compareResult = 0.0;
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	double *TargetBC = NULL;
	double *BestBC = NULL;
	rebuildGraph->regenerateGraph(*settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
	
	BOOST_CHECK(abs(compareResult-0.061915)< 0.1);
	
	graphOrder = 0;
	//largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc";
	largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc";
	settingSimulation->inputFileName = largv[1];
	settingSimulation->graphProperty = COMMUNICABILITY_BETWEENESS;
	compareResult = 0.0;
	TargetBC = NULL;
	BestBC = NULL;
	rebuildGraph->regenerateGraph(*settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
	printf("Compare Result %f",compareResult);
	BOOST_CHECK(abs(compareResult-0.051123)< 0.1);

	 }
