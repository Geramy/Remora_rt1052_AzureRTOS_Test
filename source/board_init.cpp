/*
 * board_init.c
 *
 *  Created on: Sep 9, 2023
 *      Author: Geramy
 */

#include "board_init.h"

#include "board.h"
#include "fsl_tempmon.h"
#include "pin_mux.h"

static void tempmon_init()
{
    tempmon_config_t config;
    TEMPMON_GetDefaultConfig(&config);
    TEMPMON_Init(TEMPMON, &config);
}

void board_init()
{
    /* Init board hardware. */
        BOARD_ConfigMPU();
        BOARD_InitBootPins();
        BOARD_InitBootClocks();
    #ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
        /* Init FSL debug console. */
        BOARD_InitDebugConsole();
    #endif
	tempmon_init();
}
