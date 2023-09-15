#include "DMAstepgen.h"

/***********************************************************************
                MODULE CONFIGURATION AND CREATION FROM JSON     
************************************************************************/

/***********************************************************************
                METHOD DEFINITIONS
************************************************************************/

DMAstepgen::DMAstepgen(int32_t threadFreq, int jointNumber, std::string step, std::string direction, int DMAbufferSize, int stepBit, volatile int32_t &ptrFrequencyCommand, volatile int32_t &ptrFeedback, volatile uint8_t &ptrJointEnable, RemoraStepGenDMA &ptrDMAControl) :
	jointNumber(jointNumber),
	step(step),
	direction(direction),
	DMAbufferSize(DMAbufferSize),
	stepBit(stepBit),
	ptrFrequencyCommand(&ptrFrequencyCommand),
	ptrFeedback(&ptrFeedback),
	ptrJointEnablel(&ptrJointEnable),
	stepTime(1),
	dirHoldTime(1),
	dirSetupTime(1),
	ptrDMAC(&ptrDMAControl)
{
	uint8_t pin, pin2;

	stepDMAbuffer_0 = &ptrDMAC->GetBufferAddress(true, 0);
	stepDMAbuffer_1 = &ptrDMAC->GetBufferAddress(false, 0);
	stepDMAactiveBuffer = &ptrDMAC->stepgenDMAbuffer;
	//this->frequencyCommand = 500000;

	//if (this->jointNumber == 1)
//	{
//		this->frequencyCommand = 431125;
//	}


	dirDMAbuffer_0 = &ptrDMAC->GetBufferAddress(true, 0);
	dirDMAbuffer_1 =  &ptrDMAC->GetBufferAddress(false, 0);
	dirDMAactiveBuffer = &ptrDMAC->stepgenDMAbuffer;


	this->stepPin = new Pin(this->step, OUTPUT);
	this->directionPin = new Pin(this->direction, OUTPUT);
	this->accumulator = 0;
	this->remainder = 0;
	this->prevRemainder = 0;
	this->stepLow = 0;
	this->mask = 1 << this->jointNumber;
	this->isEnabled = false;
	this->dir = false;
	//this->isForward = false;

	// determine the step pin number from the portAndPin string
	pin = this->step[3] - '0';
	pin2 = this->step[4] - '0';
	if (pin2 <= 9) pin = pin * 10 + pin2;
	this->stepMask = 1 << pin;

	// determine the dir pin number from the portAndPin string
	pin = this->direction[3] - '0';
	pin2 = this->direction[4] - '0';
	if (pin2 <= 8) pin = pin * 10 + pin2;
	this->dirMask = 1 << pin;
	this ->addValueFrequency = (BUFFER_COUNTS * PRU_SERVOFREQ);
	//this->isEnabled = true; // TESTING!!! to be removed
}


void DMAstepgen::update()
{
	// Use the standard Module interface to run makePulses()
	this->makePulses();
}

void DMAstepgen::updatePost()
{
	this->stopPulses();
}

void DMAstepgen::slowUpdate()
{
	return;
}

void DMAstepgen::makePulses()
{
	/*
	(1) steplen
	(2) stepspace
	(3) dirhold
	(4) dirsetup

			   _____         _____               _____
	STEP  ____/     \_______/     \_____________/     \______
			  |     |       |     |             |     |
	Time      |-(1)-|--(2)--|-(1)-|--(3)--|-(4)-|-(1)-|
										  |__________________
	DIR   ________________________________/

	 */


	this->isEnabled = ((*(this->ptrJointEnablel) & this->mask) != 0);

	if (this->isEnabled == true)  												// this Step generator is enabled so make the pulses
	{
		this->frequencyCommand = *(this->ptrFrequencyCommand);            		// Get the latest frequency command via pointer to the data source
		if (this->frequencyCommand != 0)
		{
			this->oldaddValue = this->addValue;
			this->addValue = addValueFrequency / abs(this->frequencyCommand);		// determine the add value from the commanded frequency ratio

			// determine which ping-pong buffer to fill
			if (!*stepDMAactiveBuffer)	// false = buffer_0
			{
				stepDMAbuffer = stepDMAbuffer_0;
			}
			else // buffer_1
			{
				stepDMAbuffer = stepDMAbuffer_1;
			}

			// finish the step from the previous period if needed
			if (this->stepLow != 0)
			{
				// put step low into DMA buffer
				*(stepDMAbuffer + this->stepLow - 1) |= this->stepMask;
				this->stepLow = 0;
			}

			// what's the direction for this period
			if (this->frequencyCommand < 0)
			{
				this->dir = false; // backwards
			}
			else
			{
				this->dir = true; // forwards
			}

			// change of direction?
			if (this->dir != this->oldDir)
			{
				// TODO set the dirHold and dirSetup

				// toggle the direction pin
				*stepDMAbuffer |= this->dirMask;
				this->oldDir = this->dir;
			}

			// accumulator cannot go negative, so keep prevRemainder within limits
			if (this->prevRemainder > this->addValue)
			{
				this->prevRemainder = this->addValue;
			}

			if (this->addValue - this->prevRemainder <= BUFFER_COUNTS)
			{
				// at least one step in this period

				this->accumulator = this->addValue - this->prevRemainder;
				this->remainder = BUFFER_COUNTS - this->accumulator;
				this->makeStep();

				while (this->remainder >= this->addValue)
				{
					// we can still step in this period
					this->accumulator = this->accumulator + this->addValue;
					this->remainder = BUFFER_COUNTS - this->accumulator;

					this->makeStep();
				}

				// reset accumulator and carry remainder into the next period
				this->accumulator = 0;
				this->prevRemainder = this->remainder;

				// update DDS accumulator (for compatability with software stepgen)
				this->DDSaccumulator = this->rawCount << this->stepBit;
				*(this->ptrFeedback) = this->DDSaccumulator;                     // Update position feedback via pointer to the data receiver
			}
			else
			{
				this->prevRemainder = this->prevRemainder + BUFFER_COUNTS;
			}
		}
	}
	else
	{
		this->prevRemainder = 0;
	}
}


void DMAstepgen::makeStep()
{
	// map stepPos (1 - 500) to DMA buffer (0 - 999)
	this->stepPos = this->accumulator / (RESOLUTION / 2);
	this->stepHigh = this->stepPos - 2;
	this->stepLow = this->stepHigh + 1;
	// TODO incorporate step length setting, which will impact max frequency / minimum add value

	//printf("acc = %ld, rem = %ld, stepH = %d, stepL = %d\n\r", this->accumulator, this->remainder, this->stepHigh, this->stepLow);

	// put step high into DMA buffer
	*(stepDMAbuffer + this->stepHigh) |= this->stepMask;
	this->stepHigh = 0;

	// update the raw step count
	if (this->dir)
	{
		this->rawCount++;
	}
	else
	{
		this->rawCount--;
	}

	// put step low into DMA buffer
	// step low could be in the next period (buffer)
	if (this->stepLow <= DMA_BUFFER_SIZE - 1)
	{
		// put step low into DMA buffer
		*(stepDMAbuffer + this->stepLow) |= this->stepMask;
		this->stepLow = 0;
	}
	else
	{
		this->stepLow = this->stepLow - DMA_BUFFER_SIZE;
	}
}


void DMAstepgen::stopPulses()
{

}


void DMAstepgen::setEnabled(bool state)
{
	this->isEnabled = state;
}
