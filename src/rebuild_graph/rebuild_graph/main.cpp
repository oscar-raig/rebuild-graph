//
//  main.c
//  rebuild_graph
//
//  Created by Oscar Raig Colon on 22/03/14.
//  Copyright (c) 2014 Oscar Raig Colon. All rights reserved.
//

#include <stdio.h>



int
fmain(int argc, const char *argv[], double *&ptargetBC, double *&pbestBC,int *order);
int fCalculateBeterness(const char *argv[]);
int fCalculateCommunicability(const char *argv[]);
int fCalculateCommunicability_cent_exp(const char *argv[]);

int main(int argc, const char * argv[])
{

    // insert code here...
    printf("Hello, World!\n");
	double *TargetBC = NULL;
	double *BestBC = NULL;
	int order = 0;

    fmain(argc,argv,TargetBC,BestBC,&order);
	
	//fCalculateBeterness(argv);
    //fCalculateCommunicability(argv);
	
//	fCalculateCommunicability_cent_exp(argv);
	return 0;
}

