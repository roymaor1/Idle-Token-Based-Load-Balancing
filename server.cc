#include "server.h"
#include <iostream>

void Server::pushTask(Task t){
	this->Queue.push(t);
}

void Server::print() {
	cout << this->id << ": " << this->p << endl;
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
	int size = this->getServerSize();
	if(tasksToComplete > size) // cant pop more than size of queue
		tasksToComplete = size;
	for (int i = 0; i < tasksToComplete; i++){
		Queue.pop();
	}
}

void Server::clear(){
	queue<Task> empty;
	swap(Queue,empty);
}

//
