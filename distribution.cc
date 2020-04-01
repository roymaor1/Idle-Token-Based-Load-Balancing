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

int chooseRandomServerFromSet(set<int> serverPool, bool isValid){
	//return *(serverPool.begin());
	int poolSize, randNum, chosenIndex, chosenServer;
	randNum = rand();
	if(!isValid){ //randomizing among all servers
		poolSize = numOfServers;
		chosenIndex = randNum % poolSize;
		return chosenIndex; // server and index are the same
	}
	poolSize = (int)serverPool.size();
	chosenIndex = randNum % poolSize;
	set<int>::iterator it = serverPool.begin();
	advance(it,chosenIndex);
	chosenServer = *it;
	return chosenServer;
}

int chooseRandomServerFromQueue(queue<int> serverPool, bool isValid){
	//return *(serverPool.begin());
	int poolSize, randNum, chosenIndex, chosenServer, numOfPops;
	randNum = rand();
	if(!isValid){ //randomizing among all servers
		poolSize = numOfServers;
		chosenIndex = randNum % poolSize;
		return chosenIndex; // server and index are the same
	}
	poolSize = (int)serverPool.size();
	numOfPops = randNum % poolSize;
	queue<int> poolCopy = serverPool;
	for(int i=0; i<numOfPops; i++){
		poolCopy.pop();
	}
	chosenServer = poolCopy.front();
	return chosenServer;
}

double calculateStrongP(int nStrong, int nWeak, int ratio){
	double nom = nWeak + nStrong * ratio;
	double denom = ratio * (lambda_NH + nStrong + nWeak);
	return nom/denom;
}

void calcParams(int h_type, int* n_Strong, double* p_Strong, double* p_Weak){
	switch (h_type){
		case 1 :
			*n_Strong = 5;
			*p_Strong = calculateStrongP(5,5,2);
			*p_Weak = *p_Strong * 2;
			break;
		case 2 :
			*n_Strong = 9;
			*p_Strong = calculateStrongP(9,1,2);
			*p_Weak = *p_Strong * 2;
			break;
		case 3 :
			*n_Strong = 1;
			*p_Strong = calculateStrongP(1,9,2);
			*p_Weak = *p_Strong * 2;
			break;
		case 4 :
			*n_Strong = 5;
			*p_Strong = calculateStrongP(5,5,10);
			*p_Weak = *p_Strong * 10;
			break;
		case 5 :
			*n_Strong = 9;
			*p_Strong = calculateStrongP(9,1,10);
			*p_Weak = *p_Strong * 10;
			break;
		case 6 :
			*n_Strong = 1;
			*p_Strong = calculateStrongP(1,9,10);
			*p_Weak = *p_Strong * 10;
			break;
		default:
			break;
	}
}


double calculateMean(double constant /*int nStrong, double p_Strong, int ratio*/){
    return log(constant) - 0.5;
    /*
	if(ratio == 1){ //homo
		return log(10*(1-p_Strong))-log(p_Strong)-0.5;
	}
	int nWeak  = numOfServers - nStrong;
	double ln1A = ratio * nStrong * (1-p_Strong);
	double ln1B = nWeak * (1 - ratio * p_Strong);
	double ln2 = ratio * p_Strong;
	double mean = log(ln1A + ln1B) - log(ln2) - 0.5;
	return mean;
     */

}

int getRatio(int Case){
    switch(Case){
    	case 0 :
    		return 1;
        case 1 :
        case 2 :
        case 3 :
        	return 2;
        case 4 :
        case 5 :
        case 6 :
            return 10;
        default :
            break;
    }
}
///
