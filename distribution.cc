//
// Created by Roy on 15/11/2018.
//

#include "distribution.h"

// called once
unsigned generateSeed(){
	unsigned seed = (unsigned int)std::chrono::steady_clock::now().time_since_epoch().count();
	return seed;
}

std::default_random_engine generateEngine(unsigned seed){
	std::default_random_engine engine(seed); // seed engine. must not be declared staticly
	return engine;
}

int chooseRandomServer(set<int> serverPool, bool isValid){
	//return *(serverPool.begin());
	int poolSize, randNum, chosenIndex, chosenServer;
	randNum = rand();
	if(isValid == false){ //randomizing among all servers
		poolSize = numOfServers;
		chosenIndex = randNum % poolSize;
		return chosenIndex; // server and index are the same
	}
	poolSize = serverPool.size();
	chosenIndex = randNum % poolSize;
	set<int>::iterator it = serverPool.begin();
	advance(it,chosenIndex);
	chosenServer = *it;
	return chosenServer;
}

double calculateWeakP(int nStrong, int nWeak, int ratio){
	double nom = nStrong + nWeak * ratio;
	double denom = ratio * (NH_lambda + nStrong + nWeak);
	return nom/denom;
}
/*
double calculateNHlambda(int nStrong, int nWeak, int ratio){
	double pWeakConst = pStrongConst/(double)ratio;
	double partA = (nStrong * (1-pStrongConst))/pStrongConst;
	double partB = (nWeak * (1-pWeakConst))/pWeakConst;
	return partA+partB;
}
 */