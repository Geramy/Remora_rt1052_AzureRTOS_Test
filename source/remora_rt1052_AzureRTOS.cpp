/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    remora_rt1052_AzureRTOS.cpp
 * @brief   Application entry point.
 */
#define NX_WEB_HTTP_NO_FILEX
#include "remora.h"
#include <stdio.h>

#include "tx_api.h"
#include "../rtos/netxduo/c/inc/nx_api.h"
#include "drivers/nx_driver/nx_driver_imxrt10xx.h"


#include "configuration.h"
#include "board.h"
#include "pin_mux.h"
#include "extern.h"
#include "flexspi_nor_flash.h"
#include "remora/RemoraKernel.h"

/*#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1052.h"
#include "fsl_debug_console.h"
#include "board_init.h"
#include "remora/RemoraStepGenDMA.h"*/

/* TODO: insert other include files here. */

/* Remora Original Data */

/*#include "tftpserver.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_pit.h"
#include "flexspi_nor_flash.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"

#include "configuration.h"

#include "crc32.h"

// libraries
#include <sys/errno.h>
#include "lib/ArduinoJson6/ArduinoJson.h"

// drivers
#include "drivers/pin/pin.h"

// interrupts
#include "interrupt/interrupt.h"

// threads
#include "thread/pruThread.h"

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

#include "drivers/nx_driver/nx_driver_imxrt10xx.h"

#include "drivers/network/networking.h"
#include "remora/RemoraKernel.h"

// state machine
enum State {
    ST_SETUP = 0,
    ST_START,
    ST_IDLE,
    ST_RUNNING,
    ST_STOP,
    ST_RESET,
    ST_WDRESET
};*/

uint8_t resetCnt;
uint32_t base_freq = PRU_BASEFREQ;
uint32_t servo_freq = PRU_SERVOFREQ;

pruThread* baseThread;
pruThread* servoThread;



// pointers to objects with global scope
RemoraComms* comms;
Module* MPG;

// unions for RX, TX and MPG data
rxData_t rxBuffer;				// temporary RX buffer
volatile rxData_t rxData;
volatile txData_t txData;
volatile bool cmdReceived;
volatile bool mpgReceived;
mpgData_t mpgData;

// pointers to data
volatile rxData_t*  ptrRxData = &rxData;
volatile txData_t*  ptrTxData = &txData;
volatile int32_t* ptrTxHeader;
volatile bool*    ptrPRUreset;
volatile int32_t* ptrJointFreqCmd[JOINTS];
volatile int32_t* ptrJointFeedback[JOINTS];
volatile uint8_t* ptrJointEnable;
volatile float*   ptrSetPoint[VARIABLES];
volatile float*   ptrProcessVariable[VARIABLES];
volatile uint32_t* ptrInputs;
volatile uint32_t* ptrOutputs;
volatile uint16_t* ptrNVMPGInputs;
volatile mpgData_t* ptrMpgData = &mpgData;


void tx_application_define(void* first_unused_memory)
{
	NX_PARAMETER_NOT_USED(first_unused_memory);
	// Initialize the NetX system.
	nx_system_initialize();
	RemoraKernel *kernel = new RemoraKernel();
	kernel->InitializeThreads();
	kernel->Run();
}

int main(void)
{

	BOARD_ConfigMPU();
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
    // Initialize the board

    // Enter the ThreadX kernel
    tx_kernel_enter();

    return 0;
}

void ResetISR(void) {

   #if defined (__REDLIB__)
     __main() ;
  #else
     main();
  #endif
}
