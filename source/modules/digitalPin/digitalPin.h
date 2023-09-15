#pragma once
#ifndef DIGITALPIN_H
#define DIGITALPIN_H

#include <cstdint>
#include <sys/errno.h>

#include "modules/module.h"
#include "drivers/pin/pin.h"

#include "extern.h"

void createDigitalPin(pruThread*);

class DigitalPin : public Module
{
private:

	volatile uint32_t *ptrData; 	// pointer to the data source
	int bitNumber;				// location in the data source
	bool invert;
	int mask;

	int mode;
	int modifier;
	std::string portAndPin;

	Pin *pin;
public:
	/***********************************************************************
	                MODULE CONFIGURATION AND CREATION FROM JSON
	************************************************************************/
	static void createDigitalPin(pruThread *thread)
	{
	    const char* comment = module["Comment"];
	    printf("\n%s\n",comment);

	    const char* pin = module["Pin"];
	    const char* mode = module["Mode"];
	    const char* invert = module["Invert"];
	    const char* modifier = module["Modifier"];
	    int dataBit = module["Data Bit"];

	    int mod;
	    bool inv;

	    if (!strcmp(modifier,"Open Drain"))
	    {
	        mod = OPENDRAIN;
	    }
	    else if (!strcmp(modifier,"Pull Up"))
	    {
	        mod = PULLUP;
	    }
	    else if (!strcmp(modifier,"Pull Down"))
	    {
	        mod = PULLDOWN;
	    }
	    else if (!strcmp(modifier,"Pull None"))
	    {
	        mod = PULLNONE;
	    }
	    else
	    {
	        mod = NONE;
	    }

	    if (!strcmp(invert,"True"))
	    {
	        inv = true;
	    }
	    else inv = false;

	    ptrOutputs = &rxData.outputs;
	    ptrInputs = &txData.inputs;

	    printf("Make Digital %s at pin %s\n", mode, pin);

	    if (!strcmp(mode,"Output"))
	    {
	        Module* digitalPin = new DigitalPin(*ptrOutputs, 1, pin, dataBit, inv);
	        thread->registerModule(digitalPin);
	    }
	    else if (!strcmp(mode,"Input"))
	    {
	        Module* digitalPin = new DigitalPin(*ptrInputs, 0, pin, dataBit, inv);
	        thread->registerModule(digitalPin);
	    }
	    else
	    {
	        printf("Error - incorrectly defined Digital Pin\n");
	    }
	};
public:

	DigitalPin(volatile uint32_t&, int, std::string, int, bool);
	virtual void update(void);
	virtual void slowUpdate(void);
};

#endif
