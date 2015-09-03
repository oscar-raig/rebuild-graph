
#include <iostream>
#include "readPythonGraphFile.h"
#include "gslIO.hpp"

using namespace std;

#define RETURN_MAIN_OK      0
#define RETURN_MAIN_NOK     1
#define NUMBER_OF_ARGUMENTS 1

#define POSTION_ARGUMENT_FILE_NAME 1



int main(int argc, char* argv[]) {
	
	std::cout << "Example using gslIO" << std::endl;
	
	if (argc < NUMBER_OF_ARGUMENTS) {	
		std::cout << "ERROR: minimim 1 argument file to print" << std::endl;
		return RETURN_MAIN_NOK;
	}
	
	if (argv[1] == NULL) {
		std::cout << "ERROR: FilenName is NULL" << std::endl;
		return RETURN_MAIN_NOK;
	}
	char* fileName = argv[POSTION_ARGUMENT_FILE_NAME];
	gslGraph *  graphTest = NULL;
	try{ 
		graphTest =  ReadPythonGraphFile::readPythonGraphFile(fileName);
	} catch(std::exception const& e){
		std::cout << "ERROR: reading the File"  << std::endl;
		return RETURN_MAIN_NOK;
	}
	gslIO::printGslMatrix(graphTest->getGslMatrix(),"%0.0f ");
	delete graphTest;					  
	return RETURN_MAIN_OK;
}