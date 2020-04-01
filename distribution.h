//
// Created by Roy on 15/11/2018.
//

#ifndef DISTRIBUTION_H_
#define DISTRIBUTION_H_


#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <set>
#include <queue>
#include "constants.h"
#include <time.h>
using namespace std;

//*****************************************************************************************************
		// function name:	chooseRandomServerFromSet	
		// Description:		If server pool exists and valid, chooses randomly among it. Elsewise chooses from all servers
		// Parameters:		set<int> serverPool, bool isValid
		// Returns:		chosen server's id
//*****************************************************************************************************
int chooseRandomServerFromSet(set<int> serverPool, bool isValid);

//*****************************************************************************************************
		// function name:	calculateStrongP	
		// Description:		Accepts number of strong and weak servers and ration, and calculates p value for strong servers
		// Parameters:		int nStrong, int nWeak, int ratio
		// Returns:		strong P value
//*****************************************************************************************************
double calculateStrongP(int nStrong, int nWeak, int ratio);

unsigned generateSeed();

std::default_random_engine generateEngine(unsigned seed);

int getRatio(int Case);

int chooseRandomServerFromQueue(queue<int> serverPool, bool isValid);

void calcParams(int h_type, int* n_Strong, double* p_Strong, double* p_Weak);

double calculateMean(double constant);

//
#endif
