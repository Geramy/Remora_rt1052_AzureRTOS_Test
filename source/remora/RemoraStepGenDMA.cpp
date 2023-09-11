/*
 * RemoraDMA.cpp
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#include <remora/RemoraStepGenDMA.h>
#include "extern.h"
using namespace std;
/*CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
	CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT, &pitConfig);

	// PIT channel 0
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, CLOCK_GetFreq(kCLOCK_OscClk)/(2 * DMA_FREQ));
	PIT_StartTimer(PIT, kPIT_Chnl_0);
*/

static void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
	RemoraStepGenDMA $this = *(RemoraStepGenDMA*)param;
	$this.g_transferDone = true;
}

RemoraStepGenDMA::RemoraStepGenDMA(uint32_t Freq, uint32_t dmaChannel) {
	/* Configure DMAMUX */
	this->timerFreq = Freq;
	this->dmaChannel = dmaChannel;
}

uint8_t RemoraStepGenDMA::InitializePIT(pit_chnl_t pitChannel) {
	this->pitChannel = pitChannel;
	// TODO Auto-generated constructor stub
	CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
	CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT, &pitConfig);

	// PIT channel 0
	PIT_SetTimerPeriod(PIT, this->pitChannel, CLOCK_GetFreq(kCLOCK_OscClk)/(2 * this->timerFreq));
	PIT_StartTimer(PIT, this->pitChannel);
	this->PITMode = true;
}

uint8_t RemoraStepGenDMA::InitializeHardware() {
	DMAMUX_Init(DMAMUX);
	if(this->PITMode) {
		DMAMUX_EnableAlwaysOn(DMAMUX, this->dmaChannel, true);
		DMAMUX_EnableChannel(DMAMUX, this->dmaChannel);
		DMAMUX_EnablePeriodTrigger(DMAMUX, this->dmaChannel);
	}
	else {
		//DMAMUX_EnableAlwaysOn(DMAMUX, this->dmaChannel, true);
		DMAMUX_EnableChannel(DMAMUX, this->dmaChannel);
		//DMAMUX_EnablePeriodTrigger(DMAMUX, this->dmaChannel);
	}


	EDMA_GetDefaultConfig(&userConfig);
	EDMA_Init(DMA0, &userConfig);
	EDMA_CreateHandle(&g_EDMA_Handle, DMA0, this->dmaChannel);
	EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, (void*)this);
	EDMA_ResetChannel(g_EDMA_Handle.base, g_EDMA_Handle.channel);
}

uint8_t RemoraStepGenDMA::SetupBuffers(bool doubleBuffer, uint16_t size) {

	if(doubleBuffer) {
		/* prepare descriptor 0 */
			EDMA_PrepareTransfer(&transferConfig, stepgenDMAbuffer_0, sizeof(stepgenDMAbuffer_0[0]), (void*)GPIO1->DR_TOGGLE, sizeof(GPIO1->DR_TOGGLE),
								 sizeof(stepgenDMAbuffer_0[0]),
								 sizeof(stepgenDMAbuffer_0[0]) * DMA_BUFFER_SIZE,
								 kEDMA_MemoryToPeripheral);

			EDMA_TcdSetTransferConfig(tcdMemoryPoolPtr, &transferConfig, &tcdMemoryPoolPtr[1]);
			EDMA_TcdEnableInterrupts(&tcdMemoryPoolPtr[0], kEDMA_MajorInterruptEnable);

			/* prepare descriptor 1 */
			EDMA_PrepareTransfer(&transferConfig, stepgenDMAbuffer_1, sizeof(stepgenDMAbuffer_1[0]), (void*)GPIO1->DR_TOGGLE, sizeof(GPIO1->DR_TOGGLE),
								 sizeof(stepgenDMAbuffer_1[0]),
								 sizeof(stepgenDMAbuffer_1[0]) * DMA_BUFFER_SIZE,
								 kEDMA_MemoryToPeripheral);

			EDMA_TcdSetTransferConfig(&tcdMemoryPoolPtr[1], &transferConfig, &tcdMemoryPoolPtr[0]);
			EDMA_TcdEnableInterrupts(&tcdMemoryPoolPtr[1], kEDMA_MajorInterruptEnable);
	}
	else {
		/* prepare descriptor 0 */
			EDMA_PrepareTransfer(&transferConfig, stepgenDMAbuffer_0, sizeof(stepgenDMAbuffer_0[0]), (void*)GPIO1->DR_TOGGLE, sizeof(GPIO1->DR_TOGGLE),
								 sizeof(stepgenDMAbuffer_0[0]),
								 sizeof(stepgenDMAbuffer_0[0]) * DMA_BUFFER_SIZE,
								 kEDMA_MemoryToPeripheral);

			EDMA_TcdSetTransferConfig(tcdMemoryPoolPtr, &transferConfig, &tcdMemoryPoolPtr[1]);
			EDMA_TcdEnableInterrupts(&tcdMemoryPoolPtr[0], kEDMA_MajorInterruptEnable);
	}


	EDMA_InstallTCD(DMA0, this->dmaChannel, tcdMemoryPoolPtr);
}

