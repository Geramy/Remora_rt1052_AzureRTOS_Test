/*
 * RemoraDMA.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#ifndef REMORA_REMORASTEPGENDMA_H_
#define REMORA_REMORASTEPGENDMA_H_

#include "fsl_adapter_uart.h"
#include "configuration.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_pit.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include <vector>
#include "modules/module.h"

using namespace std;

class RemoraStepGenDMA {
private:

	edma_transfer_config_t transferConfig;
	edma_config_t userConfig;
	pit_config_t pitConfig;

	pit_chnl_t pitChannel;
	uint8_t timerFreq;
	uint8_t dmaChannel;
	bool PITMode = false;

	vector<Module*> vDMAthread;
	vector<Module*>::iterator iterDMA;
	bool DMAstepgen = false;

	edma_handle_t g_EDMA_Handle;
private:
	void ClearBuffers();
	void SwapBuffers();
public:
	volatile bool g_transferDone = false;
	bool stepgenDMAbuffer = false;					// indicates which double buffer to use 0 or 1
public:

	RemoraStepGenDMA(uint8_t, uint8_t);
	void InitializePIT(pit_chnl_t);
	void InitializeHardware();
	void SetupBuffers(bool, uint8_t);
	void ResumeDMA();
	void StopDMA();
	int32_t& GetBufferAddress(bool BufferOne, int position);
	void AddModule(Module*);
	void RunTasks();

	virtual ~RemoraStepGenDMA();
};

#endif /* REMORA_REMORASTEPGENDMA_H_ */
