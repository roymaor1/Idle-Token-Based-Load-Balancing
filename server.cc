//
// Created by Roy on 28/10/2018.
//

#include "server.h"
#include <iostream>

void Server::pushTask(Task t){
	this->Queue.push(t);
}

void Server::print() {
	cout << this->id << ": " << this->p << endl;
	/*
	queue<Task> printQ = Queue;
	int i = 0;
	while (!printQ.empty()) {
		std::cout << "Element number " << i << ": " << printQ.front().getId() << std::endl;
		printQ.pop();
		i++;
	}
	 */
}

void Server::setServerP(double p){
	this->p = p;
}

void Server::setServerID(int id){
	this->id = id;
}

int Server::getServerID(){
	return this->id ;
}

int Server::getServerSize(){
	return (int)Queue.size();
}

void Server::completeTasks(int completed){
	int tasksToComplete = completed;
	//cout << "completed tasks: " << completed << endl;
	int size = this->getServerSize();
	if(tasksToComplete > size) // cant pop more than size of queue
		tasksToComplete = size;
	//cout << "server number " << this->getServerID() << ": popping " << completedTasks << " out of " << size << endl;
	for (int i = 0; i < tasksToComplete; i++){
		//delete Queue.front();
		Queue.pop();
	}
}

void Server::clear(){
	queue<Task> empty;
	swap(Queue,empty);
	/*
	int n = this->Queue.size();
	for (int i=0; i<n; i++){
		delete Queue.front();
		Queue.pop();
	}
	 */
}