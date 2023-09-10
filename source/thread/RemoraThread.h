/*
 * RemoraThread.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Geramy
 */

#ifndef THREAD_REMORATHREAD_H_
#define THREAD_REMORATHREAD_H_

#include "MIMXRT1052.h"
#include "tx_api.h"

// Standard Template Library (STL) includes
#include <vector>

using namespace std;

class Module;

class RemoraThread {

	private:
		UINT THREAD_STACK_SIZE;
		UINT THREAD_PRIORITY;
		TX_THREAD threadHandle;
		ULONG *thread_stack;
		UINT frequency;

		bool hasThreadPost;		// run updatePost() vector

		vector<Module*> vThread;		// vector containing pointers to Thread modules
		vector<Module*> vThreadPost;		// vector containing pointers to Thread modules that run after the main vector modules
		vector<Module*>::iterator iter;
	public:
		RemoraThread(UINT, UINT, UINT, bool);
		void RemoraThreadEntry();
		void registerModule(Module *module);
		void registerModulePost(Module *module);
		void startThread(void);
        void stopThread(void);
		void run(void);

		virtual ~RemoraThread();
};

#endif
