/*
 * RemoraNetwork.h
 *
 *  Created on: Sep 13, 2023
 *      Author: Geramy
 */
#pragma once
#ifndef REMORA_REMORANETWORK_H_
#define REMORA_REMORANETWORK_H_
#include "nx_api.h"

class RemoraNetwork {
private:
	NX_IP nx_ip;
	NX_PACKET_POOL nx_pool;
	NX_PACKET_POOL icmp_packet_pool;
	NX_UDP_SOCKET *remoraUDPSocket;

	TX_MUTEX *rxMutexPtr;
	TX_MUTEX *txMutexPtr;
	void ReceivePacket();
	void ExtractAndProcess(NX_PACKET* udpPacket);
	UINT EnableCaps();
public:
	RemoraNetwork(TX_MUTEX* rxMutex, TX_MUTEX* txMutex);
	void (*ReceivedData)(int);

	void Initialize();
	bool Accept();
	virtual ~RemoraNetwork();
};

#endif /* REMORA_REMORANETWORK_H_ */
