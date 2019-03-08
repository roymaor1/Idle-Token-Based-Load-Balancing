//
// Created by Roy on 28/10/2018.
//

#ifndef LOADBALANCER_TASK_H
#define LOADBALANCER_TASK_H
#include <random>
#include "constants.h"

class Task {
private:
	int id;
public:
	Task (int id_num){
		this->id = id_num;
	}
	int getId(){return this->id;};
};


#endif //LOADBALANCER_TASK_H
