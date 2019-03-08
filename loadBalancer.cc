//
// Created by Roy on 28/10/2018.
//
#include <iostream>
#include "loadBalancer.h"



int nextIdTask = 1; // each task will have its unique id

int loadBalancer::doAlg(){
	switch(this->alg){
		case JSQ : return doJSQ();
		case JIQ : return doJIQ();
		case PI : return doPI();
		default: return -1;
	}
}

/////////////////////////////// LB Functions: ///////////////////////////////

void loadBalancer::sendTasks(int serverIndex, int numOfTasks){ // send tasks to chosen server
	int old_size = this->servers.at(serverIndex).getServerSize();
	for(int i=0; i<numOfTasks; i++){
		Task outTask = Task(nextIdTask);
		this->servers.at(serverIndex).pushTask(outTask);
		nextIdTask++;
	}
	// JIQ, PI: if chosen server was empty and now got filled, remove it from empty_servers set
	if((this->alg == JIQ || this->alg == PI) && old_size == 0 && numOfTasks > 0) {
		empty_servers.erase(serverIndex);
	}
}

void loadBalancer::print() {
	int i=0;
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
		std::cout << "Server number " << i << ": " << std::endl;
		(*it).print();
		i++;
	}
}

void loadBalancer::setHomogeneous(double p) {
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
		(*it).setServerP(p);
	std::cout << "All Servers p: " << p << std::endl;
}



void loadBalancer::setHeterogeneous() {
	int nStrong=0, nWeak=0, ratio=0;
	double weakP, strongP;
	if(pCase == SOFT_HETEROGENEOUS){
		ratio = 2;
	}
	else if(pCase == HARD_HETEROGENEOUS){
		ratio = 10;
	}
	if(nRatio == fiveToFive){
		nStrong = 5;
		nWeak = 5;
	}
	else if(nRatio == oneToNine){
		nStrong = 1;
		nWeak = 9;
	}
	else if(nRatio == nineToOne){
		nStrong = 9;
		nWeak = 1;
	}


	weakP = calculateWeakP(nStrong,nWeak,ratio);
	strongP = weakP * (double)ratio;
	cout << "strongP: " << strongP << endl;
	cout << "weakP: " << weakP << endl;


	//weakP = pStrongConst/(double)ratio;
	vector<Server>::iterator it = servers.begin();
	for(int i=0; i<nWeak; i++){
		(*it).setServerP(weakP);
		it++;
	}
	for(int i=nWeak; i<numOfServers; i++){
		(*it).setServerP(strongP);
		it++;
	}
	//double lambda = calculateNHlambda(nStrong, nWeak, ratio);
	cout << "weakP: " << weakP << endl;
	cout << "strongP: " << strongP << endl;
}

void loadBalancer::completeTasks(int geomValues[]){
	int i = 0;
	/*
	for (int j=0; j<numOfServers; j++)
		cout << "Geom Values in index " << j << ": " << geomValues[j] << endl;
	*/
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++){
		(*it).completeTasks(geomValues[i]);
		i++;
		// JIQ, PI: if chosen server was empty and now got filled, remove it from empty_servers set
		if((this->alg == JIQ || this->alg == PI) && (*it).getServerSize() == 0) {
			this->empty_servers.insert((*it).getServerID());
		}
	}
}

int loadBalancer::averageTasksInSystem(){
	int taskSum = 0;
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++){
		taskSum += (*it).getServerSize(); // number of tasks currently in the i-th server
	}
	return taskSum;
}

void loadBalancer::clearServers(){
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
		(*it).clear();
}

/////////////////////////////// JSQ Functions: ///////////////////////////////

int loadBalancer::getMinimalServerSize(){
	int min = this->servers.at(0).getServerSize();
	for(vector<Server>::iterator it = servers.begin(); it!=servers.end(); it++){
		int currServerQueueSize = (*it).getServerSize();
		if(currServerQueueSize < min){
			min = currServerQueueSize;
		}
	}
	return min;
}

void loadBalancer::updateMinimalServersSet(int min){
	int currServerID, currServerSize;
	for(vector<Server>::iterator it = servers.begin(); it!=servers.end(); it++){
		currServerID = (*it).getServerID();
		currServerSize = (*it).getServerSize();
		// new addition to set
		if(currServerSize == min && minimal_servers.find(currServerID) == minimal_servers.end()){
			minimal_servers.insert(currServerID);
		}
		// remove from set
		else if(currServerSize != min && minimal_servers.find(currServerID) != minimal_servers.end()){
			minimal_servers.erase(currServerID);
		}
	}
}

int loadBalancer::doJSQ(){
	int min = getMinimalServerSize();
	updateMinimalServersSet(min);
	int chosenServerId = chooseRandomServer(minimal_servers, true);
	return chosenServerId;
}


/////////////////////////////// JIQ Functions: ///////////////////////////////

//empty server set is being updated in functions: completeTasks (Adds to set), sendTasks (Removes from set)
int loadBalancer::doJIQ(){
	int chosenServer;
	if(empty_servers.empty()){ // if there are no empty servers, pick one randomly from all servers
		chosenServer = chooseRandomServer(empty_servers, false); // doesnt consider empty_servers at all
	}
	else { // there are some empty servers, pick one randomly from empty servers
		chosenServer = chooseRandomServer(empty_servers, true);
	}
	return chosenServer;
}

/////////////////////////////// PI Functions: ///////////////////////////////
int loadBalancer::doPI(){
	int chosenServer;
	if(empty_servers.empty()){ // if there are no empty servers, pick the last idle
		chosenServer = this->lastIdle;
	}
	else { // there are some empty servers, pick one randomly from empty servers
		chosenServer = chooseRandomServer(empty_servers, true); // like JIQ
		this->lastIdle = chosenServer;
	}
	return chosenServer;
}







