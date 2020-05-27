#pragma once

#ifndef WIN32_LEAN_AND_MEAN // compensating for gayness of winsock
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include "functions.h"
// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")


enum cBytes {
	SPING = (char)0x00,		// 0 - ping from server
	CPING = (char)0x01,		// 1 - ping from client
	SUPDATE = (char)0x02,	// 2 - update from server
	CUPDATE = (char)0x03	// 3 - update from Client
};



namespace Tmpl8 
{

class Connection {
	public:
		Connection(PCSTR ip, u_short port, char *buf, int len) {
		//Connection(char ip, u_short port, char *buf, int len) {
			// setup winsock
			int wsOk = WSAStartup(MAKEWORD(2, 2), &data);
			if (wsOk != 0) {
				throw std::system_error(WSAGetLastError(), std::system_category(), "WSAStartup Failed");
			}

			// setup socket
			sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (sock == INVALID_SOCKET) {
				throw std::system_error(WSAGetLastError(), std::system_category(), "Error opening socket");
			}

			// make non blocking
			u_long mode = 1; // 1 = non-blocking
			if (ioctlsocket(sock, FIONBIO, &mode) == SOCKET_ERROR) {
				closesocket(sock);
				WSACleanup();
				throw std::system_error(WSAGetLastError(), std::system_category(), "Error setting non-blocking");
			}

			// setup server
			server.sin_family = AF_INET; // AF_INET = IPv4 addresses
			server.sin_port = htons(port); // Little to big endian conversion
			inet_pton(AF_INET, ip, &server.sin_addr); // Convert from string to byte array
			addrSize = sizeof(server);

			// setup local variables
			buffer = buf;
			bufferSize = len;
		}
		
		~Connection() {
			closesocket(sock);
			WSACleanup();
		}

		void send(int c, char *msg, int len) {
			msg[0] = (char)c;
			int sendOk = sendto(sock, msg, len, 0, (sockaddr*)&server, sizeof(server));
			if (sendOk == SOCKET_ERROR) {
				throw std::system_error(WSAGetLastError(), std::system_category(), "send failed");
			}
		}

		void ping() {
			char msg[5];
			int sendTime,recvTime;
			time(&sendTime);
			//insertIntIntoBuffer(sendTime, msg, 1);
			insertIntoBuffer <int> (&sendTime, msg, 1);
			send(CPING, msg, sizeof(msg));
		}

		void sendUpdate(char *msg, int len) {
			msg[0] = CUPDATE;

		}

		int recv() {
			int recvOk = recvfrom(sock, buffer, bufferSize, 0, (sockaddr*)&server, &addrSize);
			//if (recvOk < 0) {
				//throw std::system_error(WSAGetLastError(), std::system_category(), "recv failed");
			//}
			return recvOk;
		}

	private:
		WSADATA data;
		SOCKET sock;

		sockaddr_in server;
		char* buffer;
		int bufferSize, addrSize;
};

}