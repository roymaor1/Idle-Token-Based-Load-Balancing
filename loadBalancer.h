//
// Created by Roy & Rachely on 28/10/2018.
//

#ifndef LOADBALANCER_LOADBALANCER_H
#define LOADBALANCER_LOADBALANCER_H
#include "server.h"
#include <set>
#include <queue>
extern int nextIdTask;

enum LB_Type { JSQ, JIQ, PI, PI_K };


class loadBalancer {

private:
	LB_Type alg;
	vector<Server> servers;
	set<int> minimal_servers; // JSQ
	set<int> empty_servers; // JIQ, PI
	int lastIdle; // PI
	queue<int> lastIdled; // PI_K
	int k; // PI_K
public:

	loadBalancer(LB_Type t, int size, int k){
		this->alg = t;
		this->servers = vector<Server>(size);
		this->lastIdle = -1;
		int i = 0;
		if(alg == JSQ){
			for(vector<Server>::iterator it = servers.begin(); it!=servers.end(); it++){
				(*it).setServerID(i);
				minimal_servers.insert(i);
				i++;
			}
		}
		else{ // JIQ, PI, PI_K
			for(vector<Server>::iterator it = servers.begin(); it!=servers.end(); it++){
				(*it).setServerID(i);
				empty_servers.insert(i);
				i++;
			}
		}
		if(alg == PI_K){
			this->k = k;
		}
	}

	
//*****************************************************************************************************
		// function name:	sendTasks	
		// Description:		Sends to srvPos amount number of tasks
		// Parameters:		int srvPos, int amount
		// Returns:		none
//*****************************************************************************************************
	void sendTasks(int srvPos, int amount);
	
	
//*****************************************************************************************************
		// function name:	doAlg	
		// Description:		Switch case on what algoritms to choose
		// Parameters:		none
		// Returns:		chosen algorithm
//*****************************************************************************************************
	int doAlg();
	
	
//*****************************************************************************************************
		// function name:	doJSQ	
		// Description:		Returns chosen server's id accourding to current algorithm
		// Parameters:		none
		// Returns:		chosen server's id
//*****************************************************************************************************
	int doJSQ();
	
	
//*****************************************************************************************************
		// function name:	doJIQ	
		// Description:		Returns chosen server's id accourding to current algorithm
		// Parameters:		none
		// Returns:		chosen server's id
//*****************************************************************************************************
	int doJIQ();
	
	
//*****************************************************************************************************
		// function name:	doPI	
		// Description:		Returns chosen server's id accourding to current algorithm
		// Parameters:		none
		// Returns:		chosen server's id
//*****************************************************************************************************
	int doPI();


//*****************************************************************************************************
		// function name:	doPI_K
		// Description:		Returns chosen server's id accourding to current algorithm
		// Parameters:		none
		// Returns:		chosen server's id
//*****************************************************************************************************
	int doPI_K();

	
//*****************************************************************************************************
		// function name:	print	
		// Description:		Prints LB_LogNormal status
		// Parameters:		none
		// Returns:		none
//*****************************************************************************************************
	void print();
	
	
//*****************************************************************************************************
		// function name:	completeTasks	
		// Description:		Complete tasks for all servers in current time slot
		// Parameters:		none
		// Returns:		none
//*****************************************************************************************************
	void completeTasks(int geomValues[]);
	
	
//*****************************************************************************************************
		// function name:	setHomogeneous	
		// Description:		Sets the value p for all servers
		// Parameters:		double p
		// Returns:		none
//*****************************************************************************************************
	void setHomogeneous(double p);
	
	
//*****************************************************************************************************
		// function name:	setHeterogeneous	
		// Description:		Sets nStrong servers with Pweak and the rest with Pstrong
		// Parameters:		double Pstrong, double Pweak, int nStrong
		// Returns:		none
//*****************************************************************************************************
	void setHeterogeneous(double Pstrong, double Pweak, int nStrong);
	
	
//*****************************************************************************************************
		// function name:	averageTasksInSystem	
		// Description:		Calculates the average number of tasks in the system
		// Parameters:		none
		// Returns:		Average tasks num in system
//*****************************************************************************************************
	int averageTasksInSystem();
	
	
//*****************************************************************************************************
		// function name:	clearServers	
		// Description:		writes the run date on output file
		// Parameters:		none
		// Returns:		none
//*****************************************************************************************************
	void clearServers();
	
	
//*****************************************************************************************************
		// function name:	getMinimalServerSize	
		// Description:		Checks which server has the smallest number of tasks, and returns its number
		// Parameters:		none
		// Returns:		Minimal server size
//*****************************************************************************************************
	int getMinimalServerSize();
	
	
//*****************************************************************************************************
		// function name:	updateMinimalServersSet	
		// Description:		Updates the set of minimal servers according to new minimum
		// Parameters:		int min
		// Returns:		none
//*****************************************************************************************************
	void updateMinimalServersSet(int min);
};



#endif //LOADBALANCER_LOADBALANCER_H
