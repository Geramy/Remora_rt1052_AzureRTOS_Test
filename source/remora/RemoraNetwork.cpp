/*
 * RemoraNetwork.cpp
 *
 *  Created on: Sep 13, 2023
 *      Author: Geramy
 */

#include <remora/RemoraNetwork.h>
#include "drivers/nx_driver/nx_driver_imxrt10xx.h"
#include <stdint.h>

#include "nx_api.h"
#include "configuration.h"
#include "extern.h"


#define NETX_IP_STACK_SIZE  1024
#define NETX_PACKET_COUNT   8
#define NETX_PACKET_SIZE    128
#define NETX_POOL_SIZE      ((NETX_PACKET_SIZE + sizeof(NX_PACKET)) * NETX_PACKET_COUNT)

#define NETX_IPV4_ADDRESS IP_ADDRESS(10, 10, 10, 10)
#define NETX_IPV4_MASK    IP_ADDRESS(255, 255, 255, 0)

static UCHAR netx_ip_stack[NETX_IP_STACK_SIZE];
static UCHAR netx_ip_pool[NETX_POOL_SIZE];

RemoraNetwork::RemoraNetwork(TX_MUTEX* rxMutex, TX_MUTEX* txMutex) :
		rxMutexPtr(rxMutex),
		txMutexPtr(txMutex)
{

}

UINT RemoraNetwork::EnableCaps(VOID (*ip_link_driver)(struct NX_IP_DRIVER_STRUCT*))
{
	UINT status;

    // Initialize the NetX system.
    nx_system_initialize();

    // Create a packet pool.
    if ((status = nx_packet_pool_create(&this->nx_pool, "NetX Packet Pool", NETX_PACKET_SIZE, netx_ip_pool, NETX_POOL_SIZE)))
    {
        printf("ERROR: nx_packet_pool_create (0x%08x)\r\n", status);
    }

    // Create an IP instance
    if ((status = nx_ip_create(&this->nx_ip,
                  "NetX IP Instance 0",
                  NETX_IPV4_ADDRESS,
                  NETX_IPV4_MASK,
                  &this->nx_pool,
                  ip_link_driver,
                  netx_ip_stack,
                  NETX_IP_STACK_SIZE,
                  1)))
    {
        nx_packet_pool_delete(&this->nx_pool);
        printf("ERROR: nx_ip_create (0x%08x)\r\n", status);
    }

    // Enable ARP and supply ARP cache memory
    /*else if ((status = nx_arp_enable(&this->nx_ip, (VOID*)netx_arp_cache_area, NETX_ARP_CACHE_SIZE)))
    {
        nx_ip_delete(&this->nx_ip);
        nx_packet_pool_delete(&this->nx_pool);
        printf("ERROR: nx_arp_enable (0x%08x)\r\n", status);
    }*/

    // Enable TCP traffic
    /*else if ((status = nx_tcp_enable(&nx_ip)))
    {
        nx_ip_delete(&nx_ip);
        nx_packet_pool_delete(&nx_pool);
        printf("ERROR: nx_tcp_enable (0x%08x)\r\n", status);
        return status;
    }*/

    // Enable UDP traffic
    if ((status = nx_udp_enable(&this->nx_ip)))
    {
        nx_ip_delete(&this->nx_ip);
        nx_packet_pool_delete(&this->nx_pool);
        printf("ERROR: nx_udp_enable (0x%08x)\r\n", status);
    }

    // Enable ICMP traffic
    //Disable until we make our own pool for just ICMP packets
    /*else if ((status = nx_icmp_enable(&this->nx_ip)))
    {
        nx_ip_delete(&this->nx_ip);
        nx_packet_pool_delete(&this->nx_pool);
        printf("ERROR: nx_icmp_enable (0x%08x)\r\n", status);
    }*/

    return status;
}

