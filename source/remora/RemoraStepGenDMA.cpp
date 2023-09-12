/*
 * RemoraDMA.cpp
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#include <remora/RemoraStepGenDMA.h>

using namespace std;

extern int32_t stepgenDMAbuffer_0[DMA_BUFFER_SIZE];
extern int32_t stepgenDMAbuffer_1[DMA_BUFFER_SIZE];

// DMA stepgen double buffers
AT_NONCACHEABLE_SECTION_INIT(int32_t stepgenDMAbuffer_0[DMA_BUFFER_SIZE]);		// double buffers for port DMA transfers
AT_NONCACHEABLE_SECTION_INIT(int32_t stepgenDMAbuffer_1[DMA_BUFFER_SIZE]);

AT_QUICKACCESS_SECTION_DATA_ALIGN(edma_tcd_t tcdMemoryPoolPtr[3], sizeof(edma_tcd_t));
// DMA stepgen double buffers

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

uint8_t RemoraStepGenDMA::ResumeDMA() {
	PIT_StartTimer(PIT, this->pitChannel);
	DMAMUX_EnableChannel(DMAMUX, this->dmaChannel);
	DMAMUX_EnablePeriodTrigger(DMAMUX, this->dmaChannel);

	EDMA_StartTransfer(&this->g_EDMA_Handle);
}

uint8_t RemoraStepGenDMA::StopDMA() {
	PIT_StopTimer(PIT, this->pitChannel);
	EDMA_StopTransfer(&this->g_EDMA_Handle);
	DMAMUX_DisablePeriodTrigger(DMAMUX, this->dmaChannel);
	DMAMUX_DisableChannel(DMAMUX, this->dmaChannel);
}

int32_t& RemoraStepGenDMA::GetBufferAddress(bool BufferOne, int position) {
	if(BufferOne)
		return stepgenDMAbuffer_0[position];
	else
		return stepgenDMAbuffer_1[position];
}

void RemoraStepGenDMA::AddModule(Module* module) {
	this->vDMAthread.push_back(module);
}

void RemoraStepGenDMA::RunTasks() {
	if (this->g_transferDone)
	{
		this->ClearBuffers();
		this->SwapBuffers();
		// switch buffers

		// prepare the next DMA buffer
		for (this->iterDMA = this->vDMAthread.begin(); this->iterDMA != this->vDMAthread.end(); ++this->iterDMA) (*this->iterDMA)->runModule();

		this->g_transferDone = false;
	}
}

void RemoraStepGenDMA::ClearBuffers() {
	if (this->stepgenDMAbuffer)
	{
		memset(stepgenDMAbuffer_0, 0, sizeof(stepgenDMAbuffer_0));
	}
	else
	{
		memset(stepgenDMAbuffer_1, 0, sizeof(stepgenDMAbuffer_1));
	}
}

void RemoraStepGenDMA::SwapBuffers() {
	this->stepgenDMAbuffer = !this->stepgenDMAbuffer;
}

RemoraStepGenDMA::~RemoraStepGenDMA() {
	// TODO Auto-generated destructor stub
}
