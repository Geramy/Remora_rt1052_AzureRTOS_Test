/*
 * RemoraTFTPServer.h
 *
 *  Created on: Sep 15, 2023
 *      Author: Geramy
 *      Derrived from other code as shown below,
 *      modified to work with Remora and Azure RTOS / ThreadX
 *      for RT1052 with ethernet adapter Microchip 8071A
 */
/*
  ******************************************************************************
  * @file    LwIP/LwIP_IAP/Src/tftpserver.c
  * @author  MCD Application Team
  * @brief   basic tftp server implementation for IAP (only Write Req supported)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
*/

#pragma once
#ifndef REMORA_REMORATFTPSERVER_H_
#define REMORA_REMORATFTPSERVER_H_

#include "remora/RemoraNetwork.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "flexspi_nor_flash.h"
#include <string.h>
#include <stdio.h>
#include "configuration.h"
#include "extern.h"

#define TFTP_OPCODE_LEN         2
#define TFTP_BLKNUM_LEN         2
#define TFTP_DATA_LEN_MAX       512
#define TFTP_DATA_PKT_HDR_LEN   (TFTP_OPCODE_LEN + TFTP_BLKNUM_LEN)
#define TFTP_ERR_PKT_HDR_LEN    (TFTP_OPCODE_LEN + TFTP_ERRCODE_LEN)
#define TFTP_ACK_PKT_LEN        (TFTP_OPCODE_LEN + TFTP_BLKNUM_LEN)
#define TFTP_DATA_PKT_LEN_MAX   (TFTP_DATA_PKT_HDR_LEN + TFTP_DATA_LEN_MAX)
#define TFTP_MAX_RETRIES        3
#define TFTP_TIMEOUT_INTERVAL   5

class RemoraTFTPServer {
private:
	typedef struct
	{
	  int op;    /*WRQ */
	  /* last block read */
	  char data[TFTP_DATA_PKT_LEN_MAX];
	  int  data_len;
	  /* destination ip:port */
	  //ip_addr_t to_ip;
	  int to_port;
	  /* next block number */
	  int block;
	  /* total number of bytes transferred */
	  int tot_bytes;
	  /* timer interrupt count when last packet was sent */
	  /* this should be used to resend packets on timeout */
	  unsigned long long last_time;

	} tftp_connection_args;


	/* TFTP opcodes as specified in RFC1350   */
	typedef enum {
	  TFTP_RRQ = 1,
	  TFTP_WRQ = 2,
	  TFTP_DATA = 3,
	  TFTP_ACK = 4,
	  TFTP_ERROR = 5
	} tftp_opcode;


	/* TFTP error codes as specified in RFC1350  */
	typedef enum {
	  TFTP_ERR_NOTDEFINED,
	  TFTP_ERR_FILE_NOT_FOUND,
	  TFTP_ERR_ACCESS_VIOLATION,
	  TFTP_ERR_DISKFULL,
	  TFTP_ERR_ILLEGALOP,
	  TFTP_ERR_UKNOWN_TRANSFER_ID,
	  TFTP_ERR_FILE_ALREADY_EXISTS,
	  TFTP_ERR_NO_SUCH_USER,
	} tftp_errorcode;

	RemoraNetwork *networkPtr;
private:
	/* Private variables ---------------------------------------------------------*/
	static uint32_t Flash_Write_Address;
	static struct udp_pcb *UDPpcb;
	//static __IO uint32_t total_count=0;
private:
	/**
	  * @brief Returns the TFTP opcode
	  * @param buf: pointer on the TFTP packet
	  * @retval None
	  */
	static tftp_opcode IAP_tftp_decode_op(char *buf)
	{
	  return (tftp_opcode)(buf[1]);
	};

	/**
	  * @brief  Extracts the block number
	  * @param  buf: pointer on the TFTP packet
	  * @retval block number
	  */
	/*static u16_t IAP_tftp_extract_block(char *buf)
	{
	  u16_t *b = (u16_t*)buf;
	  return ntohs(b[1]);
	};*

	/**
	  * @brief Sets the TFTP opcode
	  * @param  buffer: pointer on the TFTP packet
	  * @param  opcode: TFTP opcode
	  * @retval None
	  */
	/*static void IAP_tftp_set_opcode(char *buffer, tftp_opcode opcode)
	{
	  buffer[0] = 0;
	  buffer[1] = (u8_t)opcode;
	};*/

	/**
	  * @brief Sets the TFTP block number
	  * @param packet: pointer on the TFTP packet
	  * @param  block: block number
	  * @retval None
	  */
	/*static void IAP_tftp_set_block(char* packet, u16_t block)
	{
	  u16_t *p = (u16_t *)packet;
	  p[1] = htons(block);
	};*/
public:
	RemoraTFTPServer(RemoraNetwork*);
	void InitializeServer();
	void StartServer();
	RemoraTFTPServer::tftp_opcode Accept();
	bool AcceptFlash();
	void StopServer();
	virtual ~RemoraTFTPServer();
};

#endif /* REMORA_REMORATFTPSERVER_H_ */
