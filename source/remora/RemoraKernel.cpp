/*
 * RemoraKernel.cpp
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#include <remora/RemoraKernel.h>
#include "configuration.h"
#include "extern.h"
#include "fsl_debug_console.h"


RemoraKernel::RemoraKernel() :
	RemoraThread::RemoraThread(1024, 1, 0, false)
{
	// TODO Auto-generated constructor stub
	tx_mutex_create(&this->mutexTx, "mutex tx", 1);
	tx_mutex_create(&this->mutexRx, "mutex rx", 1);
	this->network = new RemoraNetwork(&(this->mutexRx), &(this->mutexTx));
	this->config = new RemoraConfig();
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
	while (1)
	{
		switch(this->currentState)
	    {
		  case ST_SETUP:
			  // do setup tasks
			  if (currentState != prevState)
			  {
				  printf("\n## Entering SETUP state\n\n");
			  }
			  this->prevState = this->currentState;

			  this->config->jsonFromFlash();
			  this->config->deserialiseJSON();
			  this->config->configThreads();
			  //this->config->createThreads();
			  //debugThreadHigh();
			  this->config->loadModules(this->baseThread, this->servoThread, this->dmaControl);
			  //debugThreadLow();
			  //udpServer_init();
			  //IAP_tftpd_init(g_EDMA_Handle);
			  currentState = ST_START;
			  break;

		  case ST_START:
			  // do start tasks
			  if (currentState != prevState)
			  {
				  printf("\n## Entering START state\n");
			  }
			  prevState = currentState;

			  if (!threadsRunning)
			  {
				  //tx_thread_resume(&networking_thread);
				  //tx_thread_resume(&dma_thread);

				  // Start the threads
				  printf("\nStarting the BASE thread\n");
				  this->baseThread->startThread();

				  printf("\nStarting the SERVO thread\n");
				  this->servoThread->startThread();

				  //DMAconfig(); // put this in the right place+

				  threadsRunning = true;
			  }

			  currentState = ST_IDLE;

			  break;


		  case ST_IDLE:
			  // do something when idle
			  if (currentState != prevState)
			  {
				  printf("\n## Entering IDLE state\n");
			  }
			  prevState = currentState;

			  //wait for data before changing to running state
			  if (comms->getStatus())
			  {
				  currentState = ST_RUNNING;
			  }

			  break;

		  case ST_RUNNING:
			  // do running tasks
			  if (currentState != prevState)
			  {
				  printf("\n## Entering RUNNING state\n");
			  }
			  prevState = currentState;

			  if (comms->getStatus() == false)
			  {
				  currentState = ST_RESET;
			  }

			  break;

		  case ST_STOP:
			  // do stop tasks
			  if (currentState != prevState)
			  {
				  printf("\n## Entering STOP state\n");
			  }
			  prevState = currentState;


			  currentState = ST_STOP;
			  break;

		  case ST_RESET:
			  // do reset tasks
			  if (currentState != prevState)
			  {
				  printf("\n## Entering RESET state\n");
			  }
			  prevState = currentState;

			  // set all of the rxData buffer to 0
			  // rxData.rxBuffer is volatile so need to do this the long way. memset cannot be used for volatile
			  printf("   Resetting rxBuffer\n");
			  {
				  int n = sizeof(rxData.rxBuffer);
				  while(n-- > 0)
				  {
					  rxData.rxBuffer[n] = 0;
				  }
			  }

			  currentState = ST_IDLE;
			  break;

		  case ST_WDRESET:
			  // force a reset
			  //NVIC_SystemReset();
			  break;
	    }

		//EthernetTasks();
		if (this->config->newJson)
		{
			printf("\n\nChecking new configuration file\n");

			if (this->config->checkJson() > 0)
			{
				printf("Moving new configuration file to Flash storage and reset\n");
				this->config->moveJson();
				NVIC_SystemReset();
			}
		}

		this->network->Accept();
		tx_thread_sleep(1);
	}
}

void RemoraKernel::Run() {
	this->startThread();
}

RemoraKernel::~RemoraKernel() {
	// TODO Auto-generated destructor stub
}

