#include "digitalPin.h"

/***********************************************************************
                METHOD DEFINITIONS
************************************************************************/

DigitalPin::DigitalPin(volatile uint32_t &ptrData, int mode, std::string portAndPin, int bitNumber, bool invert) :
	ptrData(&ptrData),
	mode(mode),
	portAndPin(portAndPin),
	bitNumber(bitNumber),
    invert(invert),
	modifier(modifier)
{
	this->pin = new Pin(this->portAndPin, this->mode);		// Input 0x0, Output 0x1
	this->mask = 1 << this->bitNumber;
}


void DigitalPin::update()
{
	bool pinState;

	if (this->mode == 0)									// the pin is configured as an input
	{
		pinState = this->pin->get();
		if(this->invert)
		{
			pinState = !pinState;
		}

		if (pinState == 1)								// input is high
		{
			*(this->ptrData) |= this->mask;
		}
		else											// input is low
		{
			*(this->ptrData) &= ~this->mask;
		}
	}
	else												// the pin is configured as an output
	{
		pinState = *(this->ptrData) & this->mask;		// get the value of the bit in the data source
		if(this->invert)
		{
			pinState = !pinState;
		}
		this->pin->set(pinState);			// simple conversion to boolean
	}
}

void DigitalPin::slowUpdate()
{
	return;
}
