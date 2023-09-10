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
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1052.h"
#include "fsl_debug_console.h"
#include "board_init.h"
/* TODO: insert other include files here. */
#include <stdio.h>

#include "drivers/nx_driver/nx_driver_imxrt10xx.h"

#define NETWORKING_THREAD_STACK_SIZE 4096
#define NETWORKING_THREAD_PRIORITY   3

#define CONTROL_THREAD_STACK_SIZE 4096
#define CONTROL_THREAD_PRIORITY   4

#define DMA_THREAD_STACK_SIZE 4096
#define DMA_THREAD_PRIORITY   2

TX_THREAD networking_thread;
ULONG networking_thread_stack[NETWORKING_THREAD_STACK_SIZE / sizeof(ULONG)];

TX_THREAD control_thread;
ULONG control_thread_stack[CONTROL_THREAD_STACK_SIZE / sizeof(ULONG)];

TX_THREAD dma_thread;
ULONG dma_thread_stack[DMA_THREAD_STACK_SIZE / sizeof(ULONG)];

static void networking_thread_entry(ULONG parameter)
{
    UINT status;

    printf("Starting Networking Thread\r\n\r\n");

    // Initialize the network
    /*if ((status = network_init(nx_driver_imx)))
    {
        printf("ERROR: Failed to initialize the network (0x%08x)\r\n", status);
    }*/
}

static void control_thread_entry(ULONG parameter)
{
    printf("Starting Control Thread\r\n\r\n");

}

static void dma_thread_entry(ULONG parameter)
{
    printf("Starting DMA Thread\r\n\r\n");

}

void tx_application_define(void* first_unused_memory)
{
    // Create Azure thread
    UINT networkingThreadStatus = tx_thread_create(&networking_thread,
        "Network Thread",
		networking_thread_entry,
        0,
		networking_thread_stack,
		NETWORKING_THREAD_STACK_SIZE,
		NETWORKING_THREAD_PRIORITY,
		NETWORKING_THREAD_PRIORITY,
        TX_NO_TIME_SLICE,
        TX_AUTO_START);

    if (networkingThreadStatus != TX_SUCCESS)
    {
        printf("Azure IoT application failed, please restart\r\n");
    }

    UINT controlThreadStatus = tx_thread_create(&control_thread,
            "Control Thread",
    		control_thread_entry,
            0,
			control_thread_stack,
    		CONTROL_THREAD_STACK_SIZE,
			CONTROL_THREAD_PRIORITY,
			CONTROL_THREAD_PRIORITY,
            TX_NO_TIME_SLICE,
            TX_AUTO_START);

	if (controlThreadStatus != TX_SUCCESS)
	{
		printf("Control thread failed, please restart\r\n");
	}

	UINT dmaThreadStatus = tx_thread_create(&dma_thread,
	            "DMAstepgen Thread",
	    		dma_thread_entry,
	            0,
	    		dma_thread_stack,
				DMA_THREAD_STACK_SIZE,
	    		DMA_THREAD_PRIORITY,
	    		DMA_THREAD_PRIORITY,
	            TX_NO_TIME_SLICE,
	            TX_AUTO_START);

		if (dmaThreadStatus != TX_SUCCESS)
		{
			printf("DMA thread failed, please restart\r\n");
		}
}

int main(void)
{
    // Initialize the board
    board_init();

    // Enter the ThreadX kernel
    tx_kernel_enter();

    return 0;
}
