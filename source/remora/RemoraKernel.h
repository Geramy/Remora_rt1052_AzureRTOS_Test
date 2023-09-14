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
//#include "extern.h"


class RemoraKernel  : public RemoraThread {
private:
	RemoraNetwork *network;
	pruThread *baseThread;
	pruThread *servoThread;
	pruThread *dmaThread;

	TX_MUTEX mutexRx;
	TX_MUTEX mutexTx;
	// state machine
	enum KernelState {
	    ST_SETUP = 0,
	    ST_START,
	    ST_IDLE,
	    ST_RUNNING,
	    ST_STOP,
	    ST_RESET,
	    ST_WDRESET
	};
public:
	void RemoraThreadEntry();
	RemoraKernel();
	void InitializeThreads();
	void Run();
	virtual ~RemoraKernel();
};

#endif /* REMORA_REMORAKERNEL_H_ */
