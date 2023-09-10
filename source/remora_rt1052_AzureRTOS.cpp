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

#define NETWORKING_THREAD_STACK_SIZE 2048
#define NETWORKING_THREAD_PRIORITY   3

#define CONTROL_THREAD_STACK_SIZE 1024
#define CONTROL_THREAD_PRIORITY   4

#define BASE_THREAD_STACK_SIZE 2048
#define BASE_THREAD_PRIORITY   3

#define SERVO_THREAD_STACK_SIZE 2048
#define SERVO_THREAD_PRIORITY   2

#define DMA_THREAD_STACK_SIZE 2048
#define DMA_THREAD_PRIORITY   2

NX_UDP_SOCKET           socketComms;
NX_PACKET_POOL poolComms;

NX_UDP_SOCKET           socketTftp;


TX_THREAD networking_thread;
ULONG networking_thread_stack[NETWORKING_THREAD_STACK_SIZE / sizeof(ULONG)];

TX_THREAD control_thread;
ULONG control_thread_stack[CONTROL_THREAD_STACK_SIZE / sizeof(ULONG)];

TX_THREAD dma_thread;
ULONG dma_thread_stack[DMA_THREAD_STACK_SIZE / sizeof(ULONG)];

TX_THREAD base_thread;
ULONG base_thread_stack[BASE_THREAD_STACK_SIZE / sizeof(ULONG)];

TX_THREAD servo_thread;
ULONG servo_thread_stack[SERVO_THREAD_STACK_SIZE / sizeof(ULONG)];

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


int8_t checkJson()
{
	metadata_t* meta = (metadata_t*)(XIP_BASE + JSON_UPLOAD_ADDRESS);
	uint32_t* json = (uint32_t*)(XIP_BASE + JSON_UPLOAD_ADDRESS + METADATA_LEN);

    uint32_t table[256];
    crc32::generate_table(table);
    int mod, padding;

	// Check length is reasonable
	if (meta->length > 2 * SECTOR_SIZE)
	{
		newJson = false;
		printf("JSON Config length incorrect\n");
		return -1;
	}

    // for compatibility with STM32 hardware CRC32, the config is padded to a 32 byte boundary
    mod = meta->jsonLength % 4;
    if (mod > 0)
    {
        padding = 4 - mod;
    }
    else
    {
        padding = 0;
    }
    printf("mod = %d, padding = %d\n", mod, padding);

	// Compute CRC
    crc32 = 0;
    char* ptr = (char *)(XIP_BASE + JSON_UPLOAD_ADDRESS + METADATA_LEN);
    for (int i = 0; i < meta->jsonLength + padding; i++)
    {
        crc32 = crc32::update(table, crc32, ptr, 1);
        ptr++;
    }

	printf("Length (words) = %d\n", meta->length);
	printf("JSON length (bytes) = %d\n", meta->jsonLength);
	printf("crc32 = %x\n", crc32);

	// Check CRC
	if (crc32 != meta->crc32)
	{
		newJson = false;
		printf("JSON Config file CRC incorrect\n");
		return -1;
	}

	// JSON is OK, don't check it again
	newJson = false;
	printf("JSON Config file received Ok\n");
	return 1;
}