void RemoraNetwork::Initialize() {
	UINT status;
	short error_counter = 0;
	UINT thread_1_counter = 0;
	printf("Starting Networking Thread\r\n\r\n");

	// Initialize the network
	if ((status = this->EnableCaps(nx_driver_imx)))
	{
		printf("ERROR: Failed to initialize the network (0x%08x)\r\n", status);
	}
	else {
		UINT       status;

		tx_thread_sleep(NX_IP_PERIODIC_RATE);
	#ifndef NX_DISABLE_IPV6
		/* Wait 5 seconds for IPv6 stack to finish DAD process. */
		tx_thread_sleep(5 * NX_IP_PERIODIC_RATE);
	#endif
		/* Create a UDP socket.  */
		status = nx_udp_socket_create(&this->nx_ip, this->remoraUDPSocket, "Socket 1", NX_IP_NORMAL, NX_FRAGMENT_OKAY, 0x80, 5);

		/* Check status.  */
		if (status)
		{
			error_counter++;
			return;
		}

		/* Bind the UDP socket to the IP port.  */
		status =  nx_udp_socket_bind(this->remoraUDPSocket, 27181, TX_WAIT_FOREVER);

		/* Check status.  */
		if (status)
		{
			error_counter++;
			return;
		}
	}
}

bool RemoraNetwork::Accept() {
	UINT       status;
	NX_PACKET *remora_packet;
	status =  nx_udp_socket_receive(this->remoraUDPSocket, &remora_packet, TX_NO_WAIT);
	if (status == NX_SUCCESS)
	{
		this->ExtractAndProcess(remora_packet);
		return true;
	}
	else
		return false;
}

void RemoraNetwork::ExtractAndProcess(NX_PACKET* udpPacket) {
	UINT       status;
	UINT source_port;
	ULONG source_ip_address;

	status = nx_udp_source_extract(udpPacket, &source_ip_address, &source_port);
	/* Check status.  */
	if (status != NX_SUCCESS)
	{
		return;
	}
	else if(status == NX_SUCCESS) {
		//Run recieve code here
		//udp_data_callback(&remora_packet);
		ULONG bytesRx;
		UINT status = nx_packet_data_retrieve(udpPacket, &rxBuffer.rxBuffer, &bytesRx);

		int txlen = 0;

		// copy the UDP payload into the rxData structure

		if (rxBuffer.header == PRU_READ)
		{
			txData.header = PRU_DATA;
			txlen = BUFFER_SIZE;
			comms->dataReceived();
		}
		else if (rxBuffer.header == PRU_WRITE)
		{
			txData.header = PRU_ACKNOWLEDGE;
			txlen = sizeof(txData.header);
			comms->dataReceived();

			// ensure an atomic access to the rxBuffer
			// disable thread interrupts
			//We will need to use this as an example to get the mutex to fill the buffer, so that we don't have any collisions.
			//https://github.com/azure-rtos/threadx/blob/master/samples/demo_threadx.c
			tx_mutex_get(this->rxMutexPtr, TX_WAIT_FOREVER);
			// then move the data
			/*for (int i = 0; i < BUFFER_SIZE; i++)
			{
				rxData.rxBuffer[i] = rxBuffer.rxBuffer[i];
			}*/
			memcpy(&rxBuffer.rxBuffer, rxBuffer.rxBuffer, sizeof(uint8_t) * BUFFER_SIZE);
			tx_mutex_put(this->rxMutexPtr);
		}
	}
	/* Release the packet.  */
	status = nx_packet_release(udpPacket);

	/* Check status.  */
	if (status != NX_SUCCESS)
	{
		return;
	}
	/* Allocate a packet.  */
	NX_PACKET *txUdpPacket;
	status =  nx_packet_allocate(&this->nx_pool, &txUdpPacket, NX_UDP_PACKET, TX_WAIT_FOREVER);

	/* Check status.  */
	if (status != NX_SUCCESS)
	{
		return;
	}

	/* Write ABCs into the packet payload!  */
	//nx_packet_data_append(NX_PACKET *packet_ptr, VOID *data_start, ULONG data_size, NX_PACKET_POOL *pool_ptr, ULONG wait_option)
	nx_packet_data_append(txUdpPacket, (char*)&txData.txBuffer, sizeof(txData.txBuffer), &this->nx_pool, TX_WAIT_FOREVER);
	status =  nx_udp_socket_send(this->remoraUDPSocket, txUdpPacket, source_ip_address, source_port);
	status = nx_packet_release(txUdpPacket);
}

RemoraNetwork::~RemoraNetwork() {
	// TODO Auto-generated destructor stub
}

