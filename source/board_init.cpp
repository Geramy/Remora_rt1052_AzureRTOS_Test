/*
 * board_init.c
 *
 *  Created on: Sep 9, 2023
 *      Author: Geramy
 */

#include "board_init.h"

#include "board.h"
#include "pin_mux.h"

void board_init()
{
    /* Init board hardware. */

        BOARD_ConfigMPU();
        BOARD_InitBootPins();
        BOARD_InitBootClocks();
        /* Init FSL debug console. */
        BOARD_InitDebugConsole();
	//tempmon_init();
}