void moveJson()
{
	uint8_t pages, sectors;
    uint32_t i = 0;
	metadata_t* meta = (metadata_t*)(XIP_BASE + JSON_UPLOAD_ADDRESS);
	uint32_t Flash_Write_Address;

	uint16_t jsonLength = meta->jsonLength;

    // how many pages are needed to be written. The first 4 bytes of the storage location will contain the length of the JSON file
    pages = (meta->jsonLength + 4) / FLASH_PAGE_SIZE;
    if ((meta->jsonLength + 4) % FLASH_PAGE_SIZE > 0)
    {
        pages++;
    }

    sectors = pages / 16; // 16 pages per sector
    if (pages % 16 > 0)
    {
    	sectors++;
    }

    printf("pages = %d, sectors = %d\n", pages, sectors);

    uint8_t data[pages * 256] = {0};

	// store the length of the file in the 0th word
    data[0] = (uint8_t)((jsonLength & 0x00FF));
    data[1] = (uint8_t)((jsonLength & 0xFF00) >> 8);

    //The buffer argument points to the data to be written, which is of size size.
    //This size must be a multiple of the "page size", which is defined as the constant FLASH_PAGE_SIZE, with a value of 256 bytes.

    for (i = 0; i < jsonLength; i++)
    {
        data[i + 4] = *((uint8_t*)(XIP_BASE + JSON_UPLOAD_ADDRESS + METADATA_LEN + i));
    }

	// erase the old JSON config file

	// init flash
	flexspi_nor_flash_init(FLEXSPI);

	// Enter quad mode
	status_t status = flexspi_nor_enable_quad_mode(FLEXSPI);
	if (status != kStatus_Success)
	{
	  return;
	}

	Flash_Write_Address = JSON_STORAGE_ADDRESS;

	for (i = 0; i < sectors; i++)
	{
		status = flexspi_nor_flash_erase_sector(FLEXSPI, Flash_Write_Address + (i * SECTOR_SIZE));
		if (status != kStatus_Success)
		{
		  PRINTF("Erase sector failure !\r\n");
		  return;
		}
	}

	for (i = 0; i < pages; i++)
	{
		status_t status = flexspi_nor_flash_page_program(FLEXSPI, Flash_Write_Address + i * FLASH_PAGE_SIZE, (uint32_t *)(data + i * FLASH_PAGE_SIZE));
		if (status != kStatus_Success)
		{
		 PRINTF("Page program failure !\r\n");
		 return;
		}
	}

	printf("Configuration file moved\n");

}


void jsonFromFlash(std::string json)
{
    int c;
    uint32_t i = 0;
    uint32_t jsonLength;


    printf("\n1. Loading JSON configuration file from Flash memory\n");

    // read word 0 to determine length to read
    jsonLength = *(uint32_t*)(XIP_BASE + JSON_STORAGE_ADDRESS);

    if (jsonLength == 0xFFFFFFFF)
    {
    	printf("Flash storage location is empty - no config file\n");
    	printf("Using basic default configuration - 3 step generators only\n");

    	jsonLength = sizeof(defaultConfig);

    	json.resize(jsonLength);

		for (i = 0; i < jsonLength; i++)
		{
			c = defaultConfig[i];
			strJson.push_back(c);
		}
    }
    else
    {
		json.resize(jsonLength);

		for (i = 0; i < jsonLength; i++)
		{
			c = *(uint8_t*)(XIP_BASE + JSON_STORAGE_ADDRESS + 4 + i);
			strJson.push_back(c);
		}
		printf("\n%s\n", json.c_str());
    }
}


void deserialiseJSON()
{
    printf("\n2. Parsing JSON configuration file\n");

    const char *json = strJson.c_str();

    // parse the json configuration file
    DeserializationError error = deserializeJson(doc, json);

    printf("Config deserialisation - ");

    switch (error.code())
    {
        case DeserializationError::Ok:
            printf("Deserialization succeeded\n");
            break;
        case DeserializationError::InvalidInput:
            printf("Invalid input!\n");
            configError = true;
            break;
        case DeserializationError::NoMemory:
            printf("Not enough memory\n");
            configError = true;
            break;
        default:
            printf("Deserialization failed\n");
            configError = true;
            break;
    }
}


void configThreads()
{
    if (configError) return;

    printf("\n3. Configuring threads\n");

    JsonArray Threads = doc["Threads"];

    // create objects from JSON data
    for (JsonArray::iterator it=Threads.begin(); it!=Threads.end(); ++it)
    {
        thread = *it;

        const char* configor = thread["Thread"];
        uint32_t    freq = thread["Frequency"];

        if (!strcmp(configor,"Base"))
        {
            base_freq = freq;
            printf("Setting BASE thread frequency to %d\n", base_freq);
        }
        else if (!strcmp(configor,"Servo"))
        {
            servo_freq = freq;
            printf("Setting SERVO thread frequency to %d\n", servo_freq);
        }
    }
}


