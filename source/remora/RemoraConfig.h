/*
 * RemoraConfig.h
 *
 *  Created on: Sep 13, 2023
 *      Author: Geramy
 */

#ifndef REMORA_REMORACONFIG_H_
#define REMORA_REMORACONFIG_H_

#include <string>
#include "configuration.h"
#include "crc32.h"

//Json Libraries
#include "lib/ArduinoJson6/ArduinoJson.h"

//Threads and DMA Libraries
#include "thread/pruThread.h"
#include "remora/RemoraStepGenDMA.h"

class RemoraConfig {
private:
	// JSON config file stuff
	// 512 bytes of metadata in front of actual JSON file
	typedef struct
	{
	  uint32_t crc32;   		// crc32 of JSON
	  uint32_t length;			// length in words for CRC calculation
	  uint32_t jsonLength;  	// length in of JSON config in bytes
	  uint8_t padding[500];
	} metadata_t;
	#define METADATA_LEN    512

	uint32_t base_freq = PRU_BASEFREQ;
	uint32_t servo_freq = PRU_SERVOFREQ;

	bool configError;

	uint32_t crc32;
	FILE *jsonFile;
	string strJson;

	JsonObject thread;
	JsonObject module;
	/* End of Remora Original Data */
public:
	bool newJson;

public:
	RemoraConfig();
	int8_t checkJson();
	void moveJson();
	void jsonFromFlash();
	void deserialiseJSON();
	void configThreads();
	void loadModules(pruThread*, pruThread*, RemoraStepGenDMA*);
	virtual ~RemoraConfig();
};

#endif /* REMORA_REMORACONFIG_H_ */
