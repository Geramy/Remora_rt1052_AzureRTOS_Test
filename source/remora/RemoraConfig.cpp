/*
 * RemoraConfig.cpp
 *
 *  Created on: Sep 13, 2023
 *      Author: Geramy
 */

#include <remora/RemoraConfig.h>

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_pit.h"
#include "flexspi_nor_flash.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_debug_console.h"

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

//Json Related Variables that dont work inside C++
DynamicJsonDocument doc(JSON_BUFF_SIZE);
extern volatile const char defaultConfig[] = DEFAULT_CONFIG;

RemoraConfig::RemoraConfig() {
	// TODO Auto-generated constructor stub
	//this->doc = DynamicJsonDocument::DynamicJsonDocument(JSON_BUFF_SIZE);
}

int8_t RemoraConfig::checkJson()
{
	metadata_t* meta = (metadata_t*)(XIP_BASE + JSON_UPLOAD_ADDRESS);
	//uint32_t* json = (uint32_t*)(XIP_BASE + JSON_UPLOAD_ADDRESS + METADATA_LEN);

    uint32_t table[256];
    crc32::generate_table(table);
    int mod, padding;

	// Check length is reasonable
	if (meta->length > 2 * SECTOR_SIZE)
	{
		this->newJson = false;
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
    this->crc32 = 0;
    char* ptr = (char *)(XIP_BASE + JSON_UPLOAD_ADDRESS + METADATA_LEN);
    for (uint32_t i = 0; i < meta->jsonLength + padding; i++)
    {
    	this->crc32 = crc32::update(table, this->crc32, ptr, 1);
        ptr++;
    }

	printf("Length (words) = %d\n", (int)meta->length);
	printf("JSON length (bytes) = %d\n", (int)meta->jsonLength);
	printf("crc32 = %x\n", (int)crc32);

	// Check CRC
	if (this->crc32 != meta->crc32)
	{
		this->newJson = false;
		printf("JSON Config file CRC incorrect\n");
		return -1;
	}

	// JSON is OK, don't check it again
	this->newJson = false;
	printf("JSON Config file received Ok\n");
	return 1;
}

void RemoraConfig::moveJson()
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

void RemoraConfig::jsonFromFlash()
{
	//std::string json;
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

    	this->strJson.resize(jsonLength);

		for (i = 0; i < jsonLength; i++)
		{
			c = defaultConfig[i];
			this->strJson.push_back(c);
		}
    }
    else
    {
    	this->strJson.resize(jsonLength);

		for (i = 0; i < jsonLength; i++)
		{
			c = *(uint8_t*)(XIP_BASE + JSON_STORAGE_ADDRESS + 4 + i);
			this->strJson.push_back(c);
		}
		printf("\n%s\n", this->strJson.c_str());
    }
}


void RemoraConfig::deserialiseJSON()
{
    printf("\n2. Parsing JSON configuration file\n");

    const char *json = this->strJson.c_str();

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
            this->configError = true;
            break;
        case DeserializationError::NoMemory:
            printf("Not enough memory\n");
            this->configError = true;
            break;
        default:
            printf("Deserialization failed\n");
            this->configError = true;
            break;
    }
}

void RemoraConfig::configThreads()
{
    if (this->configError) return;

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
        	this->base_freq = freq;
            printf("Setting BASE thread frequency to %d\n", (int)this->base_freq);
        }
        else if (!strcmp(configor,"Servo"))
        {
        	this->servo_freq = freq;
            printf("Setting SERVO thread frequency to %d\n", (int)this->servo_freq);
        }
    }
}

void RemoraConfig::loadModules(pruThread *base, pruThread *servo, RemoraStepGenDMA *dmaControl)
{
    printf("\n4. Loading modules\n");

	// Ethernet communication monitoring
	comms = new RemoraComms();
	servo->registerModule(comms);

    if (this->configError) return;

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
            	createDMAstepgen(dmaControl);
            	//DMAstepgen = true;
            }
        }
        else if (!strcmp(thread,"Base"))
        {
            printf("\nBase thread object\n");

            if (!strcmp(type,"Stepgen"))
            {
                createStepgen(base);
            }
            else if (!strcmp(type,"Encoder"))
            {
                createEncoder(base);
            }
         }
        else if (!strcmp(thread,"Servo"))
        {
        	if (!strcmp(type,"Digital Pin"))
			{
				createDigitalPin(servo);
			}
        	else if (!strcmp(type,"Spindle PWM"))
			{
				createSpindlePWM(servo);
			}
        	else if (!strcmp(type,"NVMPG"))
			{
				createNVMPG(servo);
			}
        }
    }
}

RemoraConfig::~RemoraConfig() {
	// TODO Auto-generated destructor stub
}

