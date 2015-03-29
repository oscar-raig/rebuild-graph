#define BOOST_TEST_MODULE TestingRebuildGraph
#include <boost/test/unit_test.hpp>
#include "analitza.h"
#include <math.h>
#include <fstream>
#include <iterator>
#include "CSettingsSumulation.h"
#include "rebuildgraph.h"
#include "AbstractFactoryPatternRebuildGraph.h"
#include "graphIndicatorCommunicabilityCentralityUsingMatrixExponential.h"
#include <ctime>

using namespace boost::unit_test;

#define SLOW_TEST_ENABLED 1




#define THROW_EXCEPTION
#define THROW_EXCEPTION_FILE_NOT_FOUND

#define COMMUNICABILITY_BETWEENESS_TEST_4_NODES
#define COMMUNICABILITY_BETWEENESS__WHEEL4
// 2014-12-09 GRAPH
#define COMMUNICABILITY_BETWEENESS__WHEEL10


#define COMMUNICABILITY_BETWEENESS_WHEEL_14
#define COMMUNICABILITY_BETWEENESS__BARBASE_20_4
#define COMMUNICABILITY_BETWEENESS__PETERSEN


#define BETWEENNESS_CENTRALITY__WHEEL4
// 2014-11-11 GSL_GRAPH BETWEENESS CENTRALITY WHEEL 10
#define BETWEENNESS_CENTRALITY__WHEEL10
       // 2014-11-09 GSL_GRAPH BC WHEEL14 NOT WORK
#define BETWEENNESS_CENTRALITY__WHEEL14

// 2014-11-09 GRAPH/GSL_GRAPH BC PETERSON NOT WORK
//#define BETWEENNESS_CENTRALITY__PETERSEN



// 2014-11-11 GRAPH DEFAULT_ALGORITHM_BARBASE_20_4
// -- WORKING --> TOO SLOW
#define DEFAULT_ALGORITHM_BARBASE_20_4

// DELETE DUE NO GRAPH FROM FILE
#define COMPARA_THE_SAME_GRAPH



#define TEST_BEEWEENESS_EXP
#define COMPARA
#define TEST_ANALIZA
// 2014-11-11 GRAPH
#define DEFAULT_GRAPH_TEST_TNODES


void simulation( int algorithm, std::string GraphName, int nMax,
				double * compareResult,double valueToCompare = 0.000001, double To  = DEFAULT_TO, bool ThresHold = false);

void testexceptionSettingSimulationIsNULL(){
	CFuncTrace( false,"testexceptionSettingSimulationIsNULL");
	
	//throw std::exception();
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	int graphOrder = 0;
	CSettingsSimulation *settingSimulation = NULL;
	double *TargetBC = NULL;
	double *BestBC = NULL;
	double compareResult = 10.0;
	rebuildGraph->regenerateGraph(settingSimulation,BestBC,graphOrder,compareResult);
	
	// simulation(BETWEENNESS_CENTRALITY, "test_4nodes.gpfc" ,1000,&compareResult);

}

#ifdef THROW_EXCEPTION
BOOST_AUTO_TEST_CASE(UTest_Trow_excpetion){
	BOOST_REQUIRE_THROW(testexceptionSettingSimulationIsNULL(), std::exception);
	
}
#endif

void testexceptionFileNotFound(){
	
	CFuncTrace( false,"testexceptionFileNotFound");
	double compareResult = 10.0;
	simulation(BETWEENNESS_CENTRALITY, "filenotfound.txt" ,1000,&compareResult);

}



#ifdef THROW_EXCEPTION_FILE_NOT_FOUND
BOOST_AUTO_TEST_CASE(UTest_Trow_excpetion_file_not_found){
	BOOST_REQUIRE_THROW(testexceptionFileNotFound(), std::exception);
	
}
#endif


void simulation( int algorithm, std::string GraphName,
				int nMax, double * compareResult,double valueToCompare, double To, bool ThresHoldAccepting){
	
    AbstractFactoryPatternRebuildGraph * FactoryTest= new	AbstractFactoryPatternRebuildGraph(algorithm,GraphName,nMax,compareResult,To,ThresHoldAccepting);
	BOOST_CHECK(abs(*compareResult-valueToCompare)< 0.001);

	delete FactoryTest;
}


