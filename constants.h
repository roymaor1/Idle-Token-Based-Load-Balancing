//
// Created by Roy on 10/12/2018.
//
#include <iostream>
#include <string>

#ifndef INC_10_PROJECT_B_CONSTANTS_H
#define INC_10_PROJECT_B_CONSTANTS_H


/////////////////////////////// Constants: ///////////////////////////////

enum pType {HOMOGENOUS, SOFT_HETEROGENEOUS, HARD_HETEROGENEOUS};
enum ratio {oneToNine, fiveToFive, nineToOne}; //left side is strong server
// definitions of time slice and servers
// final tests should take timeSlots = 1000000, numOfServers = 10

const int timeSlots = (int)10e4; // num of time iterations.
const int numOfServers = 10; // num of servers

//p
const pType pCase = SOFT_HETEROGENEOUS;
const ratio nRatio = fiveToFive;


// definitions of distributional parameters
// Condition: numOfServers*(1/p) >= lambda (homogeneous case)


//const double pStrongConst = 0.9;
const double NH_lambda = 100.0; // mean: lambda. >81
const double H_lambda = 20.0; // mean: lambda
const double p = 1.0/3.0; // mean: 1-p/p (2 for p=1/3)
const int numOfFactors = 30; // width of sampling vector
//const int UNI_MIN = 0;
//const int UNI_MAX = 10;

#endif //INC_10_PROJECT_B_CONSTANTS_H
