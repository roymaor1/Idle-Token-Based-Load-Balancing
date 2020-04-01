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
		case PI_K : return doPI_K();
		default: return -1;
	}
}

/////////////////////////////// LB Functions: ///////////////////////////////

void loadBalancer::sendTasks(int serverIndex, int numOfTasks){ // send tasks to chosen server
	int old_size = this->servers.at(serverIndex).getServerSize();
	for(int i=0; i<numOfTasks; i++){
		Task outTask = Task(nextIdTask);
		servers.at(serverIndex).pushTask(outTask);
		nextIdTask++;
	}
	// JIQ, PI: if chosen server was empty and now got filled, remove it from empty_servers set
	if((alg != JSQ) && old_size == 0 && numOfTasks > 0) {
		empty_servers.erase(serverIndex);
	}
}

void loadBalancer::completeTasks(int geomValues[]){
	int i = 0, old_size;
	/*
	for (int j=0; j<numOfServers; j++)
		cout << "Geom Values in index " << j << ": " << geomValues[j] << endl;
	*/
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++){
		old_size = (*it).getServerSize();
		(*it).completeTasks(geomValues[i]);
		i++;
		// JIQ, PI: if chosen server was empty and now got filled, remove it from empty_servers set
		if((alg != JSQ) && (*it).getServerSize() == 0) {
			empty_servers.insert((*it).getServerID()); // set allows no dups
		}
		if((alg == PI_K) && (*it).getServerSize() == 0 && old_size > 0){
			if(lastIdled.size() == k){
				lastIdled.pop();
			}
			lastIdled.push((*it).getServerID());
		}
	}
}

void loadBalancer::print() {
	int i=0;
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
		//std::cout << "Server number " << i << ": " << std::endl;
		(*it).print();
		i++;
	}
}

void loadBalancer::setHomogeneous(double p) {
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
		(*it).setServerP(p);
	//std::cout << "All Servers p: " << p << std::endl;
}

void loadBalancer::setHeterogeneous(double pStrong, double pWeak, int nStrong) {
	vector<Server>::iterator it = servers.begin();
	for(int i=0; i<nStrong; i++){
		(*it).setServerP(pStrong);
		it++;
	}
	for(int i=nStrong; i<numOfServers; i++){
		(*it).setServerP(pWeak);
		it++;
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
	int chosenServerId = chooseRandomServerFromSet(minimal_servers, true);
	return chosenServerId;
}


/////////////////////////////// JIQ Functions: ///////////////////////////////

//empty server set is being updated in functions: completeTasks (Adds to set), sendTasks (Removes from set)
int loadBalancer::doJIQ(){
	int chosenServer;
	if(empty_servers.empty()){ // if there are no empty servers, pick one randomly from all servers
		chosenServer = chooseRandomServerFromSet(empty_servers, false); // doesnt consider empty_servers at all
	}
	else { // there are some empty servers, pick one randomly from empty servers
		chosenServer = chooseRandomServerFromSet(empty_servers, true);
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
		chosenServer = chooseRandomServerFromSet(empty_servers, true); // like JIQ
		this->lastIdle = chosenServer;
	}
	return chosenServer;
}

int loadBalancer::doPI_K(){
	int chosenServer;
	if(empty_servers.empty()){ // if there are no empty servers
		if(lastIdled.empty()){
			chosenServer = chooseRandomServerFromQueue(lastIdled, false); // pick one from all
		}
		else{
			chosenServer = chooseRandomServerFromQueue(lastIdled, true); // pick one from queue
		}
	}
	else { // there are some empty servers, pick one randomly from empty servers
		chosenServer = chooseRandomServerFromSet(empty_servers, true); // like JIQ and PI
	}
	return chosenServer;

	/*
	int chosenServer;
	if(lastIdled.empty()){ // randomize from all servers
		chosenServer = chooseRandomServerFromQueue(lastIdled, false);
	}
	else{
		chosenServer = chooseRandomServerFromQueue(lastIdled, true);
	}
	return chosenServer;
	 */
}
///