uint8_t RemoraStepGenDMA::StartDMA() {

}

uint8_t RemoraStepGenDMA::StopDMA() {
	EDMA_StopTransfer(&this->g_EDMA_Handle);
	EDMA_ResetChannel(this->g_EDMA_Handle.base, this->g_EDMA_Handle.channel);
	EDMA_Deinit(DMA0);

	DMAMUX_DisableChannel(DMAMUX, this->dmaChannel);
	DMAMUX_Deinit(DMAMUX);
}

RemoraStepGenDMA::~RemoraStepGenDMA() {
	// TODO Auto-generated destructor stub
}

/*
 * void DMAconfig(void)
{
	// Use the Periodic Interrupt Timer to trigger the DMA transfer to the GPIO DR_TOGGLE register

	edma_transfer_config_t transferConfig;
	edma_config_t userConfig;
	pit_config_t pitConfig;

	// The Periodic Interrupt Timer (PIT) module
	CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
	CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT, &pitConfig);

	// PIT channel 0
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, CLOCK_GetFreq(kCLOCK_OscClk)/(2 * DMA_FREQ));
	PIT_StartTimer(PIT, kPIT_Chnl_0);

	/* Configure DMAMUX
	DMAMUX_Init(DMAMUX);
	DMAMUX_EnableAlwaysOn(DMAMUX, 0, true);
	DMAMUX_EnableChannel(DMAMUX, 0);

	DMAMUX_EnablePeriodTrigger(DMAMUX, 0);

	EDMA_GetDefaultConfig(&userConfig);
	EDMA_Init(DMA0, &userConfig);
	EDMA_CreateHandle(&g_EDMA_Handle, DMA0, 0);
	EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
	EDMA_ResetChannel(g_EDMA_Handle.base, g_EDMA_Handle.channel);

	/* prepare descriptor 0
	EDMA_PrepareTransfer(&transferConfig, stepgenDMAbuffer_0, sizeof(stepgenDMAbuffer_0[0]), &GPIO1->DR_TOGGLE, sizeof(GPIO1->DR_TOGGLE),
						 sizeof(stepgenDMAbuffer_0[0]),
						 sizeof(stepgenDMAbuffer_0[0]) * DMA_BUFFER_SIZE,
						 kEDMA_MemoryToPeripheral);
	EDMA_TcdSetTransferConfig(tcdMemoryPoolPtr, &transferConfig, &tcdMemoryPoolPtr[1]);
	EDMA_TcdEnableInterrupts(&tcdMemoryPoolPtr[0], kEDMA_MajorInterruptEnable);

	/* prepare descriptor 1
	EDMA_PrepareTransfer(&transferConfig, stepgenDMAbuffer_1, sizeof(stepgenDMAbuffer_1[0]), &GPIO1->DR_TOGGLE, sizeof(GPIO1->DR_TOGGLE),
						 sizeof(stepgenDMAbuffer_1[0]),
						 sizeof(stepgenDMAbuffer_1[0]) * DMA_BUFFER_SIZE,
						 kEDMA_MemoryToPeripheral);
	EDMA_TcdSetTransferConfig(&tcdMemoryPoolPtr[1], &transferConfig, &tcdMemoryPoolPtr[0]);
	EDMA_TcdEnableInterrupts(&tcdMemoryPoolPtr[1], kEDMA_MajorInterruptEnable);

	EDMA_InstallTCD(DMA0, 0, tcdMemoryPoolPtr);

	EDMA_StartTransfer(&g_EDMA_Handle);
}
 */
