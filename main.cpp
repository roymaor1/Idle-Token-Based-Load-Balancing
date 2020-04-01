/////////////////////////////// Includes: ///////////////////////////////

#include <iostream>
#include <random>
#include <fstream>
#include <time.h>
#include "loadBalancer.h"


string algorithms[] = {"JSQ", "JIQ", "PI", "PI_K"};

//*****************************************************************************************************
// function name:	getDate
// Description:		Writes the run date on output file
// Parameters:		none
// Returns:		none
//*****************************************************************************************************
string getDate(){
    time_t rawtime;
    struct tm * timeinfo;
    const int size = 80;
    char buffer [size];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,size,"%d_%m_%y_%H_%M_%S",timeinfo);
    return string(buffer);
}

/////////////////////////////// Helper Functions: ///////////////////////////////

static vector<double> factors_0 = {0.1, 0.2, 0.3, 0.4, 0.5, 0.55, 0.6, 0.65,
                         0.7, 0.75, 0.8, 0.82, 0.84, 0.86, 0.88,
                         0.9, 0.91, 0.92, 0.93, 0.94, 0.95, 0.96,
                         0.965, 0.97, 0.975, 0.98, 0.985, 0.99,
                         0.995, 0.998, 1, 1.005, 1.01, 1.02};
static vector<double> factors_1_3 = {0.84, 0.86, 0.88,
                              0.9,0.9017,0.9034,0.9051,0.9068,0.9085,0.9102,0.9119,0.9136,0.9153,
                              0.9169,0.9186,0.9203,0.922,0.9237,0.9254,0.9271,0.9288,0.9305,0.9322,
                              0.9339,0.9356,0.9373,0.939,0.9407,0.9424,0.9441,0.9458,0.9475,0.9492,
                              0.9508,0.9525,0.9542,0.9559,0.9576,0.9593,0.961,0.9627,0.9644,0.9661,
                              0.9678,0.9695,0.9712,0.9729,0.9746,0.9763,0.978,0.9797,0.9814,0.9831,
                              0.9847,0.9864,0.9881,0.9898,0.9915,0.9932,0.9949,0.9966,0.9983,1,
                              1.002, 1.005, 1.01, 1.02};
static vector<double> factors_4_6 = {0.5,0.55,0.6,0.65,0.6538,0.6577,0.6615,0.6654,0.6692,0.6731,
                        0.6769,0.6808,0.6846,0.6885,0.6923,0.6962,0.7,0.7038,0.7077,0.7115,
                        0.7154,0.7192,0.7231,0.7269,0.7308,0.7346,0.7385,0.7423,0.7462,0.75,
                        0.7538,0.7577,0.7615,0.7654,0.7692,0.7731,0.7769,0.7808,0.7846,0.7885,
                        0.7923,0.7962,0.8,0.81,0.82,0.83,0.84,0.85,0.86,0.87,
                        0.88,0.89,0.9,0.91,0.92,0.93,0.94,0.95,0.96,0.97,
                        0.98,0.99,0.995,1,1.002, 1.005, 1.01, 1.02};

/////////////////////////////// Main Function: ///////////////////////////////

