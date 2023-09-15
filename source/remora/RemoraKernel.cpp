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

static void dma_thread_entry(ULONG parameter)
{
	RemoraStepGenDMA *dmaPtr = reinterpret_cast<RemoraStepGenDMA*>(parameter);
	while (1)
		dmaPtr->RunTasks();
}

RemoraKernel::RemoraKernel() :
	RemoraThread::RemoraThread(1024, 1, 0, false)
{
	// TODO Auto-generated constructor stub
	tx_mutex_create(&this->mutexTx, "mutex tx", 1);
	tx_mutex_create(&this->mutexRx, "mutex rx", 1);
	this->network = new RemoraNetwork(&(this->mutexRx), &(this->mutexTx));

	this->config = new RemoraConfig();
	this->dmaControl = new RemoraStepGenDMA(DMA_FREQ, 0, &(this->mutexRx));
	this->dmaControl->InitializePIT(kPIT_Chnl_0);
	this->dmaControl->InitializeHardware();
	this->dmaControl->SetupBuffers(true);

	UINT dmaThreadStatus = tx_thread_create(&this->dma_thread,
				"DMAstepgen Thread",
				dma_thread_entry,
				(ULONG)&(this->dmaControl),
				this->dma_thread_stack,
				DMA_THREAD_STACK_SIZE,
				DMA_THREAD_PRIORITY,
				DMA_THREAD_PRIORITY,
				TX_NO_TIME_SLICE,
				TX_DONT_START);

	if (dmaThreadStatus != TX_SUCCESS)
	{
		printf("DMA thread failed, please restart\r\n");
	}
}

void RemoraKernel::InitializeThreads()
{
	//Networking has to be done on the control thread, because networking has to be done inside a thread.
	this->baseThreadPtr = new pruThread(base_freq);
	this->servoThreadPtr = new pruThread(servo_freq);
	baseThread = this->baseThreadPtr;
	servoThread = this->servoThreadPtr;
	//this->dmaThread = new pruThread(DMA_FREQ);
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
			  this->config->loadModules(this->baseThreadPtr, this->servoThreadPtr, this->dmaControl);
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
				  this->baseThreadPtr->startThread();

				  printf("\nStarting the SERVO thread\n");
				  this->servoThreadPtr->startThread();
				  this->dmaControl->ResumeDMA();
				  tx_thread_resume(&(this->dma_thread));
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

		this->network->Accept();
		tx_thread_sleep(1);

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
	}
}

void RemoraKernel::Run() {
	this->startThread();
}

RemoraKernel::~RemoraKernel() {
	// TODO Auto-generated destructor stub
}

