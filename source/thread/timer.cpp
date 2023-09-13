#include <stdio.h>

#include "../interrupt/interrupt.h"
#include "timerInterrupt.h"
#include "timer.h"
#include "pruThread.h"

#include "fsl_gpt.h"

VOID timer_entry(ULONG self) {
	pruTimer *$this = (pruTimer*)self;
	$this->timerTick();
}
//Timer set for 1ms per tick.
// Timer constructor
pruTimer::pruTimer(uint32_t frequency, pruThread* ownerPtr):
	frequency(frequency),
	timerOwnerPtr(ownerPtr)
{
	//interruptPtr = new TimerInterrupt(this->irq, this);	// Instantiate a new Timer Interrupt object and pass "this" pointer
	if (tx_timer_create(
					&this->timer,
					"",// name of timer
					timer_entry,//Function
					(ULONG)this, // Variable
					0, // zero initial ticks
					frequency, // reschedule time
					TX_NO_ACTIVATE) != TX_SUCCESS)
	{
		/// Could not create MeasInd timer.
		// Log error

	}
	//tx_timer_create(&this->timer, "remora_timer", this->timerTick(), )
	this->startTimer();
}


void pruTimer::timerTick(void)
{
	//Do something here
	this->timerOwnerPtr->run();
}



void pruTimer::startTimer(void)
{
	tx_timer_activate(&this->timer);
    printf("	timer started\n");
}

void pruTimer::stopTimer()
{
	tx_timer_deactivate(&this->timer);
    printf("	timer stop\n\r");
}

pruTimer::~pruTimer() {
	tx_timer_delete(&this->timer);
}