BOOST_AUTO_TEST_CASE(UTest_beetweeness_centrality_test_4nodes){
	CFuncTrace trace (true,"test_beetweeness_centrality_test_4nodes");
	trace.trace(CTrace::TRACE_DEBUG,"begin");
	double compareResult = 10.0;

	simulation(BETWEENNESS_CENTRALITY, "test_4nodes.gpfc" ,1000,&compareResult);
	
	trace.trace(CTrace::TRACE_INFO,"test_4nodes.gpfc %f",compareResult);
	std::cout << "BETWEENNESS_CENTRALITY test_4nodes.gpfc " << compareResult << " <<" << std::endl;
	
}



BOOST_AUTO_TEST_CASE(UTest_beetweeness_centrality_test_4nodes_thresholdAcceptance){
	CFuncTrace trace (true,"test_beetweeness_centrality_test_4nodes_thresholdAcceptance");
	trace.trace(CTrace::TRACE_DEBUG,"begin");
	double compareResult = 10.0;

	simulation(BETWEENNESS_CENTRALITY, "test_4nodes.gpfc" ,10000,&compareResult,0,1,true);
	
	trace.trace(CTrace::TRACE_INFO,"test_4nodes.gpfc %f",compareResult);
	std::cout << "BETWEENESS_CENTRALITY test_4nodes.gpfc thresholdAcceptance " << compareResult << " <<" << std::endl;
	
}



#ifdef COMMUNICABILITY_BETWEENESS__WHEEL4
BOOST_AUTO_TEST_CASE(UTest_wheel4_COMMUNICABILITY_BETWEENESS){
	std::cout << " COMMUNICABILITY BETWEENESS with wheel 4 >>"  << std::endl;
	double compareResult = 10.0;
	simulation(COMMUNICABILITY_CENTRALITY, "wheel4.txt" ,100, &compareResult);
	std::cout << "COMMUNICABILITY BETWEENESS wheel 4 Result:" << compareResult << " <<" << std::endl;
}
#endif



#ifdef COMMUNICABILITY_BETWEENESS__WHEEL10
BOOST_AUTO_TEST_CASE(Test_wheel10_COMMUNICABILITY_BETWEENESS){
	
	std::cout << " COMMUNICABILITY BETWEENESS with wheel 10 >>" << std::endl;
	double compareResult = 10.0;
	simulation(COMMUNICABILITY_CENTRALITY, "wheel10.txt" ,100,&compareResult);
	std::cout << "COMMUNICABILITY BETWEENESS wheel 10 Result:" << compareResult << " <<" << std::endl;
}
#endif


