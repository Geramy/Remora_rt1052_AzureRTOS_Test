/*
 * RemoraThread.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */
#pragma once
#ifndef THREAD_REMORATHREAD_H_
#define THREAD_REMORATHREAD_H_

#include "MIMXRT1052.h"
#include "tx_api.h"
#include "modules/module.h"
// Standard Template Library (STL) includes
#include <vector>

using namespace std;

class RemoraThread {

	private:

		TX_THREAD threadHandle;
		ULONG *thread_stack;

		bool hasThreadPost;		// run updatePost() vector

	public:
		UINT THREAD_STACK_SIZE;
		UINT THREAD_PRIORITY;
		UINT frequency;
	public:
		RemoraThread(UINT, UINT, UINT, bool);
		virtual void RemoraThreadEntry() {}
		void startThread(void);
        void stopThread(void);

		virtual ~RemoraThread();
};

#endif