void loadModules(void)
{
    printf("\n4. Loading modules\n");

	// Ethernet communication monitoring
	comms = new RemoraComms();
	servoThread->registerModule(comms);

    if (configError) return;

    JsonArray Modules = doc["Modules"];

    // create objects from JSON data
    for (JsonArray::iterator it=Modules.begin(); it!=Modules.end(); ++it)
    {
        module = *it;

        const char* thread = module["Thread"];
        const char* type = module["Type"];

        if (!strcmp(thread,"DMA"))
        {
            printf("\nDMA thread object\n");

            if (!strcmp(type,"DMAstepgen"))
            {
            	createDMAstepgen();
            	DMAstepgen = true;
            }
        }
        else if (!strcmp(thread,"Base"))
        {
            printf("\nBase thread object\n");

            if (!strcmp(type,"Stepgen"))
            {
                createStepgen();
            }
            else if (!strcmp(type,"Encoder"))
            {
                createEncoder();
            }
         }
        else if (!strcmp(thread,"Servo"))
        {
        	if (!strcmp(type,"Digital Pin"))
			{
				createDigitalPin();
			}
        	else if (!strcmp(type,"Spindle PWM"))
			{
				createSpindlePWM();
			}
        	else if (!strcmp(type,"NVMPG"))
			{
				createNVMPG();
			}
        }
    }
}

/*void debugThreadHigh()
{
    //Module* debugOnB = new Debug("PE_13", 1);
    //baseThread->registerModule(debugOnB);

    Module* debugOnS = new Debug("P1_22", 1);
    servoThread->registerModule(debugOnS);
}

void debugThreadLow()
{
    //Module* debugOffB = new Debug("PE_13", 0);
    //baseThread->registerModule(debugOffB);

    Module* debugOffS = new Debug("P1_22", 0);
    servoThread->registerModule(debugOffS);
}*/


void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
	g_transferDone = true;
}


// Interrupt service for SysTick timer.
extern "C" {
	void SysTick_Handler(void)
	{
		time_isr();
	}
}



void DMAconfig(void)
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

	/* Configure DMAMUX */
	DMAMUX_Init(DMAMUX);
	DMAMUX_EnableAlwaysOn(DMAMUX, 0, true);
	DMAMUX_EnableChannel(DMAMUX, 0);

	DMAMUX_EnablePeriodTrigger(DMAMUX, 0);

	EDMA_GetDefaultConfig(&userConfig);
	EDMA_Init(DMA0, &userConfig);
	EDMA_CreateHandle(&g_EDMA_Handle, DMA0, 0);
	EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
	EDMA_ResetChannel(g_EDMA_Handle.base, g_EDMA_Handle.channel);

	/* prepare descriptor 0 */
	EDMA_PrepareTransfer(&transferConfig, stepgenDMAbuffer_0, sizeof(stepgenDMAbuffer_0[0]), &GPIO1->DR_TOGGLE, sizeof(GPIO1->DR_TOGGLE),
						 sizeof(stepgenDMAbuffer_0[0]),
						 sizeof(stepgenDMAbuffer_0[0]) * DMA_BUFFER_SIZE,
						 kEDMA_MemoryToPeripheral);
	EDMA_TcdSetTransferConfig(tcdMemoryPoolPtr, &transferConfig, &tcdMemoryPoolPtr[1]);
	EDMA_TcdEnableInterrupts(&tcdMemoryPoolPtr[0], kEDMA_MajorInterruptEnable);

	/* prepare descriptor 1 */
	EDMA_PrepareTransfer(&transferConfig, stepgenDMAbuffer_1, sizeof(stepgenDMAbuffer_1[0]), &GPIO1->DR_TOGGLE, sizeof(GPIO1->DR_TOGGLE),
						 sizeof(stepgenDMAbuffer_1[0]),
						 sizeof(stepgenDMAbuffer_1[0]) * DMA_BUFFER_SIZE,
						 kEDMA_MemoryToPeripheral);
	EDMA_TcdSetTransferConfig(&tcdMemoryPoolPtr[1], &transferConfig, &tcdMemoryPoolPtr[0]);
	EDMA_TcdEnableInterrupts(&tcdMemoryPoolPtr[1], kEDMA_MajorInterruptEnable);

	EDMA_InstallTCD(DMA0, 0, tcdMemoryPoolPtr);

	EDMA_StartTransfer(&g_EDMA_Handle);
}

