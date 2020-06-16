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

// -----------------------------------------------------------------

/* SPING
in	sz	description
0	1	control byte
1	4	time int
*/

/* CPING
in	sz	description
0	1	control byte
1	4	time int
*/


/* SUPDATE:
in	sz	description
0	1	control byte
1	4	time int
5	47	client 0
52	47	client 1
99	47	client 2
146	47	client 3
193	47	client 4
240	47	client 5
287	47	client 6
334	47	client 7
*/

/* CUPDATE
in	sz 	description
0	1	control byte
1	4	time int
5	4	x float
9	4 	y float
13	4	r float
17	4	v float
21	4	tr float
25	4	bx float
29	4 	by float
33	4	br float
37	1	id u int
38	8	name chars
46	1	color byte
*/

/* CASSIGN
in	sz	description
0	1	control byte
*/

/* SASSIGN
in	sz	description
0	1	control byte
1	1	id
*/

// -----------------------------------------------------------------

enum cBytes {
	SPING = (char)0x00,		// 0 - ping from server
	CPING = (char)0x01,		// 1 - ping from client
	SUPDATE = (char)0x02,	// 2 - update from server
	CUPDATE = (char)0x03,	// 3 - update from Client
	SASSIGN = (char)0x04,	// 4 - recv index from server
	CASSIGN = (char)0x05	// 4 - ask the server for an id
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

		char connect() {
			int sendTime = time();
			int timeOut = 10000;
			char msg;
			//msg[0] = CASSIGN;
			send(CASSIGN, &msg, 1);
			while (1) {
				while (time() - sendTime < timeOut) {

					if (recv() >= 0) {
						if (buffer[0] == (char)SASSIGN) {
							//return getFromBuffer<char>(buffer, 1);
							return buffer[1];
						}
					}
				}
				printf("Timed out! maybe the packet got lost. trying again...\n");
				send(CASSIGN, &msg, 1);
				time(&sendTime);
			}
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