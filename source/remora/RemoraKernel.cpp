/*
 * RemoraKernel.cpp
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#include <remora/RemoraKernel.h>
#include "configuration.h"
#include "extern.h"

RemoraKernel::RemoraKernel() :
	RemoraThread::RemoraThread(1024, 1, 0, false)
{
	// TODO Auto-generated constructor stub
	tx_mutex_create(&this->mutexTx, "mutex tx", 1);
	tx_mutex_create(&this->mutexRx, "mutex rx", 1);
	this->network = new RemoraNetwork(&(this->mutexRx), &(this->mutexTx));
}

void RemoraKernel::InitializeThreads()
{
	//Networking has to be done on the control thread, because networking has to be done inside a thread.
	this->baseThread = new pruThread(base_freq);
	this->servoThread = new pruThread(servo_freq);
	this->dmaThread = new pruThread(DMA_FREQ);
}

void RemoraKernel::RemoraThreadEntry() {
	//Initialize kernel stuff here
	this->network->Initialize();
	while (1) {

		this->network->Accept();
		tx_thread_sleep(1);
	}
}

void RemoraKernel::Run() {

}

RemoraKernel::~RemoraKernel() {
	// TODO Auto-generated destructor stub
}

