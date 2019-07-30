/*
 * Task.h
 *
 *  Created on: Jul 23, 2019
 *      Author: xiaoba-8
 */

#ifndef TASK_H_
#define TASK_H_

class Task {
public:

	virtual void Run() = 0;
	virtual bool IsDeleteOnFinish() = 0;
};

#endif /* TASK_H_ */
