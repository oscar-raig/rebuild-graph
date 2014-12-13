#define BOOST_TEST_MODULE TestingRebuildGraph
#include <boost/test/unit_test.hpp>
#include "analitza.h"
#include <math.h>
#include "graphs.h"
#include <fstream>
#include <iterator>
#include "CSettingsSumulation.h"
#include "rebuildgraph.h"
#include "AbstractFactoryPatternRebuildGraph.h"

using namespace boost::unit_test;


// Record 11/12 GRAPH
// Record 11/2 GSL_GRAPH

#define THROW_EXCEPTION
#define THROW_EXCEPTION_FILE_NOT_FOUND

#define COMMUNICABILITY_BETWEENESS_TEST_4_NODES
#define COMMUNICABILITY_BETWEENESS__WHEEL4
// 2014-12-09 GRAPH
//#define COMMUNICABILITY_BETWEENESS__WHEEL10

// 2014-11-09 GSL_GRAPH CB WHEEL14 NOT WORK
//#define COMMUNICABILITY_BETWEENESS_WHEEL_14
#define COMMUNICABILITY_BETWEENESS__BARBASE_20_4
#define COMMUNICABILITY_BETWEENESS__PETERSEN


#define BETWEENNESS_CENTRALITY__WHEEL4
// 2014-11-11 GRAPH BETWEENESS CENTRALITY WHEEL 10
#define BETWEENNESS_CENTRALITY__WHEEL10
       // 2014-11-09 GSL_GRAPH BC WHEEL14 NOT WORK
//#define BETWEENNESS_CENTRALITY__WHEEL14

// 2014-11-09 GRAPH/GSL_GRAPH BC PETERSON NOT WORK
#define BETWEENNESS_CENTRALITY__PETERSEN



// 2014-11-11 GRAPH DEFAULT_ALGORITHM_BARBASE_20_4
// -- WORKING --> TOO SLOW
//#define DEFAULT_ALGORITHM_BARBASE_20_4
#define COMPARA_THE_SAME_GRAPH



#define TEST_BEEWEENESS_EXP
#define COMPARA
#define TEST_ANALIZA
// 2014-11-11 GRAPH
//#define DEFAULT_GRAPH_TEST_TNODES


