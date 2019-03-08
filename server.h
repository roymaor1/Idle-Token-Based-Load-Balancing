//
// Created by Roy on 28/10/2018.
//

#ifndef LOADBALANCER_SERVER_H
#define LOADBALANCER_SERVER_H

using namespace std;

#include "task.h"

#include <queue>
#include <vector>
#include <list>
#include "distribution.h"
#include <functional>

class Server {
private:
	queue<Task> Queue;
	double p; // geometric distribution
	int id;
public:
	void pushTask(Task t);
	int getServerSize();
	void print();
	void setServerP(double p);
	void setServerID(int id);
	int getServerID();
	void completeTasks(int completed);
	void clear();
};


#endif //LOADBALANCER_SERVER_H
