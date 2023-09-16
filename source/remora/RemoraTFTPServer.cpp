/*
 * RemoraTFTPServer.cpp
 *
 *  Created on: Sep 15, 2023
 *      Author: Geramy
 */

#include <remora/RemoraTFTPServer.h>

#define NETX_IP_STACK_SIZE  1024
#define NETX_PACKET_COUNT   8
#define NETX_PACKET_SIZE    512
#define NETX_POOL_SIZE      ((NETX_PACKET_SIZE + sizeof(NX_PACKET)) * NETX_PACKET_COUNT)

#define NETX_IPV4_ADDRESS IP_ADDRESS(10, 10, 10, 10)
#define NETX_IPV4_MASK    IP_ADDRESS(255, 255, 255, 0)

static UCHAR netx_ip_stack[NETX_IP_STACK_SIZE];
static UCHAR netx_ip_pool[NETX_POOL_SIZE];

RemoraTFTPServer::RemoraTFTPServer(RemoraNetwork *network) :
networkPtr(network)
{
	// TODO Auto-generated constructor stub

}
/*
void RemoraTFTPServer::InitializeServer() {
	if ((status = nx_packet_pool_create(&this->nx_pool, "NetX Packet Pool", NETX_PACKET_SIZE, netx_ip_pool, NETX_POOL_SIZE)))
	{
		printf("ERROR: nx_packet_pool_create (0x%08x)\r\n", status);
	}

	status = nx_udp_socket_create(&this->networkPtr->nx_ip, this->tftpSocket, "Socket 2", NX_IP_NORMAL, NX_FRAGMENT_OKAY, 0x80, 5);

	// Check status.
	if (status)
	{
		error_counter++;
		return;
	}

	// Bind the UDP socket to the IP port.
	status =  nx_udp_socket_bind(this->remoraUDPSocket, 69, TX_WAIT_FOREVER);

	// Check status.
	if (status)
	{
		error_counter++;
		return;
	}
}

void RemoraTFTPServer::StartServer() {

}

RemoraTFTPServer::tftp_opcode RemoraTFTPServer::Accept()
{

	return 0;
}

bool RemoraTFTPServer::AcceptFlash() {

}

void RemoraTFTPServer::StopServer()
{

}*/

RemoraTFTPServer::~RemoraTFTPServer() {
	// TODO Auto-generated destructor stub
}

