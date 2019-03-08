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
#include "constants.h"
#include <time.h>
using namespace std;

int chooseRandomServer(set<int> serverPool, bool isValid);
unsigned generateSeed();
std::default_random_engine generateEngine(unsigned seed);
double calculateWeakP(int nStrong, int nWeak, int ratio);
double calculateNHlambda(int nStrong, int nWeak, int ratio);
#endif