static void networking_thread_entry(ULONG parameter)
{
	/* Example of how to implement UDP Server/Client */
	//Implementation not done.
	//https://github.com/STMicroelectronics/x-cube-azrtos-h7/blob/main/Projects/STM32H735G-DK/Applications/NetXDuo/Nx_UDP_Echo_Server/NetXDuo/App/app_netxduo.c
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
    		NX_PACKET *remora_packet;

    	    NX_PARAMETER_NOT_USED(parameter);

    	    tx_thread_sleep(NX_IP_PERIODIC_RATE);
    	#ifndef NX_DISABLE_IPV6
    	    /* Wait 5 seconds for IPv6 stack to finish DAD process. */
    	    tx_thread_sleep(5 * NX_IP_PERIODIC_RATE);
    	#endif
    	    /* Create a UDP socket.  */
    	    status = nx_udp_socket_create(&nx_ip, &socketComms, "Socket 1", NX_IP_NORMAL, NX_FRAGMENT_OKAY, 0x80, 5);

    	    /* Check status.  */
    	    if (status)
    	    {
    	        error_counter++;
    	        return;
    	    }

    	    /* Bind the UDP socket to the IP port.  */
    	    status =  nx_udp_socket_bind(&socketComms, 27181, TX_WAIT_FOREVER);

    	    /* Check status.  */
    	    if (status)
    	    {
    	        error_counter++;
    	        return;
    	    }
    	    UINT source_port;
    	    ULONG source_ip_address;

    	    while (1)
    	    {

    	        /* Receive a UDP packet.  */
    	        status =  nx_udp_socket_receive(&socketComms, &remora_packet, TX_WAIT_FOREVER);
    	        nx_udp_source_extract(remora_packet, &source_ip_address, &source_port);

    	        /* Check status.  */
    	        if (status != NX_SUCCESS)
    	        {
    	            break;
    	        }
    	        else if(status == NX_SUCCESS) {

    	        	udp_data_callback(&remora_packet);
    	        }
    	        /* Release the packet.  */
    	        status =  nx_packet_release(remora_packet);

    	        /* Check status.  */
    	        if (status != NX_SUCCESS)
    	        {
    	            break;
    	        }
    	        /* Allocate a packet.  */
				status =  nx_packet_allocate(&nx_pool, &remora_packet, NX_UDP_PACKET, TX_WAIT_FOREVER);

				/* Check status.  */
				if (status != NX_SUCCESS)
				{
					break;
				}

				/* Write ABCs into the packet payload!  */
				//nx_packet_data_append(NX_PACKET *packet_ptr, VOID *data_start, ULONG data_size, NX_PACKET_POOL *pool_ptr, ULONG wait_option)
				nx_packet_data_append(remora_packet, (char*)&txData.txBuffer, sizeof(txData.txBuffer), &nx_pool, TX_WAIT_FOREVER);
				status =  nx_udp_socket_send(&socketComms, remora_packet, source_ip_address, source_port);
    	    }
    }
}

