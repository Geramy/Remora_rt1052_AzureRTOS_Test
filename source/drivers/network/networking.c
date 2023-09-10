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
    else if ((status = nx_tcp_enable(&nx_ip)))
    {
        nx_ip_delete(&nx_ip);
        nx_packet_pool_delete(&nx_pool);
        printf("ERROR: nx_tcp_enable (0x%08x)\r\n", status);
        return status;
    }

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
