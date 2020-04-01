//
// Created by Roy on 10/12/2018.
//
using namespace std;
#include <iostream>
#include <string>

#ifndef INC_10_PROJECT_B_CONSTANTS_H
#define INC_10_PROJECT_B_CONSTANTS_H


/////////////////////////////// Constants: ///////////////////////////////

/// Type definitions (from main):
/*
 * 0 - HOMOGENOUS
 * 1 - SOFT_HETEROGENEOUS + fiveToFive
 * 2 - SOFT_HETEROGENEOUS + nineToOne
 * 3 - SOFT_HETEROGENEOUS + oneToNine
 * 4 - HARD_HETEROGENEOUS + fiveToFive
 * 5 - HARD_HETEROGENEOUS + nineToOne
 * 6 - HARD_HETEROGENEOUS + oneToNine
 */

// 1000 - 1 thousand
// 10000 - 10 thousand
// 100000 - 100 thousand
// 1000000 - 1 million

/// Time slice and #Servers
/// Final tests should take timeSlots = 1000000, numOfServers = 10
const int timeSlots = (int)10000000; // num of time iterations
const int numOfServers = 10; // num of servers

/// Factors
const int numOfAlgs = 4; // JSQ, JIQ, PI, PI_K

/// Distributional parameters
/// Condition: numOfServers*(1/p) >= lambda (homogeneous case)

/// These lambda values set the p values (they are constant)!
const double p_H_Global = 1.0/3.0; // mean: 1-p/p (2 for p=1/3)
const double lambda_H = 20.0; // mean: lambda. H = HOMOGENOUS
const double lambda_NH = 100.0; // mean: lambda. >81. NH = NOT HOMOGENOUS



//
#endif //INC_10_PROJECT_B_CONSTANTS_H