static void control_thread_entry(ULONG parameter)
{
    printf("Starting Control Thread\r\n\r\n");
    enum State currentState;
	enum State prevState;

	BOARD_ConfigMPU();
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	currentState = ST_SETUP;
	prevState = ST_RESET;

	printf("\nRemora RT1052 starting\n\n");


	//Module* debugOn1 = new Debug("P1_17", 1);
	//Module* debugOff1 = new Debug("P1_17", 0);
	//Module* debugOn2 = new Debug("P1_31", 1);
	//Module* debugOff2 = new Debug("P1_31", 0);

	initEthernet();

	while (1)
	{
	   switch(currentState){
					  case ST_SETUP:
						  // do setup tasks
						  if (currentState != prevState)
						  {
							  printf("\n## Entering SETUP state\n\n");
						  }
						  prevState = currentState;

						  jsonFromFlash(strJson);
						  deserialiseJSON();
						  configThreads();
						  createThreads();
						  //debugThreadHigh();
						  loadModules();
						  //debugThreadLow();
						  //udpServer_init();
						  //IAP_tftpd_init(g_EDMA_Handle);
						  currentState = ST_START;
						  break;

					  case ST_START:
						  // do start tasks
						  if (currentState != prevState)
						  {
							  printf("\n## Entering START state\n");
						  }
						  prevState = currentState;

						  if (!threadsRunning)
						  {
							  tx_thread_resume(&networking_thread);
							  tx_thread_resume(&dma_thread);

							  // Start the threads
							  printf("\nStarting the BASE thread\n");
							  baseThread->startThread();

							  printf("\nStarting the SERVO thread\n");
							  servoThread->startThread();

							  DMAconfig(); // put this in the right place+

							  threadsRunning = true;
						  }

						  currentState = ST_IDLE;

						  break;


					  case ST_IDLE:
						  // do something when idle
						  if (currentState != prevState)
						  {
							  printf("\n## Entering IDLE state\n");
						  }
						  prevState = currentState;

						  //wait for data before changing to running state
						  if (comms->getStatus())
						  {
							  currentState = ST_RUNNING;
						  }

						  break;

					  case ST_RUNNING:
						  // do running tasks
						  if (currentState != prevState)
						  {
							  printf("\n## Entering RUNNING state\n");
						  }
						  prevState = currentState;

						  if (comms->getStatus() == false)
						  {
							  currentState = ST_RESET;
						  }

						  break;

					  case ST_STOP:
						  // do stop tasks
						  if (currentState != prevState)
						  {
							  printf("\n## Entering STOP state\n");
						  }
						  prevState = currentState;


						  currentState = ST_STOP;
						  break;

					  case ST_RESET:
						  // do reset tasks
						  if (currentState != prevState)
						  {
							  printf("\n## Entering RESET state\n");
						  }
						  prevState = currentState;

						  // set all of the rxData buffer to 0
						  // rxData.rxBuffer is volatile so need to do this the long way. memset cannot be used for volatile
						  printf("   Resetting rxBuffer\n");
						  {
							  int n = sizeof(rxData.rxBuffer);
							  while(n-- > 0)
							  {
								  rxData.rxBuffer[n] = 0;
							  }
						  }

						  currentState = ST_IDLE;
						  break;

					  case ST_WDRESET:
						  // force a reset
						  //NVIC_SystemReset();
						  break;
			  }

		//EthernetTasks();
		if (newJson)
		{
			printf("\n\nChecking new configuration file\n");

			if (checkJson() > 0)
			{
				printf("Moving new configuration file to Flash storage and reset\n");
				moveJson();
				NVIC_SystemReset();
			}
		}
	}

}

static void dma_thread_entry(ULONG parameter)
{
    printf("Starting DMA Thread\r\n\r\n");
    while (1) {
    	if (g_transferDone)
		{


			// clear the DMA buffer ready for next use
			if (stepgenDMAbuffer)
			{
				memset(stepgenDMAbuffer_0, 0, sizeof(stepgenDMAbuffer_0));
			}
			else
			{
				memset(stepgenDMAbuffer_1, 0, sizeof(stepgenDMAbuffer_1));
			}

			// switch buffers
			stepgenDMAbuffer = !stepgenDMAbuffer;

			// prepare the next DMA buffer
			for (iterDMA = vDMAthread.begin(); iterDMA != vDMAthread.end(); ++iterDMA) (*iterDMA)->runModule();

			g_transferDone = false;
		}
    }
}

static void base_thread_entry(ULONG parameter)
{
    printf("Starting base Thread\r\n\r\n");
    while (1) {

    }
}

static void servo_thread_entry(ULONG parameter)
{
    printf("Starting servo Thread\r\n\r\n");
    while (1) {

    }
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
		TX_DONT_START);

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
				TX_DONT_START);

	if (dmaThreadStatus != TX_SUCCESS)
	{
		printf("DMA thread failed, please restart\r\n");
	}

	UINT baseThreadStatus = tx_thread_create(&base_thread,
		            "DMAstepgen Thread",
		    		base_thread_entry,
		            0,
		    		base_thread_stack,
					BASE_THREAD_STACK_SIZE,
		    		BASE_THREAD_PRIORITY,
		    		BASE_THREAD_PRIORITY,
		            TX_NO_TIME_SLICE,
					TX_DONT_START);

	if (baseThreadStatus != TX_SUCCESS)
	{
		printf("DMA thread failed, please restart\r\n");
	}

	UINT servoThreadStatus = tx_thread_create(&servo_thread,
					"DMAstepgen Thread",
					servo_thread_entry,
					0,
					servo_thread_stack,
					SERVO_THREAD_STACK_SIZE,
					SERVO_THREAD_PRIORITY,
					SERVO_THREAD_PRIORITY,
					TX_NO_TIME_SLICE,
					TX_DONT_START);

	if (servoThreadStatus != TX_SUCCESS)
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
