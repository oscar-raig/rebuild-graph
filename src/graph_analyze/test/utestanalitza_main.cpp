#include <iostream>
#include "gtest/gtest.h"
#include "graph_analyze.hpp"


#define FILE_UNDER_TEST_CIRC "./data/circ-inipy.txt"

#define FILE_UNDER_TEST_RAND "./data/rand-inipy.txt"



TEST(Llegir_dadaes, if_file_do_not_exists_should_return_error) {
    int result = 0;
    int linies = 0;
    int maxveins = 0;
    int minveins = 0;
    float mitja = 0;

    result = llegir_dades("superfile.txt", &linies, &maxveins, &minveins, &mitja);
    EXPECT_TRUE(result == READ_GRAPH_ERROR);
}


TEST(Llegir_dadaes, if_file_exist_should_return_ok) {
    int result = 0;
    int linies = 0;
    int maxveins = 0;
    int minveins = 0;
    float mitja = 0;

    result = llegir_dades(FILE_UNDER_TEST_CIRC, &linies, &maxveins, &minveins, &mitja);
    EXPECT_TRUE(result == READ_GRAPH_OK);
}


TEST(Llegir_dadaes, circ_graph_should_return_specificvalues) {
    int result = 0;
    int nodes = 0;
    int maxNeighbour = 0;
    int minNeighbour = 0;
    float average = 0;
    result = llegir_dades(FILE_UNDER_TEST_CIRC, &nodes,
                          &maxNeighbour, &minNeighbour, &average);
    EXPECT_EQ(nodes, 40);
    EXPECT_EQ(maxNeighbour, 5);
    EXPECT_EQ(minNeighbour, 1);
    EXPECT_EQ(average, 3);
}


TEST(clustering,circ_graph_should_return_specificvalues) {
    int result = 0;
    int nodes = 0;
    int maxNeighbour = 0;
    int minNeighbour = 0;
    float average = 0;
    #define NUMBER_VERTEX_GRAPH_CIRC 40
    float expectedClustering[NUMBER_VERTEX_GRAPH_CIRC] = {
        0,0,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,0
    };
    
    result = llegir_dades(FILE_UNDER_TEST_CIRC, &nodes,
                          &maxNeighbour, &minNeighbour, &average);
    for ( int vertex = 0; vertex < nodes ; vertex++) {
        int clusterVertex = clustering(vertex);
         EXPECT_EQ(clusterVertex, expectedClustering[vertex]);

    }
  
}

TEST(clustering,rand_graph_should_return_specificvalues) {
    int result = 0;
    int nodes = 0;
    int maxNeighbour = 0;
    int minNeighbour = 0;
    float average = 0;
    #define NUMBER_VERTEX_GRAPH_RAND 40
    float expectedClustering[NUMBER_VERTEX_GRAPH_RAND] = {
        0,1,0,1,0,0,0,0,0,1,
        0,0,0,1,1,0,1,0,1,0,
        1,0,0,0,0,1,1,1,0,0,
        0,0,0,1,0,1,1,0,0,0
    };
    
    result = llegir_dades(FILE_UNDER_TEST_RAND, &nodes,
                          &maxNeighbour, &minNeighbour, &average);
    for ( int vertex = 0; vertex < nodes ; vertex++) {
        int clusterVertex = clustering(vertex);
        EXPECT_EQ(clusterVertex, expectedClustering[vertex]);
        
    }
    
}


TEST(distances,circ_graph_should_return_specificvalues) {
    int result = 0;
    int nodes = 0;
    int maxNeighbour = 0;
    int minNeighbour = 0;
    float average = 0;
    float clustering,average_distance;
    int diameter;
#define NUMBER_VERTEX_GRAPH_CIRC 40

    
    result = llegir_dades(FILE_UNDER_TEST_RAND, &nodes,
                          &maxNeighbour, &minNeighbour, &average);

    int clusterVertex = distancies(&clustering,&diameter,&average_distance);
    ASSERT_FLOAT_EQ(clustering,0.594762);
    EXPECT_EQ(diameter, 5);
    ASSERT_FLOAT_EQ(average_distance, 0.25512832);
        

 
}


int main(int argc, char**argv) {
    testing::InitGoogleTest(&argc, argv);
    return(RUN_ALL_TESTS());
}
