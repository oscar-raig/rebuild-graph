#include <iostream>
#include "gtest/gtest.h"
#include "analitza.h"


#define FILE_UNDER_TEST "/Users/oscarraigcolon/Arrel/git/" \
        "rebuild-graph/src/analitza/data/circ-inipy.txt"

TEST(Llegir_dadaes, if_file_do_not_exists_should_return_error) {
    int result = 0;
    int linies = 0;
    int maxveins = 0;
    int minveins = 0;
    float mitja = 0;

    result = llegir_dades("superfile.txt", linies, maxveins, minveins, mitja);
    EXPECT_TRUE(result == READ_GRAPH_ERROR);
}


TEST(Llegir_dadaes, if_file_exist_should_return_ok) {
    int result = 0;
    int linies = 0;
    int maxveins = 0;
    int minveins = 0;
    float mitja = 0;

    result = llegir_dades(FILE_UNDER_TEST, linies, maxveins, minveins, mitja);
    EXPECT_TRUE(result == READ_GRAPH_OK);
}


TEST(Llegir_dadaes, circ_graph_should_return_specificvalues) {
    int result = 0;
    int nodes = 0;
    int maxNeighbour = 0;
    int minNeighbour = 0;
    float average = 0;
    result = llegir_dades(FILE_UNDER_TEST, nodes,
                          maxNeighbour, minNeighbour, average);
    EXPECT_EQ(nodes, 40);
    EXPECT_EQ(maxNeighbour, 5);
    EXPECT_EQ(minNeighbour, 1);
    EXPECT_EQ(average, 3);
}



int main(int argc, char**argv) {
    testing::InitGoogleTest(&argc, argv);
    return(RUN_ALL_TESTS());
}
