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
#include "remora.h"

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

#include "drivers/network/networking.h"

#define NETWORKING_THREAD_STACK_SIZE 4096
#define NETWORKING_THREAD_PRIORITY   3

#define CONTROL_THREAD_STACK_SIZE 4096
#define CONTROL_THREAD_PRIORITY   4

#define DMA_THREAD_STACK_SIZE 4096
#define DMA_THREAD_PRIORITY   2

NX_UDP_SOCKET           socket_0;
NX_UDP_SOCKET           socket_1;

TX_THREAD networking_thread;
ULONG networking_thread_stack[NETWORKING_THREAD_STACK_SIZE / sizeof(ULONG)];

TX_THREAD control_thread;
ULONG control_thread_stack[CONTROL_THREAD_STACK_SIZE / sizeof(ULONG)];

TX_THREAD dma_thread;
ULONG dma_thread_stack[DMA_THREAD_STACK_SIZE / sizeof(ULONG)];

/* Remora Original Data */

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_pit.h"
//#include "flexspi_nor_flash.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"

#include "configuration.h"
#include "remora.h"

//#include "crc32.h"

// libraries
#include <sys/errno.h>
#include "lib/ArduinoJson6/ArduinoJson.h"

// drivers
#include "drivers/pin/pin.h"

// interrupts
#include "interrupt/irqHandlers.h"
#include "interrupt/interrupt.h"

// threads
#include "thread/pruThread.h"
#include "thread/createThreads.h"

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


// state machine
enum State {
    ST_SETUP = 0,
    ST_START,
    ST_IDLE,
    ST_RUNNING,
    ST_STOP,
    ST_RESET,
    ST_WDRESET
};

uint8_t resetCnt;
uint32_t base_freq = PRU_BASEFREQ;
uint32_t servo_freq = PRU_SERVOFREQ;

// boolean
volatile bool PRUreset;
bool configError = false;
bool threadsRunning = false;


// DMA stepgen double buffers
AT_NONCACHEABLE_SECTION_INIT(int32_t stepgenDMAbuffer_0[DMA_BUFFER_SIZE]);		// double buffers for port DMA transfers
AT_NONCACHEABLE_SECTION_INIT(int32_t stepgenDMAbuffer_1[DMA_BUFFER_SIZE]);
vector<Module*> vDMAthread;
vector<Module*>::iterator iterDMA;
bool DMAstepgen = false;
bool stepgenDMAbuffer = false;					// indicates which double buffer to use 0 or 1
edma_handle_t g_EDMA_Handle;
volatile bool g_transferDone = false;
AT_QUICKACCESS_SECTION_DATA_ALIGN(edma_tcd_t tcdMemoryPoolPtr[3], sizeof(edma_tcd_t));

// pointers to objects with global scope
pruThread* servoThread;
pruThread* baseThread;
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


// JSON config file stuff

const char defaultConfig[] = DEFAULT_CONFIG;


// 512 bytes of metadata in front of actual JSON file
typedef struct
{
  uint32_t crc32;   		// crc32 of JSON
  uint32_t length;			// length in words for CRC calculation
  uint32_t jsonLength;  	// length in of JSON config in bytes
  uint8_t padding[500];
} metadata_t;
#define METADATA_LEN    512

volatile bool newJson;
uint32_t crc32;
FILE *jsonFile;
string strJson;
DynamicJsonDocument doc(JSON_BUFF_SIZE);
JsonObject thread;
JsonObject module;
/* End of Remora Original Data */

static void networking_thread_entry(ULONG parameter)
{
    UINT status;
    short error_counter = 0;
    UINT thread_1_counter = 0;
    printf("Starting Networking Thread\r\n\r\n");

    // Initialize the network
    if ((status = network_init(nx_driver_imx)))
    {
        printf("ERROR: Failed to initialize the network (0x%08x)\r\n", status);
    }
    else {
    		UINT       status;
    		NX_PACKET *my_packet;

    	    NX_PARAMETER_NOT_USED(parameter);

    	    tx_thread_sleep(NX_IP_PERIODIC_RATE);
    	#ifndef NX_DISABLE_IPV6
    	    /* Wait 5 seconds for IPv6 stack to finish DAD process. */
    	    tx_thread_sleep(5 * NX_IP_PERIODIC_RATE);
    	#endif
    	    /* Create a UDP socket.  */
    	    status = nx_udp_socket_create(&nx_ip, &socket_1, "Socket 1", NX_IP_NORMAL, NX_FRAGMENT_OKAY, 0x80, 5);

    	    /* Check status.  */
    	    if (status)
    	    {
    	        error_counter++;
    	        return;
    	    }

    	    /* Bind the UDP socket to the IP port.  */
    	    status =  nx_udp_socket_bind(&socket_1, 27181, TX_WAIT_FOREVER);

    	    /* Check status.  */
    	    if (status)
    	    {
    	        error_counter++;
    	        return;
    	    }

    	    while (1)
    	    {


    	        /* Receive a UDP packet.  */
    	        status =  nx_udp_socket_receive(&socket_1, &my_packet, TX_WAIT_FOREVER);

    	        /* Check status.  */
    	        if (status != NX_SUCCESS)
    	        {
    	            break;
    	        }
    	        else if(status == NX_SUCCESS) {
    	        	udp_data_callback(&my_packet);
    	        }
    	        /* Release the packet.  */
    	        status =  nx_packet_release(my_packet);

    	        /* Check status.  */
    	        if (status != NX_SUCCESS)
    	        {
    	            break;
    	        }

    	        /* Increment thread 1's counter.  */
    	        thread_1_counter++;
    	    }
    }
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
