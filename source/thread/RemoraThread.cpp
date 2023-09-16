/*
 * RemoraThread.cpp
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#include <thread/RemoraThread.h>

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
			&this->thread_stack,
			this->THREAD_STACK_SIZE,
			this->THREAD_PRIORITY,
			this->THREAD_PRIORITY,
	        TX_NO_TIME_SLICE,
			(AutoStart ? TX_AUTO_START : TX_DONT_START));
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

RemoraThread::~RemoraThread() {
	// TODO Auto-generated destructor stub
}

