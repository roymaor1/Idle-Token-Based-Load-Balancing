/////////////////////////////// Includes: ///////////////////////////////

#include <iostream>
#include <random>
#include <fstream>
#include <time.h>
#include "loadBalancer.h"


string algorithms[] = {"JSQ", "JIQ", "PI"};

/////////////////////////////// Helper Functions: ///////////////////////////////

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

/////////////////////////////// Main Function: ///////////////////////////////

int main() {
	clock_t start, end;
	start = clock();
	LB_Type my_alg;
	double lambda;
	unsigned seed = generateSeed(); // SEED
	std::default_random_engine eng = generateEngine(seed); // ENGINE
	double pValues[numOfServers] = {0.0};
	std::uniform_real_distribution<double> distribution(0.000000000001,1.0);
	for(int i=0; i<numOfServers; i++){
		pValues[i] = distribution(eng);
	}
	for(int i=0; i<3; i++){
		my_alg = (LB_Type)i;
		string filename = algorithms[my_alg] + "_" + to_string(numOfServers) + "_" + to_string(timeSlots) + "_" + getDate() + ".txt";
		ofstream meanTasksFile;
		meanTasksFile.open(filename);
		loadBalancer my_LB = loadBalancer(my_alg,numOfServers);
		if(pCase == HOMOGENOUS){
			my_LB.setHomogeneous(p);
			lambda = H_lambda;
		}
		else{
			my_LB.setHeterogeneous();
			cout << "lambda: " << NH_lambda << endl;
			//lambda = NH_lambda;
		}
		int currTasks; // amount of tasks in system
		uint64_t taskSum = 0;
		double meanTasks;
		int geomValues[numOfServers] = {0};
		std::geometric_distribution<int> dist_geom(p); // homogenous case
		// TODO: BUG IS HERE!!!
		// TODO: CREATE TWO DISTRIBUTION FUNCS - 1 FOR STRONG AND 1 FOR WEAK
		srand((unsigned int)time(NULL));
		double factors[numOfFactors] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.55, 0.6, 0.65,
										0.7, 0.75, 0.8, 0.82, 0.84, 0.86, 0.88,
										0.9, 0.91, 0.92, 0.93, 0.94, 0.95, 0.96,
										0.965, 0.97, 0.975, 0.98, 0.985, 0.99,
										0.995, 0.998};


		for(int i=0; i < numOfFactors; i++){ // CONCURRENT LOOP (each thread runs an iteration)

			my_LB.clearServers();
			nextIdTask = 1;
			std::cout << i << endl; // print iteration number
			std::poisson_distribution<int> dist_pois(factors[i]*NH_lambda); // called 30 times
			cout << "load: " << factors[i]*NH_lambda << endl;
			my_LB.print();

			for(int j=0; j<timeSlots; j++){
				int tasks = dist_pois(eng);

				//cout << i << " " << tasks << endl;
				// start slot
				// find server to push to by the LB's algorithm
				int chosenServer = my_LB.doAlg();
				//cout << "chosen server is: " << chosenServer << endl;
				// push all tasks in LB queue to the chosen server
				my_LB.sendTasks(chosenServer, tasks);
				// all servers completing tasks according to their p
				for(int i=0; i<numOfServers; i++){
					geomValues[i] = dist_geom(eng);
					//cout << "completed tasks of server " << i << ": " << geomValues[i] << endl;
				}
				my_LB.completeTasks(geomValues);
				// finish slot
				currTasks = my_LB.averageTasksInSystem();
				//cout << "sum of tasks in system: " << currTasks << endl;
				taskSum += currTasks;
				// printing
				//my_LB.print();
				//cout << "--------------------------------------------------" << endl;
			}
			meanTasks = (double)taskSum/(double)timeSlots; // called 30 times
			meanTasksFile << factors[i] << " " << meanTasks << endl;
			taskSum = 0;
		}
		meanTasksFile.close();
	}
	end = clock();
	double duration_sec = double(end-start)/CLOCKS_PER_SEC;
	cout << "execution time: " << duration_sec << " seconds (" << duration_sec/60.0 << " minutes)" << endl;
	return 0;
}