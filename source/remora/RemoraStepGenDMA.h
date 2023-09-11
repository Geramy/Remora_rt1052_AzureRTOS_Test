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

class RemoraStepGenDMA {
private:

	edma_transfer_config_t transferConfig;
	edma_config_t userConfig;
	pit_config_t pitConfig;

	pit_chnl_t pitChannel;
	uint32_t timerFreq;
	uint32_t dmaChannel;
	bool PITMode = false;

	// DMA stepgen double buffers


	bool stepgenDMAbuffer = false;					// indicates which double buffer to use 0 or 1
	edma_handle_t g_EDMA_Handle;
public:
	volatile bool g_transferDone = false;
public:

	enum RDMAStatus {
		Success,
		Failure
	};

	RemoraStepGenDMA(uint32_t, uint32_t);
	uint8_t InitializePIT(pit_chnl_t);
	uint8_t InitializeHardware();
	uint8_t SetupBuffers(bool, uint16_t);
	uint8_t StartDMA();
	uint8_t StopDMA();
	virtual ~RemoraStepGenDMA();
};

#endif /* REMORA_REMORASTEPGENDMA_H_ */
