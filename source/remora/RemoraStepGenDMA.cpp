/*
 * RemoraDMA.cpp
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#include <remora/RemoraStepGenDMA.h>
#include "nx_api.h"
#include "modules/DMAstepgen/DMAstepgen.h"

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
	RemoraStepGenDMA* $this = reinterpret_cast<RemoraStepGenDMA*>(param);
	tx_semaphore_put(&$this->semaphoreSG);
}

RemoraStepGenDMA::RemoraStepGenDMA(uint8_t Freq, uint8_t dmaChannel, TX_MUTEX *rxMutex) {
	/* Configure DMAMUX */
	this->timerFreq = Freq;
	this->dmaChannel = dmaChannel;
	this->rxMutexPtr = rxMutex;
	tx_semaphore_create(&this->semaphoreSG, "semaphore dma stepgen", 1);
	//status = tx_queue_create(&my_queue, "my_queue", TX_4_ULONG, (VOID *) 0x300000, 2000):
	//tx_semaphore_create(&semaphore_0, "semaphore 0", 1);
	//_txe_semaphore_create(TX_SEMAPHORE *semaphore_ptr, CHAR *name_ptr, ULONG initial_count, UINT semaphore_control_block_size);
}

void RemoraStepGenDMA::InitializePIT(pit_chnl_t pitChannel) {
	this->pitChannel = pitChannel;
	// TODO Auto-generated constructor stub
	CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
	CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT, &pitConfig);

	// PIT channel 0
	PIT_SetTimerPeriod(PIT, this->pitChannel, CLOCK_GetFreq(kCLOCK_OscClk)/(2 * this->timerFreq));
	//PIT_StartTimer(PIT, this->pitChannel);
	this->PITMode = true;
}

void RemoraStepGenDMA::InitializeHardware() {
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

void RemoraStepGenDMA::SetupBuffers(bool doubleBuffer) {

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

void RemoraStepGenDMA::ResumeDMA() {
	PIT_StartTimer(PIT, this->pitChannel);
	DMAMUX_EnableChannel(DMAMUX, this->dmaChannel);
	DMAMUX_EnablePeriodTrigger(DMAMUX, this->dmaChannel);

	EDMA_StartTransfer(&this->g_EDMA_Handle);
}

void RemoraStepGenDMA::StopDMA() {
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
	tx_semaphore_get(&this->semaphoreSG, TX_WAIT_FOREVER);
	tx_mutex_get(this->rxMutexPtr, TX_WAIT_FOREVER);
	this->ClearBuffers();
	this->SwapBuffers();
	// switch buffers

	// prepare the next DMA buffer
	bool filledBuffers = false;
	for (this->iterDMA = this->vDMAthread.begin(); this->iterDMA != this->vDMAthread.end(); ++this->iterDMA)
	{
		(*this->iterDMA)->runModule();
		if(((DMAstepgen*)&(*this->iterDMA))->dmaFilled) {
			filledBuffers = true;
		}
	}
	if(!filledBuffers) {
		tx_semaphore_put(&this->semaphoreSG);
	}

	//this->g_transferDone = false;
	tx_mutex_put(this->rxMutexPtr);
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
