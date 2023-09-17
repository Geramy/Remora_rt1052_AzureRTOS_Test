/*
 * RemoraNetwork.cpp
 *
 *  Created on: Sep 13, 2023
 *      Author: Geramy
 */

#include <remora/RemoraNetwork.h>
#include <stdint.h>
#include "drivers/nx_driver/nx_driver_imxrt10xx.h"
#include "configuration.h"
#include "extern.h"
#include "fsl_silicon_id.h"
#include "fsl_phy.h"
#include "fsl_phylan8720a.h"
#include "fsl_enet_mdio.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "board.h"
#include "../../rtos/netxduo/c/inc/nx_api.h"

#define NETX_IP_STACK_SIZE  256
#define NETX_PACKET_COUNT   8
#define NETX_PACKET_SIZE    96
#define NETX_POOL_SIZE      ((NETX_PACKET_SIZE + sizeof(NX_PACKET)) * NETX_PACKET_COUNT)

//#define NETX_IPV4_ADDRESS IP_ADDRESS(10, 10, 10, 10)
//#define NETX_IPV4_MASK    IP_ADDRESS(255, 255, 255, 0)

static UCHAR netx_ip_stack[NETX_IP_STACK_SIZE];
static UCHAR netx_ip_pool[NETX_POOL_SIZE];

static mdio_handle_t mdioHandle = {.ops = &enet_ops};

RemoraNetwork::RemoraNetwork(TX_MUTEX* rxMutex, TX_MUTEX* txMutex) :
		rxMutexPtr(rxMutex),
		txMutexPtr(txMutex)
{

}

UINT RemoraNetwork::EnableCaps()
{
	UINT status;
	// Enable Ethernet Hardware
	/*const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
		CLOCK_InitEnetPll(&config);
	IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
	mdioHandle.resource.csrClock_Hz = CLOCK_GetFreq(kCLOCK_IpgClk);*/
	//(void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);



    // Create a packet pool.
    if ((status = nx_packet_pool_create(&this->nx_pool, "NetX Packet Pool", NETX_PACKET_SIZE, netx_ip_pool, NETX_POOL_SIZE)))
    {
        printf("ERROR: nx_packet_pool_create (0x%08x)\r\n", status);
    }
    // Create an IP instance
    /*_nxe_ip_create(NX_IP *ip_ptr, CHAR *name, ULONG ip_address, ULONG network_mask,
                        NX_PACKET_POOL *default_pool,
                        VOID (*ip_link_driver)(NX_IP_DRIVER *),
                        VOID *memory_ptr, ULONG memory_size, UINT priority, SIZEOF(NX_IP))*/
    if ((status = nx_ip_create(&this->nx_ip,
                  "NetX IP Instance 0",
				  IP_ADDRESS(10, 10, 10, 10),
				  IP_ADDRESS(255, 255, 255, 0),
                  &this->nx_pool,
				  nx_driver_imx,
                  &netx_ip_stack[0],
                  sizeof(netx_ip_stack),
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
    if ((status = nx_icmp_enable(&this->nx_ip)))
    {
        printf("ERROR: nx_icmp_enable (0x%08x)\r\n", status);
    }

    return status;
}

void RemoraNetwork::Initialize() {
	UINT status;
	short error_counter = 0;
	UINT thread_1_counter = 0;
	printf("Starting Networking Thread\r\n\r\n");

	// Initialize the network
	if ((status = this->EnableCaps()))
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

