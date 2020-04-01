//
// Created by Roy on 28/10/2018.
//

#ifndef LOADBALANCER_SERVER_H
#define LOADBALANCER_SERVER_H


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
//*****************************************************************************************************
		// function name:	pushTask	
		// Description:		Push given task to a server's queue
		// Parameters:		Task t
		// Returns:		none
//*****************************************************************************************************
	void pushTask(Task t);
	
//*****************************************************************************************************
		// function name:	getServerSize	
		// Description:		Given a server, calculates its number of tasks 
		// Parameters:		none
		// Returns:		The number of tasks in given server
//*****************************************************************************************************
	int getServerSize();
	
//*****************************************************************************************************
		// function name:	print	
		// Description:		Prints server's status
		// Parameters:		none
		// Returns:		none
//*****************************************************************************************************
	void print();
	
//*****************************************************************************************************
		// function name:	setServerP	
		// Description:		Given geometric distribution value p, sets the server's p to it
		// Parameters:		double p
		// Returns:		none
//*****************************************************************************************************
	void setServerP(double p);
	
//*****************************************************************************************************
		// function name:	setServerID	
		// Description:		Given identity number id, sets the server's id to it
		// Parameters:		int id
		// Returns:		none
//*****************************************************************************************************
	void setServerID(int id);
	
//*****************************************************************************************************
		// function name:	getServerID	
		// Description:		returns server's id
		// Parameters:		none
		// Returns:		Server's id
//*****************************************************************************************************
	int getServerID();
	
//*****************************************************************************************************
		// function name:	completeTasks	
		// Description:		Deleting a task when finished.  
		// Parameters:		The number of tasks to be finished
		// Returns:		none
//*****************************************************************************************************
	void completeTasks(int completed);
	
//*****************************************************************************************************
		// function name:	clear	
		// Description:		Cleaning the server's queues forward to new run of algorithm
		// Parameters:		none
		// Returns:		none
//*****************************************************************************************************
	void clear();
};


#endif //LOADBALANCER_SERVER_H
//
