/*
 * RemoraKernel.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#ifndef REMORA_REMORAKERNEL_H_
#define REMORA_REMORAKERNEL_H_

#include "tx_api.h"
#include "thread/RemoraThread.h"
#include "thread/pruThread.h"
#include "drivers/network/networking.h"
#include "modules/DMAstepgen/DMAstepgen.h"
#include "remora/RemoraStepGenDMA.h"
#include "modules/stepgen/stepgen.h"
#include "remora/RemoraNetwork.h"
#include "remora/RemoraConfig.h"

//#include "extern.h"
#define DMA_THREAD_STACK_SIZE 2048
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
	pruThread *baseThread;
	pruThread *servoThread;
	pruThread *dmaThread;

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