void testexceptionSettingSimulationIsNULL(){
	CFuncTrace( false,"testexceptionSettingSimulationIsNULL");
	
	//throw std::exception();
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	int graphOrder = 0;
	CSettingsSimulation *settingSimulation = NULL;
	double *TargetBC = NULL;
	double *BestBC = NULL;
	double compareResult = 10.0;
	rebuildGraph->regenerateGraph(settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
}

#ifdef THROW_EXCEPTION
BOOST_AUTO_TEST_CASE(Trow_excpetion){
	BOOST_REQUIRE_THROW(testexceptionSettingSimulationIsNULL(), std::exception);
	
}
#endif

void testexceptionFileNotFound(){
	
	CFuncTrace( false,"testexceptionFileNotFound");
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	int graphOrder = 0;
	CSettingsSimulation *settingSimulation = new CSettingsSimulation() ;
	settingSimulation->inputFileName = "filenotfound.txt";
	double *TargetBC = NULL;
	double *BestBC = NULL;
	double compareResult = 10.0;
	rebuildGraph->regenerateGraph(settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
}



#ifdef THROW_EXCEPTION_FILE_NOT_FOUND
BOOST_AUTO_TEST_CASE(Trow_excpetion_file_not_found){
	BOOST_REQUIRE_THROW(testexceptionFileNotFound(), std::exception);
	
}
#endif


void simulation( int algorithm, std::string GraphName, int nMax, double * compareResult){
	
    AbstractFactoryPatternRebuildGraph * FactoryTest= new	AbstractFactoryPatternRebuildGraph(algorithm,GraphName,nMax,compareResult);
	BOOST_CHECK(abs(*compareResult-0.000001)< 0.001);

	delete FactoryTest;
}

#ifdef COMMUNICABILITY_BETWEENESS_TEST_4_NODES
BOOST_AUTO_TEST_CASE(Test_communicability_beetweeness_centrality_test_4nodes){
	CFuncTrace trace (true,"test_communicability_beetweeness_centrality_test_4nodes");
	trace.trace(CTrace::level::TRACE_DEBUG,"begin");
	double compareResult = 10.0;
	// 2014-11-09 compareResult == 0.29804, very different 0.000001 GSL_GRAPH
	simulation(BETWEENNESS_CENTRALITY, "test_4nodes.gpfc" ,1000,&compareResult);
	
	trace.trace(CTrace::level::TRACE_INFO,"test_4nodes.gpfc %f",compareResult);
	std::cout << "COMMUNICABILITY_BETWEENESS_CENTRALITY test_4nodes.gpfc " << compareResult << " <<" << std::endl;
	
}
#endif

#ifdef COMMUNICABILITY_BETWEENESS__WHEEL4
BOOST_AUTO_TEST_CASE(Test_wheel4_COMMUNICABILITY_BETWEENESS){
	std::cout << " COMMUNICABILITY BETWEENESS with wheel 4 >>"  << std::endl;
	double compareResult = 10.0;
	simulation(COMMUNICABILITY_BETWEENESS, "wheel4.txt" ,100, &compareResult);
	std::cout << "COMMUNICABILITY BETWEENESS wheel 4 Result:" << compareResult << " <<" << std::endl;
}
#endif



#ifdef COMMUNICABILITY_BETWEENESS__WHEEL10
BOOST_AUTO_TEST_CASE(Test_wheel10_COMMUNICABILITY_BETWEENESS){
	
	std::cout << " COMMUNICABILITY BETWEENESS with wheel 10 >>" << std::endl;
	double compareResult = 10.0;
	simulation(COMMUNICABILITY_BETWEENESS, "wheel10.txt" ,100,&compareResult);
	std::cout << "COMMUNICABILITY BETWEENESS wheel 10 Result:" << compareResult << " <<" << std::endl;
}
#endif


#ifdef COMMUNICABILITY_BETWEENESS_WHEEL_14
BOOST_AUTO_TEST_CASE(test_wheel14_COMMUNICABILITY_BETWEENESS){
	
	std::cout << " COMMUNICABILITY BETWEENESS with wheel 14 >>" << std::endl;
		
	double compareResult = 10.0;
	simulation(COMMUNICABILITY_BETWEENESS, "wheel14.txt" ,1000000,&compareResult);

	std::cout << "COMMUNICABILITY BETWEENESS wheel 14 Result:" << compareResult << " <<" << std::endl;
	
}
#endif

#ifdef COMMUNICABILITY_BETWEENESS_BARBASE_20_4
BOOST_AUTO_TEST_CASE(regenerate_compare_barbase_20_4_COMMUNICABILITY_BETWEENESS){
	
	std::cout << "COMMUNICABILITY_BETWEENESS barabase_20_4.gpfc>>" << std::endl;
	double compareResult = 10.0;
	simulation(COMMUNICABILITY_BETWEENESS, "barabase_20_4.gpfc" ,10,&compareResult);
	std::cout << "COMMUNICABILITY_BETWEENESS barabase_20_4.gpfcResult:" << compareResult << " <<" << std::endl;
	BOOST_REQUIRE(abs(compareResult-0.051123)< 0.1);
	
}
#endif

#ifdef COMMUNNICABILITY_BETWEENESS_PETERSEN
BOOST_AUTO_TEST_CASE(test_petersen_COMMUNICABILITY_BETWEENESS){
	
	std::cout << " COMMUNICABILITY BETWEENESS with petersen >>" << std::endl;
	double compareResult = 10.0;
	simulation(COMMUNICABILITY_BETWEENESS, "petersen.txt" ,100000,&compareResult);
	std::cout << "COMMUNICABILITY BETWEENESS petersen Result:" << compareResult << " <<" << std::endl;
}
#endif



#ifdef BETWEENNESS_CENTRALITY__WHEEL4
BOOST_AUTO_TEST_CASE(Test_wheel4_BETWEENNESS_CENTRALITY){
	std::cout << " BETWEENNESS CENTRALITY with wheel 4>>" << std::endl;
	double compareResult = 10.0;
	simulation(BETWEENNESS_CENTRALITY, "wheel4.txt" ,100,&compareResult);
	std::cout << "BETWEENNESS CENTRALITY with wheel 4 " << compareResult << " <<" << std::endl;
}
#endif



#ifdef BETWEENNESS_CENTRALITY__WHEEL10
BOOST_AUTO_TEST_CASE(test_wheel10_BETWEENNESS_CENTRALITY){
	
	std::cout << " BETWEENNESS CENTRALITY with wheel 10>>" << std::endl;
	double compareResult = 10.0;
	simulation(BETWEENNESS_CENTRALITY, "wheel10.txt" ,10000,&compareResult);
	std::cout << "BETWEENESS CENTRALITY wheel 10 Result:" << compareResult << " <<" << std::endl;
	
}
#endif



#ifdef BETWEENNESS_CENTRALITY__WHEEL14
BOOST_AUTO_TEST_CASE(test_wheel14_BETWEENNESS_CENTRALITY){
	
	std::cout << " BETWEENNESS CENTRALITY with wheel 14>>" << std::endl;
	double compareResult = 10.0;
	simulation(BETWEENNESS_CENTRALITY, "wheel14.txt" ,1000000,&compareResult);
	std::cout << "BETWEENESS CENTRALITY wheel 14 Result:" << compareResult << " <<" << std::endl;
}
#endif


#ifdef BETWEENNESS_CENTRALITY__PETERSEN
BOOST_AUTO_TEST_CASE(test_petersen_BETWEENNESS_CENTRALITY){
	
	
	std::cout << "BETWEENNESS_CENTRALITY__PETERSEN>>" << std::endl;
	double compareResult = 10.0;
	simulation(BETWEENNESS_CENTRALITY, "petersen.txt" ,100000,&compareResult);
	std::cout << "BETWEENNESS_CENTRALITY__PETERSEN " << compareResult << " <<" << std::endl;
	
}
#endif




#ifdef TEST_BEEWEENESS_EXP

BOOST_AUTO_TEST_CASE( Test_Beetweeness_exp)
{
	const char *largv[2]={"program_name","/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test_4nodes.gpfc"};
	largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test.gpfc";
	int graphOrder = 0;
	CSettingsSimulation *settingSimulation = new CSettingsSimulation() ;
	
	settingSimulation->inputFileName = largv[1];
	settingSimulation->setNMax(100);
	settingSimulation->setMaxIterations(100);
	double compareResult = 0.0;
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	double *TargetBC = NULL;
	double *BestBC = NULL;
	rebuildGraph->regenerateGraph(settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
	
	BOOST_REQUIRE(abs(compareResult-0.061915)< 0.1);
	
	 graphOrder = 0;
	 //largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/barabase_20_4.gpfc";
	 largv[1]="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test.gpfc";
	 settingSimulation->inputFileName = largv[1];
	 settingSimulation->graphProperty = COMMUNICABILITY_BETWEENESS;
	 compareResult = 0.0;
	 TargetBC = NULL;
	 BestBC = NULL;
	 rebuildGraph->regenerateGraph(settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
	printf("Compare Result %f",compareResult);
	// BOOST_CHECK(abs(compareResult-0.298288)< 0.1);
	BOOST_REQUIRE(abs(compareResult-0.051123<0.1));
	
	
}
#endif


#ifdef  TEST_ANALIZA

BOOST_AUTO_TEST_CASE( Test_analitza){
	CFuncTrace trace (true,"Test_analitza");
	
	/* Begin analitza::llegir_dades -------------------------------------------*/
	
	/* If the function cannot open the file
	* an error -1 should be returned
	*/
	
	int order,maxdegree,mindegree;
	float avaragedegree;
	int res_llegir =llegir_dades("test",order,maxdegree,mindegree,avaragedegree);
	BOOST_REQUIRE( res_llegir == -1 );

	const char *szFileGraph = "/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test.gpfc";
	res_llegir =llegir_dades(szFileGraph,order,maxdegree,mindegree,avaragedegree);
	BOOST_REQUIRE( res_llegir != -1 );
    // In this example order:7, maxim grau 3, minim grau 1, grau mitja 2.13
	BOOST_REQUIRE(order == 7);
	BOOST_REQUIRE(maxdegree == 3);
	BOOST_REQUIRE(mindegree == 1);
	BOOST_REQUIRE(fabs(avaragedegree - 2.142857) < 0.1 );
	
	/* End analitza::llegir_dades -------------------------------------------*/

	/* Begin analitza::clustering -------------------------------------------*/

	float res_expected_clusteringresults[7]=
	{1,0.66,1,0.66,1,0,0};
	float res_clustering = 0.0;
	
	for (int nVertex = 0;nVertex < order;nVertex++){
		res_clustering = clustering(nVertex);
		std::cout << res_clustering << std::endl;
		BOOST_REQUIRE(fabs(res_expected_clusteringresults[nVertex] - res_clustering) < 0.1 );

	}
	/* End analitza::llegir_dades -------------------------------------------*/

}
#endif


#ifdef DEFAULT_GRAPH_TEST_TNODES
BOOST_AUTO_TEST_CASE(graph_betweness_centrality){
		CFuncTrace trace (true,"graph_betweness_centrality");
		const char *largv[2]={"program_name","/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test_4nodes.gpfc"};
		double *TargetBC = NULL;
		double *BestBC = NULL;
		int graphOrder = 0;
	CSettingsSimulation *settingSimulation = new CSettingsSimulation() ;
	settingSimulation->inputFileName =largv[1];
	settingSimulation->setNMax(100);
	settingSimulation->setMaxIterations(100);
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	double compareResult = 0.0;
	rebuildGraph->regenerateGraph(settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
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
		rebuildGraph->regenerateGraph(settingSimulation,TargetBC,BestBC,graphOrder,compareResult);
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
#endif




#ifdef DEFAULT_ALGORITHM_BARBASE_20_4
BOOST_AUTO_TEST_CASE(regenerate_compare_default_algorithm){
	
	std::cout << " DEFAULT ALGORITHM with barabase_20_4.gpfc>>" << std::endl;
	double compareResult = 10.0;
	simulation(0, "barabase_20_4.gpfc" ,1000000,&compareResult);
	std::cout << "DEFAULT ALGORITHM barabase_20_4.gpfc Result:" << compareResult << " <<" << std::endl;
	
	BOOST_CHECK(abs(compareResult-0.0467452)< 0.1);
}
#endif

#ifdef COMPARA_THE_SAME_GRAPH
BOOST_AUTO_TEST_CASE(compara){
	CFuncTrace trace (true,"COMPARA_THE_SAME_GRAPH");
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	gsl_matrix * matrixA;
	gsl_matrix * matrixB;
	GeneralGraph* graph = NULL;
	
	const char *fileNameGraph="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test_4nodes.gpfc";
	graph = rebuildGraph->GetGraphfromFile(fileNameGraph);
	
	matrixA = gsl_matrix_alloc(graph->getOrder(),graph->getOrder());
	matrixB = gsl_matrix_alloc(graph->getOrder(),graph->getOrder());
	
	graph->graphToGsl( matrixA );
	graph->graphToGsl( matrixB );
	
	
	float result = rebuildGraph->compareMatrix(matrixA,matrixB);
	BOOST_CHECK( abs(result)  < 0.00001);
	
	delete rebuildGraph;
	delete matrixA;
	delete matrixB;
	
}
#endif


BOOST_AUTO_TEST_CASE(test_generateInitialGraph){
	CFuncTrace trace (true,"test_generateInitialGraph");
	int random_value_x=11,random_value_y=92,random_value_z=37;
	GeneralGraph *  generalGraph =
	StrategyPatternAlgorithm::generateInitialGraph(4,
												   random_value_x,random_value_y,random_value_z);
	
	int degree = generalGraph->getDegree();
	int order = generalGraph->getOrder();
	trace.trace(CTrace::TRACE_INFO," Graph degree %d order %d",degree,order);

	generalGraph->printGraph(CTrace::level::TRACE_INFO);
	BOOST_CHECK(degree == 3); // FAILED IN GRAPH
	BOOST_CHECK(order ==4);
	StrategyPatternAlgorithm::modifyGraph(generalGraph,
												   random_value_x,random_value_y,random_value_z);
	generalGraph->printGraph();
	degree = generalGraph->getDegree();
	order = generalGraph->getOrder();
	trace.trace(CTrace::TRACE_INFO," Graph degree %d order %d",degree,order);
	BOOST_CHECK(degree == 3);
	BOOST_CHECK(order ==4);
}

#ifdef lol
BOOST_AUTO_TEST_CASE(test_generateInitialGraph_order_10_){
	CFuncTrace trace (true,"test_generateInitialGraph");
	int random_value_x=11,random_value_y=92,random_value_z=37;
	int c_random_value_x=11,c_random_value_y=92,c_random_value_z=37;
	
	GeneralGraph *  generalGraph =
	StrategyPatternAlgorithm::generateInitialGraph(10,
												   random_value_x,random_value_y,random_value_z);
	
	int degree = generalGraph->getDegree();
	int order = generalGraph->getOrder();
	trace.trace(CTrace::TRACE_INFO," Graph degree %d order %d",degree,order);
	
	generalGraph->printGraph(CTrace::level::TRACE_INFO);
	BOOST_CHECK(degree == 3);
	BOOST_CHECK(order ==10);
	GeneralGraph *copyGraph = NULL;
	
	gsl_matrix * matrixA;
	gsl_matrix * matrixB;
	CRebuildGraph *rebuildGraph = NULL;
	float result =0 ;
	
	matrixA = gsl_matrix_alloc(generalGraph->getOrder(),generalGraph->getOrder());
	matrixB = gsl_matrix_alloc(generalGraph->getOrder(),generalGraph->getOrder());
	copyGraph = generalGraph->copyGraph();
	copyGraph->setAllVertexNeighbours();
	copyGraph->graphToGsl( matrixA );
	generalGraph->graphToGsl( matrixB );
	
	rebuildGraph = new CRebuildGraph();
	
	result = rebuildGraph->compareMatrix(matrixA,matrixB);
	BOOST_CHECK( abs(result)  < 0.00001);

	
	copyGraph->printGraph();
	generalGraph->printGraph();
	
	c_random_value_x = random_value_x;
	c_random_value_y = random_value_y;
	c_random_value_z = random_value_z;
	StrategyPatternAlgorithm::modifyGraph(copyGraph,
										  random_value_x,random_value_y,random_value_z);
	double newBC [10];
	double newBC2 [10];
	copyGraph->brandes_comunicability_centrality_exp(newBC);
	StrategyPatternAlgorithm::modifyGraph(generalGraph,
										  c_random_value_x,c_random_value_y,c_random_value_z);
	generalGraph->brandes_comunicability_centrality_exp(newBC2);
	
	for ( int j =0 ; j < 10; j++){
		BOOST_CHECK( newBC[j] == newBC2[j]);
		trace.trace(CTrace::TRACE_INFO,"NEWBC %f",newBC[j]);
		
	}
	BOOST_CHECK(19.563079 - newBC[0] < 0.00001);
	BOOST_CHECK(27.479907 - newBC[1] < 0.00001);
	BOOST_CHECK(34.327844 - newBC[2] < 0.00001);
	BOOST_CHECK(19.399584 - newBC[3] < 0.00001);
	BOOST_CHECK(2.068966 - newBC[4] < 0.00001);
	BOOST_CHECK(20.973092 - newBC[5] < 0.00001);
	BOOST_CHECK(50.668183 - newBC[6] < 0.00001);
	BOOST_CHECK(41.005052 - newBC[7] < 0.00001);
	BOOST_CHECK(24.364700 - newBC[8] < 0.00001);
	BOOST_CHECK(19.346583093959595 - newBC[9] < 0.00001);
	
	
	matrixA = gsl_matrix_alloc(generalGraph->getOrder(),generalGraph->getOrder());
	matrixB = gsl_matrix_alloc(generalGraph->getOrder(),generalGraph->getOrder());
	
	copyGraph->graphToGsl( matrixA );
	generalGraph->graphToGsl( matrixB );
	
	rebuildGraph = new CRebuildGraph();

	result = rebuildGraph->compareMatrix(matrixA,matrixB);
	BOOST_CHECK( abs(result)  < 0.00001);

	
	generalGraph->printGraph();
	degree = generalGraph->getDegree();
	order = generalGraph->getOrder();
	trace.trace(CTrace::TRACE_INFO," Graph degree %d order %d",degree,order);
	BOOST_CHECK(degree == 3);
	BOOST_CHECK(order ==10);
}
#endif

BOOST_AUTO_TEST_CASE(test_1){
	CFuncTrace trace (true,"test_1");
	int random_value_x=11,random_value_y=92,random_value_z=37;
	double newBC [10];
	
	GeneralGraph *  generalGraph =
	StrategyPatternAlgorithm::generateInitialGraph(10,
												   random_value_x,random_value_y,random_value_z);
	generalGraph->brandes_comunicability_centrality_exp(newBC);
	
	for ( int j =0 ; j < 10; j++){
		trace.trace(CTrace::TRACE_INFO,"NEWBC %f",newBC[j]);
		
	}
	BOOST_CHECK(abs(19.780136 - newBC[0]) < 0.00001);
	BOOST_CHECK(abs(37.142999 - newBC[1]) < 0.00001);
	BOOST_CHECK(abs(27.733060 - newBC[2] )< 0.00001);
	BOOST_CHECK(abs(18.014479 - newBC[3]) < 0.00001);
	BOOST_CHECK(abs(2.071303 - newBC[4]) < 0.00001);
	BOOST_CHECK(abs(20.609464 - newBC[5]) < 0.00001);
	BOOST_CHECK(abs(53.250202 - newBC[6] )< 0.00001);
	BOOST_CHECK(abs(33.859518 - newBC[7]) < 0.00001);
	BOOST_CHECK(abs(34.453578 - newBC[8]) < 0.00001);
	BOOST_CHECK(abs(28.560175 - newBC[9]) < 0.00001);
	
	GeneralGraph *newGraph=NULL;
	newGraph=generalGraph->copyGraph();
	newGraph->setAllVertexNeighbours();
	
	//
	// 9-12-2014
	// Si la brandes_comuncbiliy es diferent es perque els dos grafs son diferents, en alguna cosa
	//
	StrategyPatternAlgorithm::modifyGraph(newGraph,random_value_x,random_value_y,random_value_z);
	// en aqest p
    newGraph->brandes_comunicability_centrality_exp(newBC);
	for ( int j =0 ; j < 10; j++){
		trace.trace(CTrace::TRACE_INFO,"NEWBC 2 %f",newBC[j]);
		
	}
	BOOST_CHECK(abs(20.251314 - newBC[0]) < 0.00001);
	BOOST_CHECK(abs(27.104859 - newBC[1]) < 0.00001);
	BOOST_CHECK(abs(32.967459 - newBC[2] )< 0.00001);
	BOOST_CHECK(abs(19.221320 - newBC[3]) < 0.00001);
	BOOST_CHECK(abs(3.946619 - newBC[4]) < 0.00001);
	BOOST_CHECK(abs( 15.363911 - newBC[5]) < 0.00001);
	BOOST_CHECK(abs(49.258089 - newBC[6] )< 0.00001);
	BOOST_CHECK(abs(39.717238 - newBC[7]) < 0.00001);
	BOOST_CHECK(abs(24.218654 - newBC[8]) < 0.00001);
	BOOST_CHECK(abs(19.136546 - newBC[9]) < 0.00001);
	
}


BOOST_AUTO_TEST_CASE(test_2){
	CFuncTrace trace (true,"test_2");
	int random_value_x=11,random_value_y=92,random_value_z=37;
	double newBC [5];
	
	GeneralGraph *  generalGraph =
	StrategyPatternAlgorithm::generateInitialGraph(5,
												   random_value_x,random_value_y,random_value_z);
	generalGraph->brandes_comunicability_centrality_exp(newBC);
	
	for ( int j =0 ; j < 5; j++){
		trace.trace(CTrace::TRACE_INFO,"NEWBC %f",newBC[j]);
		trace.trace(CTrace::TRACE_INFO,"BOOST_CHECK(abs( %f - newBC[0]) < 0.00001);",newBC[j]);
		
	}
	/*
	BOOST_CHECK(abs(2.782980 - newBC[0]) < 0.00001);
	BOOST_CHECK(abs(2.782980 - newBC[1]) < 0.00001);
	BOOST_CHECK(abs(3.492149 - newBC[2] )< 0.00001);
	BOOST_CHECK(abs(1.661114 - newBC[3]) < 0.00001);*/

BOOST_CHECK(abs(7.239877 - newBC[0]) < 0.00001);
BOOST_CHECK(abs(5.154875 - newBC[1]) < 0.00001);
BOOST_CHECK(abs(5.154875 - newBC[2]) < 0.00001);
BOOST_CHECK(abs(5.154875 - newBC[3]) < 0.00001);
BOOST_CHECK(abs(5.154875 - newBC[4]) < 0.00001);
	
	GeneralGraph *newGraph=NULL;
	newGraph=generalGraph->copyGraph();
	newGraph->setAllVertexNeighbours();
	
	//
	// 9-12-2014
	// Si la brandes_comuncbiliy es diferent es perque els dos grafs son diferents, en alguna cosa
	//
	StrategyPatternAlgorithm::modifyGraph(newGraph,random_value_x,random_value_y,random_value_z);
	// en aqest p
	newGraph->brandes_comunicability_centrality_exp(newBC);
	for ( int j =0 ; j < 5; j++){
		trace.trace(CTrace::TRACE_INFO,"NEWBC 2 %f",newBC[j]);
		
	}
//	newGraph->printGraph();
	BOOST_CHECK(abs(7.472245 - newBC[0]) < 0.00001);
	BOOST_CHECK(abs(5.820453 - newBC[1]) < 0.00001);
	BOOST_CHECK(abs(7.472245 - newBC[2] )< 0.00001);
	BOOST_CHECK(abs(5.820453 - newBC[3]) < 0.00001);
	BOOST_CHECK(abs(3.521522 - newBC[4]) < 0.00001);
	
}


