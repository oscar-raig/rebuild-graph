#include <iostream>

#include "gtest/gtest.h"
#include "analitza.h"


#define file_under_test "

TEST(Llegir_dadaes, Positive) {
    int result = 0;
    int linies = 0;
    int maxveins = 0;
    int minveins = 0;
    float mitja = 0;
    
    
    result = llegir_dades("superfile.txt", linies, maxveins, minveins, mitja );
    EXPECT_TRUE(result == -1);
    
}


int main(int argc, char**argv)
{
    testing::InitGoogleTest(&argc, argv);
    return(RUN_ALL_TESTS());
}