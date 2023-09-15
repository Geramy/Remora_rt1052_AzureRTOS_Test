#pragma once
#ifndef ENCODER_H
#define ENCODER_H

#include <cstdint>
#include <iostream>
#include <string>

#include "configuration.h"
#include "modules/module.h"
#include "drivers/pin/pin.h"

#include "extern.h"

class Encoder : public Module
{

private:

	std::string ChA;			// physical pin connection
	std::string ChB;			// physical pin connection

	std::string Index;			// physical pin connection
	bool hasIndex;
	volatile uint32_t *ptrData; 	// pointer to the data source
	int bitNumber;				// location in the data source
	int mask;

	volatile float *ptrEncoderCount; 	// pointer to the data source

	uint8_t state;
	int32_t count;
	int32_t indexCount;
	int8_t  indexPulse;
	int8_t  pulseCount;
public:
	/***********************************************************************
	                MODULE CONFIGURATION AND CREATION FROM JSON
	************************************************************************/
	static void createEncoder(pruThread* thread)
	{
	    const char* comment = module["Comment"];
	    printf("%s\n",comment);

	    int pv = module["PV[i]"];
	    const char* pinA = module["ChA Pin"];
	    const char* pinB = module["ChB Pin"];
	    const char* pinI = module["Index Pin"];
	    int dataBit = module["Data Bit"];

	    printf("Creating Quadrature Encoder at pins %s and %s\n", pinA, pinB);


	    ptrProcessVariable[pv]  = &txData.processVariable[pv];
	    ptrInputs = &txData.inputs;

	    if (pinI == nullptr)
	    {
	        Module* encoder = new Encoder(*ptrProcessVariable[pv], pinA, pinB);
	        thread->registerModule(encoder);
	    }
	    else
	    {
	        printf("  Encoder has index at pin %s\n", pinI);
	        Module* encoder = new Encoder(*ptrProcessVariable[pv], *ptrInputs, dataBit, pinA, pinB, pinI);
	        thread->registerModule(encoder);
	    }
	};
public:

	Pin* pinA;      // channel A
	Pin* pinB;      // channel B
	Pin* pinI;      // index

	Encoder(volatile float&, std::string, std::string);
	Encoder(volatile float&, volatile uint32_t&, int, std::string, std::string, std::string);

	virtual void update(void);	// Module default interface
};

#endif
