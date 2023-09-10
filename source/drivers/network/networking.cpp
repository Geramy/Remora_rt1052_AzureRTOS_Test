/*
 * networking.c
 *
 *  Created on: Sep 9, 2023
 *      Author: Geramy
 */
#include "networking.h"
#include <stdint.h>

#include "nx_api.h"
#include "nx_secure_tls_api.h"
#include "nxd_dhcp_client.h"
#include "nxd_dns.h"

#include "configuration.h"
#include "remora.h"
#include "extern.h"


#define NETX_IP_STACK_SIZE  2048
#define NETX_PACKET_COUNT   60
#define NETX_PACKET_SIZE    1536
#define NETX_POOL_SIZE      ((NETX_PACKET_SIZE + sizeof(NX_PACKET)) * NETX_PACKET_COUNT)
#define NETX_ARP_CACHE_SIZE 512
#define NETX_DNS_COUNT      6

#define NETX_IPV4_ADDRESS IP_ADDRESS(10, 10, 10, 10)
#define NETX_IPV4_MASK    IP_ADDRESS(255, 255, 255, 0)

#define DHCP_WAIT_TIME_TICKS (30 * TX_TIMER_TICKS_PER_SECOND)

static UCHAR netx_ip_stack[NETX_IP_STACK_SIZE];
static UCHAR netx_ip_pool[NETX_POOL_SIZE];
static UCHAR netx_arp_cache_area[NETX_ARP_CACHE_SIZE];

NX_IP nx_ip;
NX_PACKET_POOL nx_pool;
NX_DNS nx_dns_client;


UINT network_init(VOID (*ip_link_driver)(struct NX_IP_DRIVER_STRUCT*))
{
	UINT status;

    // Initialize the NetX system.
    nx_system_initialize();

    // Create a packet pool.
    if ((status = nx_packet_pool_create(&nx_pool, "NetX Packet Pool", NETX_PACKET_SIZE, netx_ip_pool, NETX_POOL_SIZE)))
    {
        printf("ERROR: nx_packet_pool_create (0x%08x)\r\n", status);
    }

    // Create an IP instance
    else if ((status = nx_ip_create(&nx_ip,
                  "NetX IP Instance 0",
                  NETX_IPV4_ADDRESS,
                  NETX_IPV4_MASK,
                  &nx_pool,
                  ip_link_driver,
                  netx_ip_stack,
                  NETX_IP_STACK_SIZE,
                  1)))
    {
        nx_packet_pool_delete(&nx_pool);
        printf("ERROR: nx_ip_create (0x%08x)\r\n", status);
    }

    // Enable ARP and supply ARP cache memory
    else if ((status = nx_arp_enable(&nx_ip, (VOID*)netx_arp_cache_area, NETX_ARP_CACHE_SIZE)))
    {
        nx_ip_delete(&nx_ip);
        nx_packet_pool_delete(&nx_pool);
        printf("ERROR: nx_arp_enable (0x%08x)\r\n", status);
    }

    // Enable TCP traffic
    /*else if ((status = nx_tcp_enable(&nx_ip)))
    {
        nx_ip_delete(&nx_ip);
        nx_packet_pool_delete(&nx_pool);
        printf("ERROR: nx_tcp_enable (0x%08x)\r\n", status);
        return status;
    }*/

    // Enable UDP traffic
    else if ((status = nx_udp_enable(&nx_ip)))
    {
        nx_ip_delete(&nx_ip);
        nx_packet_pool_delete(&nx_pool);
        printf("ERROR: nx_udp_enable (0x%08x)\r\n", status);
    }

    // Enable ICMP traffic
    else if ((status = nx_icmp_enable(&nx_ip)))
    {
        nx_ip_delete(&nx_ip);
        nx_packet_pool_delete(&nx_pool);
        printf("ERROR: nx_icmp_enable (0x%08x)\r\n", status);
    }

    return status;
}
//status = nx_packet_data_retrieve(packet_ptr, udpBufferMsg, &bytesRx);
void udp_data_callback(NX_PACKET **packet_ptr)
{
	ULONG bytesRx;
	UINT status = nx_packet_data_retrieve(*packet_ptr, &rxBuffer.rxBuffer, &bytesRx);

	int txlen = 0;
	struct pbuf *txBuf;

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
		__disable_irq();

		// then move the data
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			rxData.rxBuffer[i] = rxBuffer.rxBuffer[i];
		}

		// re-enable thread interrupts
		__enable_irq();
	}


	// allocate pbuf from RAM
	txBuf = pbuf_alloc(PBUF_TRANSPORT, txlen, PBUF_RAM);

	// copy the data into the buffer
	pbuf_take(txBuf, (char*)&txData.txBuffer, txlen);

	// Connect to the remote client
	udp_connect(upcb, addr, port);

	// Send a Reply to the Client
	udp_send(upcb, txBuf);

	// free the UDP connection, so we can accept new clients
	udp_disconnect(upcb);

	// Free the p_tx buffer
	pbuf_free(txBuf);

	// Free the p buffer
	pbuf_free(p);
}
