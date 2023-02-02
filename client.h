#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
 
#include <thread>
#include <mutex>


#include "helper_udp.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class Client
{
	public:
		Client();
		int Start(const char* ipaddress, const char* port);
		int Interact(void);
		void Stop(void);

		player_state server_player; // received data from client and save it in this variable.
		player_state client_player; // send this to client

	private:
		 
		WSADATA wsaData;
		SOCKET ConnectSocket = INVALID_SOCKET;
		struct addrinfo *result = NULL, *ptr = NULL, hints;
		int iResult;


};

Client::Client()
{

}

int Client::Start(const char* ipaddress, const char* port)
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	} 


	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ipaddress, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	std::cout << "Server connected" << std::endl;

}

int  Client::Interact( void )
{
	// iResult = send(ConnectSocket, recvbuf, sizeof(recvbuf), 0);
	iResult = send(ConnectSocket, (char *) &client_player, sizeof(client_player), 0);

	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
	}

	// iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	iResult = recv(ConnectSocket, (char *) &server_player, sizeof(server_player), 0);
	server_player.print();
	if (iResult > 0)
		printf("Server says: %d\n", server_player.score);

	return iResult;
}


void Client::Stop(void)
{
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}


//int __cdecl main(int argc, char **argv)
//{
//
//	if (argc < 3) {
//		std::cout << "Usage: " << argv[0] << " hostname port" << std::endl;
//	} else {
//		Client client;
//
//		int status = client.Start( argv[1], argv[2] );
//		if ( status ) 
//		{
//			while( status > 0 ) {
//				client.client_player.score += 1;
//				status = client.Interact();
//			}
//
//			client.Stop();
//		}
//	}
//
//	return 0;
//}