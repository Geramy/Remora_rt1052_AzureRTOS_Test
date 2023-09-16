/*
 * RemoraKernel.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */
#pragma once
#ifndef REMORA_REMORAKERNEL_H_
#define REMORA_REMORAKERNEL_H_

#include "tx_api.h"

#include "thread/RemoraThread.h"
#include "thread/pruThread.h"

#include "drivers/network/networking.h"

// modules
#include "modules/module.h"
#include "modules/blink/blink.h"
//#include "modules/debug/debug.h"
#include "modules/DMAstepgen/DMAstepgen.h"
#include "modules/encoder/encoder.h"
#include "modules/comms/RemoraComms.h"
#include "modules/pwm/spindlePWM.h"
#include "modules/stepgen/stepgen.h"
#include "modules/digitalPin/digitalPin.h"
#include "modules/nvmpg/nvmpg.h"

#include "remora/RemoraStepGenDMA.h"
#include "remora/RemoraNetwork.h"
#include "remora/RemoraConfig.h"

//#include "extern.h"
#define DMA_THREAD_STACK_SIZE 1768
#define DMA_THREAD_PRIORITY   3

class RemoraKernel  : public RemoraThread {
private:
	enum KernelState {
		ST_SETUP = 0,
		ST_START,
		ST_IDLE,
		ST_RUNNING,
		ST_STOP,
		ST_RESET,
		ST_WDRESET
	};
private:
	RemoraConfig *config;
	RemoraNetwork *network;
	pruThread *baseThreadPtr;
	pruThread *servoThreadPtr;

	TX_THREAD dma_thread;
	ULONG dma_thread_stack[DMA_THREAD_STACK_SIZE / sizeof(ULONG)];

	TX_MUTEX mutexRx;
	TX_MUTEX mutexTx;

	RemoraStepGenDMA *dmaControl;
	// boolean
	bool PRUreset;
	bool configError = false;
	bool threadsRunning = false;

	// state machine
	enum KernelState prevState;
	enum KernelState currentState = ST_SETUP;
public:
	void RemoraThreadEntry();
	RemoraKernel();
	void InitializeThreads();
	void Run();
	virtual ~RemoraKernel();
};

#endif /* REMORA_REMORAKERNEL_H_ */
