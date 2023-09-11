/*
 * RemoraKernel.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#ifndef REMORA_REMORAKERNEL_H_
#define REMORA_REMORAKERNEL_H_

class RemoraKernel {
private:
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
	RemoraKernel();
	void InitializeThreads();
	void Run();
	virtual ~RemoraKernel();
};

#endif /* REMORA_REMORAKERNEL_H_ */
