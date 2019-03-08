//
// Created by Roy & Rachely on 28/10/2018.
//

#ifndef LOADBALANCER_LOADBALANCER_H
#define LOADBALANCER_LOADBALANCER_H
#include "server.h"
#include <set>

extern int nextIdTask;

enum LB_Type { JSQ, JIQ, PI };


class loadBalancer {

private:
	LB_Type alg;
	vector<Server> servers;
	set<int> minimal_servers; // JSQ
	set<int> empty_servers; // JIQ, PI
	int lastIdle; // PI
public:
	loadBalancer(LB_Type t, int size){
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
		else{ // JIQ, PI
			for(vector<Server>::iterator it = servers.begin(); it!=servers.end(); it++){
				(*it).setServerID(i);
				empty_servers.insert(i);
				i++;
			}
		}
	}

	void sendTasks(int srvPos, int amount);
	int doAlg();
	int doJSQ();
	int doJIQ();
	int doPI();
	void print();
	void completeTasks(int geomValues[]);
	void setHomogeneous(double p);
	void setHeterogeneous();
	int averageTasksInSystem();
	void clearServers();
	int getMinimalServerSize();
	void updateMinimalServersSet(int min);
};



#endif //LOADBALANCER_LOADBALANCER_H
