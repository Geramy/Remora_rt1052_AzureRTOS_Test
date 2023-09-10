/*
 * RemoraThread.cpp
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#include <thread/RemoraThread.h>
#include "../modules/module.h"


using namespace std;

static void thread_entry(ULONG param)
{
    RemoraThread *that = (RemoraThread*)param;
    that->RemoraThreadEntry();
}

RemoraThread::RemoraThread(UINT THREAD_STACK_SIZE, UINT THREAD_PRIORITY, UINT frequency, bool AutoStart) {
	// TODO Auto-generated constructor stub
	this->THREAD_STACK_SIZE = THREAD_STACK_SIZE;
	this->THREAD_PRIORITY = THREAD_PRIORITY;
	this->thread_stack = new ULONG[this->THREAD_STACK_SIZE / sizeof(ULONG)];
	UINT threadStatus = tx_thread_create(&this->threadHandle,
	        "thread",
			thread_entry,
	        (ULONG)(RemoraThread*)(this),
			this->thread_stack,
			this->THREAD_STACK_SIZE,
			this->THREAD_PRIORITY,
			this->THREAD_PRIORITY,
	        TX_NO_TIME_SLICE,
			(AutoStart ? TX_AUTO_START : TX_DONT_START));
}

void RemoraThread::RemoraThreadEntry() {
	while(1) {
		this->run();
	}
}

void RemoraThread::startThread(void)
{
	//TimerPtr = new pruTimer(this->timer, this->irq, this->frequency, this);
	tx_thread_resume(&this->threadHandle);
}

void RemoraThread::stopThread(void)
{
	tx_thread_suspend(&this->threadHandle);
}


void RemoraThread::registerModule(Module* module)
{
	this->vThread.push_back(module);
}


void RemoraThread::registerModulePost(Module* module)
{
	this->vThreadPost.push_back(module);
	this->hasThreadPost = true;
}


void RemoraThread::run(void)
{
	// iterate over the Thread pointer vector to run all instances of Module::runModule()
	for (iter = vThread.begin(); iter != vThread.end(); ++iter) (*iter)->runModule();

	// iterate over the second vector that contains module pointers to run after (post) the main vector

	if (hasThreadPost)
	{
		for (iter = vThreadPost.begin(); iter != vThreadPost.end(); ++iter) (*iter)->runModulePost();
	}
}


RemoraThread::~RemoraThread() {
	// TODO Auto-generated destructor stub
}