int main() { //int argc, char *argv[]

    /// Time Definitions (for output file)
    clock_t start, end;
    start = clock();
    double duration_sec;
    srand((unsigned int)time(NULL));

    /// General Definitions
    LB_Type my_alg = JSQ; // just for init


    for(int Case = 0; Case <= 6; Case++){

        /// Defines LB's characteristics (1 of 7 options, 0-6)
        int n_Strong;
        double p_Strong, p_Weak;
        double p_H = p_H_Global; // in use only in homogenous case (case 0)
        int ratio = getRatio(Case);
        calcParams(Case,&n_Strong,&p_Strong,&p_Weak);
        double logNormalMean;
        if(Case==0){
            logNormalMean = calculateMean(lambda_H/*n_Strong,p_H,ratio*/);
        }
        else{
            logNormalMean = calculateMean(lambda_NH/*n_Strong,p_Strong,ratio*/);
        }


        //logNormalMean = 20;
        //cout << "----logNormalMean: " << logNormalMean << endl;



        /// Initialize distibution functions and definitions
        std::geometric_distribution<int> distGeomHomo(p_H); // dist_geom is homogenous
        std::geometric_distribution<int> distGeomStrong(p_Strong); // distGeomWeak is hetrogenous
        std::geometric_distribution<int> distGeomWeak(p_Weak); // distGeomStrong is hetrogenous
        double distParam; // the distribution pararmater by which the LB_LogNormal sends tasks
        unsigned seed = generateSeed(); // seed (for engine)
        std::default_random_engine eng = generateEngine(seed); // engine (for dist. functions)


        for( int k = 2; k <= 4; k++){





            /// Start run for each algorithm (JSQ, JIQ, PI, PI_K)
            for(int n = 0; n < numOfAlgs; n++){

                /// If not PI_K, do only k==2
                if(k>2 && n<3){
                    continue;
                }

                /// Calculate LogNormalMean

                /// Alg Type (JSQ, JIQ, PI, PI_K)
                my_alg = (LB_Type)n;


                /// Output file
                string filename = algorithms[my_alg] + "_" + to_string(k) + "_" + to_string(Case) + "_" + getDate() + ".txt";
                ofstream meanTasksFile;
                meanTasksFile.open(filename);

                /// Init LB
                loadBalancer my_LB = loadBalancer(my_alg,numOfServers,k);
                if(Case == 0){ // Homo case
                    my_LB.setHomogeneous(p_H);
                    distParam = lambda_H;
                }
                else{ // Hetero case
                    my_LB.setHeterogeneous(p_Strong,p_Weak,n_Strong);
                    distParam = lambda_NH;
                }

                /// Task Definitions
                int currTasks; // amount of tasks in system
                uint64_t taskSum = 0;
                double meanTasks;

                /// Server Definitions
                int geomValues[numOfServers] = {0}; //completed tasks for each server, each iteration
                vector<double> factors;
                if(Case == 0){
                    factors = factors_0;
                }
                else if (Case<4){
                    factors = factors_1_3;
                }
                else{
                    factors = factors_4_6;
                }
                int numOfFactors = (int)factors.size();

                /// Print stage
                std::cout << "case: " << Case << ", alg: " << algorithms[n] << ", k: " << k << endl;

                /// Main Algorithm Run
                for(int i=0; i < numOfFactors; i++){

                    my_LB.clearServers();
                    nextIdTask = 1;

                    /// Print iteration number
                    //std::cout << "case: " << Case << ", alg: " << algorithms[n] << ", k: " << k << ", iter: " << i << endl;

                    /// Poisson/Lognormal distribution (for each LB factor)
                    //std::poisson_distribution<int> distEntry(factors[i]*distParam); // called 30 times
                    std::lognormal_distribution<double> distEntry(factors[i]*logNormalMean, 1); // entry of tasks distribution

                    //cout << "load: " << factors[i]*distParam << endl;

                    /// Print Full Status
                    //my_LB.print();

                    /// Empiric Mean Check
                    //cout << "logNormal Mean: " << factors[i]*logNormalMean;
                    //double empiricSum = 0;

                    for(int j=0; j<timeSlots; j++){

                        int tasks = distEntry(eng);
                        //empiricSum+=tasks;
                        // print iteration number
                        //cout << i << ": " << tasks << endl;

                        /// Starts time slot

                        /// Find server to push to by the LB's algorithm
                        int chosenServer = my_LB.doAlg();
                        //cout << "chosen server is: " << chosenServer << endl;

                        /// Push all tasks in LB queue to the chosen server
                        my_LB.sendTasks(chosenServer, tasks);

                        /// All servers completing tasks according to their p
                        for(int t=0; t<numOfServers; t++) {
                            switch (Case) {
                                case 0 :
                                    geomValues[t] = distGeomHomo(eng);
                                    break;
                                case 1 :
                                case 4 :
                                    if(t < 5)
                                        geomValues[t] = distGeomStrong(eng);
                                    else
                                        geomValues[t] = distGeomWeak(eng);
                                    break;
                                case 2 :
                                case 5 :
                                    if(t < 9)
                                        geomValues[t] = distGeomStrong(eng);
                                    else
                                        geomValues[t] = distGeomWeak(eng);
                                    break;
                                case 3 :
                                case 6 :
                                    if(t == 0)
                                        geomValues[t] = distGeomStrong(eng);
                                    else
                                        geomValues[t] = distGeomWeak(eng);
                                    break;
                                default:
                                    break;
                            }
                        }


                        /// Complete tasks
                        //cout << "completed tasks of server " << i << ": " << geomValues[i] << endl;
                        my_LB.completeTasks(geomValues);

                        /// Finish time slot
                        currTasks = my_LB.averageTasksInSystem();
                        //cout << "sum of tasks in system: " << currTasks << endl;
                        taskSum += currTasks;

                        /// Print LB status
                        //my_LB.print();
                        //cout << "--------------------------------------------------" << endl;
                    }


                    //cout << "empiricMean: " << (double)empiricSum/timeSlots << endl;

                    /// Finished all time slots for factor X
                    /// Analyze data for X + write to output file
                    meanTasks = (double)taskSum/(double)timeSlots; // called 30 times
                    meanTasksFile << factors[i] << " " << meanTasks << endl; //<< "\n";
                    taskSum = 0;
                }
                meanTasksFile.close();
                factors.clear();




            } // end of alg (in this case, we run only PI(k), so only one iteration)


            /// Execution Time Calculator
            end = clock();
            duration_sec = double(end-start)/CLOCKS_PER_SEC;
            cout << "execution time of " << Case << "," << algorithms[my_alg] << "," << k << "seconds: " << duration_sec/60.0 << " minutes)" << endl;



        } // end of k, go back up for new k (same case)



    } // end of case
    end = clock();
    duration_sec = double(end-start)/CLOCKS_PER_SEC;
    cout << "execution time of program: " << duration_sec << " seconds (" << duration_sec/60.0 << " minutes)" << endl;

    return 0;
}
//
