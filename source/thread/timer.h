#pragma once
#ifndef TIMER_H
#define TIMER_H

#include "MIMXRT1052.h"
#include <stdint.h>
#include "fsl_gpt.h"
#include "../../rtos/threadx/common/inc/tx_api.h"
#include "../../rtos/threadx/ports/cortex_m7/gnu/inc/tx_port.h"

//#define TIM_PSC 1
//#define APB1CLK SystemCoreClock/2
//#define APB2CLK SystemCoreClock

class TimerInterrupt; // forward declaration
class pruThread; // forward declaration

class pruTimer
{
	friend class TimerInterrupt;

	private:

		TimerInterrupt* 	interruptPtr;
		uint32_t 			frequency;
		pruThread* 			timerOwnerPtr;

		TX_TIMER			timer;
		UINT status;

		void startTimer(void);


	public:

		pruTimer(uint32_t frequency, pruThread* ownerPtr);
        void stopTimer(void);
        void timerTick();			// Private timer triggered method
        ~pruTimer();
};

#endif