#ifdef COMMUNICABILITY_BETWEENESS_WHEEL_14
BOOST_AUTO_TEST_CASE(UTest_wheel14_COMMUNICABILITY_BETWEENESS){
	
	std::cout << " COMMUNICABILITY BETWEENESS with wheel 14 >>" << std::endl;
		
	double compareResult = 10.0;
	simulation(COMMUNICABILITY_CENTRALITY, "wheel14.txt" ,1000000,&compareResult);

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
BOOST_AUTO_TEST_CASE(UTest_wheel4_BETWEENNESS_CENTRALITY){
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



#if defined( BETWEENNESS_CENTRALITY__WHEEL14 ) && defined(SLOW_TEST_ENABLED)
BOOST_AUTO_TEST_CASE(test_wheel14_BETWEENNESS_CENTRALITY){
	
	std::cout  << std::endl << " BETWEENNESS CENTRALITY with wheel 14>>" << std::endl;
	double compareResult = 10;
	simulation(BETWEENNESS_CENTRALITY, "wheel14.txt" ,1000,&compareResult,0.0458888);
	std::cout << "BETWEENESS CENTRALITY wheel 14 Result:" << compareResult << " <<" << std::endl;
}
#endif


#ifdef BETWEENNESS_CENTRALITY__PETERSEN
BOOST_AUTO_TEST_CASE(test_petersen_BETWEENNESS_CENTRALITY){
	
	
	std::cout << "BETWEENNESS_CENTRALITY__PETERSEN>>" << std::endl;
	double compareResult = 10.0;
	simulation(BETWEENNESS_CENTRALITY, "petersen.txt" ,10,&compareResult);
	std::cout << "BETWEENNESS_CENTRALITY__PETERSEN " << compareResult << " <<" << std::endl;
	
}
#endif




#ifdef TEST_BEEWEENESS_EXP

BOOST_AUTO_TEST_CASE( UTest_Betweeness_centraliyt_4_nodes_and_communicabilt_betweeness_test)
{
	CFuncTrace trace (true,"UTest_Betweeness_centraliyt_4_nodes_and_communicabilt_betweeness_test");
	double compareResult = 0.0;
	simulation(BETWEENNESS_CENTRALITY, "test_4nodes.gpfc" ,100,&compareResult);
	
	
	simulation(COMMUNICABILITY_CENTRALITY, "test.gpfc" ,1000,&compareResult);
	std::cout << "BETWEENNESS_CENTRALITY " << compareResult << " <<" << std::endl;
}
#endif


#ifdef  TEST_ANALIZA

BOOST_AUTO_TEST_CASE( UTest_analitza){
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

#if defined(SLOW_TEST_ENABLED)

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
	rebuildGraph->regenerateGraph(settingSimulation,BestBC,graphOrder,compareResult);
		BOOST_CHECK( 4 == graphOrder);
		double ExpectedBestBC[4]={0,0.66,0.66,0};
		for (int nVertex = 0;nVertex < graphOrder;nVertex++){
		
			std::cout << "BestBC[" << nVertex << "]" << BestBC[nVertex] << std::endl;
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
		rebuildGraph->regenerateGraph(settingSimulation,BestBC,graphOrder,compareResult);
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
			
			BOOST_CHECK(fabs(BestBC[nVertex] - expectedBestBCBarabase[nVertex]) < 0.1 );
		}
		free(TargetBC);
		free(BestBC);
	
	
}

#endif

#if defined(SLOW_TEST_ENABLED)

BOOST_AUTO_TEST_CASE(regenerate_compare_default_algorithm){
	
	std::cout << " DEFAULT ALGORITHM with barabase_20_4.gpfc>>" << std::endl;
	double compareResult = 10.0;
	simulation(0, "barabase_20_4.gpfc" ,1000,&compareResult,0.0764033);
	std::cout << "DEFAULT ALGORITHM barabase_20_4.gpfc Result:" << compareResult << " <<" << std::endl;
	
	BOOST_CHECK(abs(compareResult-0.0764033)< 0.1);
}
#endif

#if defined(SLOW_TEST_ENABLED)
BOOST_AUTO_TEST_CASE(regenerate_compare_default_algorithm_threshold){
	
	std::cout << " DEFAULT ALGORITHM with barabase_20_4.gpfc threshold>>" << std::endl;
	double compareResult = 10.0;
	simulation(0, "barabase_20_4.gpfc" ,1000,&compareResult,0.287882,1,true);
	std::cout << "DEFAULT ALGORITHM barabase_20_4.gpfc threshold Result:" << compareResult << " <<" << std::endl;
	
	BOOST_CHECK(abs(compareResult-0.287882)< 0.1);
}


#endif

BOOST_AUTO_TEST_CASE(compara){
	CFuncTrace trace (true,"COMPARA_THE_SAME_GRAPH");
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	gsl_matrix * matrixA;
	gsl_matrix * matrixB;
	gslGraph* graph = new gslGraph();
	
	const char *fileNameGraph="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test_4nodes.gpfc";
	graph->readPythonGraphFile((char*)fileNameGraph);
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



BOOST_AUTO_TEST_CASE(UTest_generateInitialGraph){
	CFuncTrace trace (true,"test_generateInitialGraph");

	StrategyPatternAlgorithm *strategyPatternAlgorithm = new StrategyPatternAlgorithm(NULL);
	
	//gslGraph *  generalGraph =
	strategyPatternAlgorithm->generateInitialGraph(4);
	gslGraph *  generalGraph = strategyPatternAlgorithm->getGraph();
	int degree = generalGraph->getDegree();
	int order = generalGraph->getOrder();
	trace.trace(CTrace::TRACE_INFO," Graph degree %d order %d",degree,order);

//	generalGraph->printGraph(CTrace::TRACE_INFO);
	BOOST_CHECK(degree == 3); // FAILED IN GRAPH
	BOOST_CHECK(order ==4);
	StrategyPatternAlgorithm *strategy= new StrategyPatternAlgorithm(NULL);
	strategy->modifyGraph(generalGraph);
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
	
	gslGraph *  generalGraph =
	StrategyPatternAlgorithm::generateInitialGraph(10,
												   random_value_x,random_value_y,random_value_z);
	
	int degree = generalGraph->getDegree();
	int order = generalGraph->getOrder();
	trace.trace(CTrace::TRACE_INFO," Graph degree %d order %d",degree,order);
	
	generalGraph->printGraph(CTrace::TRACE_INFO);
	BOOST_CHECK(degree == 3);
	BOOST_CHECK(order ==10);
	gslGraph *copyGraph = NULL;
	
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

BOOST_AUTO_TEST_CASE(UTest_generateInitialGraph_brandes_communicability_10_nodes){
	CFuncTrace trace (true,"test_generateInitialGraph_brandes_communicability_10_nodes");
	double *newBC = NULL;
	StrategyPatternAlgorithm * strategyPatternAlgorithm = new StrategyPatternAlgorithm(NULL);
	
	strategyPatternAlgorithm->generateInitialGraph(10);
	gslGraph *  generalGraph = strategyPatternAlgorithm->getGraph();
	
	graphIndicatorCommunicabilityCentralityUsingMatrixExponential *communicabilityCentrality =
	new graphIndicatorCommunicabilityCentralityUsingMatrixExponential(generalGraph);
	
	newBC = communicabilityCentrality->calculateIndicatorWithReescale(false);

	
	
	for ( int j =0 ; j < 10; j++){
		trace.trace(CTrace::TRACE_INFO,"NEWBC %f",newBC[j]);
		
	}
	BOOST_CHECK(abs(261.013261 - newBC[0]) < 0.00001);
	BOOST_CHECK(abs(225.343407 - newBC[1]) < 0.00001);
	BOOST_CHECK(abs(175.408517 - newBC[2] )< 0.00001);
	BOOST_CHECK(abs(261.013261 - newBC[3]) < 0.00001);
	BOOST_CHECK(abs(175.408517 - newBC[4]) < 0.00001);
	BOOST_CHECK(abs(181.020066 - newBC[5]) < 0.00001);
	BOOST_CHECK(abs(181.020066 - newBC[6] )< 0.00001);
	BOOST_CHECK(abs(261.013261 - newBC[7]) < 0.00001);
	BOOST_CHECK(abs(142.901666 - newBC[8]) < 0.00001);
	BOOST_CHECK(abs(98.909936 - newBC[9]) < 0.00001);
	
	for ( int j =0 ; j < 10; j++){
		trace.trace(CTrace::TRACE_INFO,"Degree %d",generalGraph->getDegree(j));
		
	}
	BOOST_CHECK( 9 == generalGraph->getDegree(0));
	BOOST_CHECK( 8 == generalGraph->getDegree(1));
	BOOST_CHECK( 7 == generalGraph->getDegree(2));
	BOOST_CHECK( 9 == generalGraph->getDegree(3));
	BOOST_CHECK( 7 == generalGraph->getDegree(4));
	BOOST_CHECK( 7 == generalGraph->getDegree(5));
	BOOST_CHECK( 7 == generalGraph->getDegree(6));
	BOOST_CHECK( 9 == generalGraph->getDegree(7));
	BOOST_CHECK( 6 == generalGraph->getDegree(8));
	BOOST_CHECK( 5 == generalGraph->getDegree(9));

	
//	generalGraph->printGraph(CTrace::TRACE_INFO);
	
	gslGraph *newGraph=NULL;
	newGraph=generalGraph->copyGraph();
	
	
	
	strategyPatternAlgorithm->modifyGraph(newGraph,CTrace::TRACE_INFO);
//	newGraph->printGraph(CTrace::TRACE_INFO);


	graphIndicatorCommunicabilityCentralityUsingMatrixExponential *communicabilityCentralityCopy =
	new graphIndicatorCommunicabilityCentralityUsingMatrixExponential(newGraph);
	
	
	newBC = communicabilityCentralityCopy->calculateIndicatorWithReescale(false);
	
	for ( int j =0 ; j < 10; j++){
		trace.trace(CTrace::TRACE_INFO,"NEWBC 2 %f",newBC[j]);
		
	}
	BOOST_CHECK(abs(261.013261 - newBC[0]) < 0.00001);
	BOOST_CHECK(abs(225.343407 - newBC[1]) < 0.00001);
	BOOST_CHECK(abs(175.408517 - newBC[2] )< 0.00001);
	BOOST_CHECK(abs(261.013261 - newBC[3]) < 0.00001);
	BOOST_CHECK(abs(175.408517 - newBC[4]) < 0.00001);
	BOOST_CHECK(abs(181.020066 - newBC[5]) < 0.00001);
	BOOST_CHECK(abs(181.020066 - newBC[6] )< 0.00001);
	BOOST_CHECK(abs(261.013261 - newBC[7]) < 0.00001);
	BOOST_CHECK(abs(142.901666 - newBC[8]) < 0.00001);
	BOOST_CHECK(abs(98.909936 - newBC[9]) < 0.00001);
	
	BOOST_CHECK( 9 == newGraph->getDegree(0));
	BOOST_CHECK( 8 == newGraph->getDegree(1));
	BOOST_CHECK( 7 == newGraph->getDegree(2));
	BOOST_CHECK( 9 == newGraph->getDegree(3));
	BOOST_CHECK( 7 == newGraph->getDegree(4));
	BOOST_CHECK( 7 == newGraph->getDegree(5));
	BOOST_CHECK( 7 == newGraph->getDegree(6));
	BOOST_CHECK( 9 == newGraph->getDegree(7));
	BOOST_CHECK( 6 == newGraph->getDegree(8));
	BOOST_CHECK( 5 == newGraph->getDegree(9));
}


BOOST_AUTO_TEST_CASE(UTest_generateInitialGraph_brandes_communicability_5_nodes){
	CFuncTrace trace (true,"UTest_generateInitialGraph_brandes_communicability_5_nodes");
	double *newBC = NULL;
	StrategyPatternAlgorithm * strategyPatternAlgorithm = new StrategyPatternAlgorithm(NULL);
	
	
	strategyPatternAlgorithm->generateInitialGraph(5);
	gslGraph *  generalGraph = strategyPatternAlgorithm->getGraph();
	
	// generalGraph->brandes_comunicability_centrality_exp(newBC);
	
	graphIndicatorCommunicabilityCentralityUsingMatrixExponential *communicabilityCentrality =
	new graphIndicatorCommunicabilityCentralityUsingMatrixExponential(generalGraph);
	
	newBC = communicabilityCentrality->calculateIndicatorWithReescale(false);
	
	for ( int j =0 ; j < 5; j++){
		trace.trace(CTrace::TRACE_INFO,"NEWBC %f",newBC[j]);
		trace.trace(CTrace::TRACE_INFO,"BOOST_CHECK(abs( %f - newBC[0]) < 0.00001);",newBC[j]);
		
	}
	
	BOOST_CHECK(abs(9.063906 - newBC[0]) < 0.00001);
	BOOST_CHECK(abs(9.063906 - newBC[1]) < 0.00001);
	BOOST_CHECK(abs(6.524227 - newBC[2]) < 0.00001);
	BOOST_CHECK(abs(9.063906 - newBC[3]) < 0.00001);
	BOOST_CHECK(abs(6.524227 - newBC[4]) < 0.00001);
	
	gslGraph *newGraph=NULL;
	newGraph=generalGraph->copyGraph();
	

	strategyPatternAlgorithm->modifyGraph(newGraph);
	// en aqest p
	//newGraph->brandes_comunicability_centrality_exp(newBC);
	
	graphIndicatorCommunicabilityCentralityUsingMatrixExponential *communicabilityCentralityCopy =
	new graphIndicatorCommunicabilityCentralityUsingMatrixExponential(newGraph);
	
	
	newBC = communicabilityCentralityCopy->calculateIndicatorWithReescale(false);
	
	for ( int j =0 ; j < 5; j++){
		trace.trace(CTrace::TRACE_INFO,"NEWBC 2 %f",newBC[j]);
	}
	BOOST_CHECK(abs(6.236668 - newBC[0]) < 0.00001);
	BOOST_CHECK(abs(6.236668 - newBC[1]) < 0.00001);
	BOOST_CHECK(abs(3.371805 - newBC[2]) < 0.00001);
	BOOST_CHECK(abs(3.371805 - newBC[3]) < 0.00001);
	BOOST_CHECK(abs(3.371805 - newBC[4]) < 0.00001);
	
}



BOOST_AUTO_TEST_CASE(UTEST_GENErateRandomNumbers){
	CFuncTrace trace (true,"UTEST_GENErateRandomNumbers");
	StrategyPatternAlgorithm * strategyPatternAlgorithm = new StrategyPatternAlgorithm(NULL);
	int values_random[10];
	memset(values_random,0,10*sizeof(int));
	for ( int i = 0; i < 10 ; i++){
		double value =strategyPatternAlgorithm->generateRandomNumber();
		trace.trace(CTrace::TRACE_INFO,"%f",value);
		values_random[(int)(value*10.0)]++;
	}
	for ( int i = 0; i < 10 ; i++){
		trace.trace(CTrace::TRACE_INFO," %d %d",i, values_random[i]);
	}
	
	BOOST_CHECK(values_random[0] == 0);
	BOOST_CHECK(values_random[1] == 0);
	BOOST_CHECK(values_random[2] == 0);
	BOOST_CHECK(values_random[3] == 2);
	BOOST_CHECK(values_random[4] == 1);
	BOOST_CHECK(values_random[5] == 0);
	BOOST_CHECK(values_random[6] == 3);
	BOOST_CHECK(values_random[7] == 3);
	BOOST_CHECK(values_random[8] == 1);
	BOOST_CHECK(values_random[9] == 0);
	
}

BOOST_AUTO_TEST_CASE(UTest_Betweeness_centraliy_wheel14_execution_time){
	CFuncTrace trace (true,"UTest_Betweeness_centraliyt_wheel14_execution_time");
	using namespace std;
	clock_t begin = clock();
	double compareResult = 10;
	simulation(BETWEENNESS_CENTRALITY, "wheel14.txt" ,1000,&compareResult,0.0458888);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	trace.trace(CTrace::TRACE_INFO,"Elapsed time %f %f",elapsed_secs,compareResult);
	// 56.893174 seconds 69.096907 72.344237 37.685672(-O3 and -DHAVE_INLINE -DGSL_RANGE_CHECK_OFF
	// 36.947093 matrix_calloc instead matrix zero
	// 28.101863, deleting sequence 25.411876
	// inline 23.243 // 22.87308 12.6194 10.082
	BOOST_CHECK((elapsed_secs ) < ( 10.082 + (elapsed_secs / 10)));
	
}

BOOST_AUTO_TEST_CASE(UTest_CommunicabilityBetweenessCentrality_wheel14_execution_time){
	CFuncTrace trace (true,"UTest_CommunicabilityBetweenessCentrality_wheel14_execution_time");
	using namespace std;
	clock_t begin = clock();
	double compareResult = 10;
	simulation(COMMUNICABILITY_CENTRALITY, "wheel14.txt" ,1000,&compareResult);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	trace.trace(CTrace::TRACE_INFO,"Elapsed time %f %f",elapsed_secs,compareResult);
	// 5.085668 , 4.088514 inline  3.983686
	BOOST_CHECK((elapsed_secs ) < ( 3.945558 + (elapsed_secs / 10)));
	
}


BOOST_AUTO_TEST_CASE(UTest_StrategyPatternAlgorithmThresholdAccepting){
	
	CFuncTrace trace (true,"UTest_StrategyPatternAlgorithmThresholdAccepting");

	const char *largv="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/test_4nodes.gpfc";
	double *TargetBC = NULL;
	double *BestBC = NULL;
	int graphOrder = 0;
	CSettingsSimulation *settingSimulation = new CSettingsSimulation() ;
	settingSimulation->inputFileName =largv;
	settingSimulation->setNMax(100);
	settingSimulation->setMaxIterations(100);
	settingSimulation->thresholdAccepting = THRESHOLD_ACCEPTING_ALGORITHM;
	settingSimulation->tMin = 0.0001;
	settingSimulation->To = 1;
	
	CRebuildGraph *rebuildGraph = new CRebuildGraph();
	double compareResult = 0.0;
	rebuildGraph->regenerateGraph(settingSimulation,BestBC,graphOrder,compareResult);
	BOOST_CHECK( 4 == graphOrder);
	double ExpectedBestBC[4]={0,0.66,0.66,0};
	for (int nVertex = 0;nVertex < graphOrder;nVertex++){
		
		std::cout << "BestBC[" << nVertex << "]" << BestBC[nVertex] << std::endl;
		BOOST_CHECK(fabs(BestBC[nVertex] - ExpectedBestBC[nVertex]) < 0.1 );
	}
	free(TargetBC);
	free(BestBC);
	free(settingSimulation);
}


BOOST_AUTO_TEST_CASE(UTest_generateInitialGraph_sfba_inipy) {
	CFuncTrace trace (true,"UTest_generateInitialGraph_sfba_inipy");
	
	StrategyPatternAlgorithm *strategyPatternAlgorithm = new StrategyPatternAlgorithm(NULL);
	
	const char *fileNameGraph="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/sfba-inipy.txt";
	gslGraph* graph = new gslGraph();
	graph->readPythonGraphFile((char*)fileNameGraph);
	int degreeSfgaInipy = graph->getDegree();
	int orderSfgaInipy = graph->getOrder();

	trace.trace(CTrace::TRACE_INFO," Graph degree %d order %d",degreeSfgaInipy,orderSfgaInipy);
	
	BOOST_CHECK(degreeSfgaInipy == 17); // FAILED IN GRAPH
	BOOST_CHECK(orderSfgaInipy == 40);
	
	
	graphIndicatorCommunicabilityCentralityUsingMatrixExponential *communicabilityCentrality =
	new graphIndicatorCommunicabilityCentralityUsingMatrixExponential(graph);
	
	double *target = communicabilityCentrality->calculateIndicatorWithReescale(false);
	
	delete communicabilityCentrality;
	
	for ( int j =0 ; j < 5; j++){
		trace.trace(CTrace::TRACE_INFO,"NEWBC %f",target[j]);
	}
	
	BOOST_CHECK(abs(45.652232 - target[0]) < 0.00001);
	BOOST_CHECK(abs(3.619358 - target[1]) < 0.00001);
	BOOST_CHECK(abs(14.810197 - target[2]) < 0.00001);
	BOOST_CHECK(abs(95.680259 - target[3]) < 0.00001);
	BOOST_CHECK(abs(80.759941 - target[4]) < 0.00001);
	
	
	
	strategyPatternAlgorithm->generateInitialGraph(graph->getOrder());
	gslGraph *  generalGraph = strategyPatternAlgorithm->getGraph();
	int degree = generalGraph->getDegree();
	int order = generalGraph->getOrder();
	trace.trace(CTrace::TRACE_INFO," Graph degree %d order %d",degree,order);
	
//	generalGraph->printGraph(CTrace::TRACE_INFO);
	BOOST_CHECK(degree == 39); // FAILED IN GRAPH
	BOOST_CHECK(order ==40);
	
	communicabilityCentrality = 
	new graphIndicatorCommunicabilityCentralityUsingMatrixExponential(generalGraph);
	
	//const char *inputGraphFilename="/Users/oscarraigcolon/Arrel/git/rebuild-graph/data/example_graphs/sfba-inipy.txt.new";
	//generalGraph->printMyGraph(inputGraphFilename,OUTPUT_ADJACENCY_LIST);
	
	double *newBC = communicabilityCentrality->calculateIndicatorWithReescale(false);
	
	delete communicabilityCentrality;
	
	for ( int j =0 ; j < 5; j++){
		trace.trace(CTrace::TRACE_INFO,"NEWBC %f",newBC[j]);
	}
	
	BOOST_CHECK(abs(223074037715.542542 - newBC[0]) < 0.00001);
	BOOST_CHECK(abs(96724489314.835770 - newBC[1]) < 0.00001);
	BOOST_CHECK(abs(77268189515.968658 - newBC[2]) < 0.00001);
	BOOST_CHECK(abs(121779028298.609863 - newBC[3]) < 0.00001);
	BOOST_CHECK(abs(103926740003.010391 - newBC[4]) < 0.00001);

	
	double cost = strategyPatternAlgorithm->cost(target,newBC, order);
	
	trace.trace(CTrace::TRACE_INFO," Cost %f",cost);
	BOOST_CHECK(abs(1040609828999.770264 - cost) < 0.00001);
	
	}
