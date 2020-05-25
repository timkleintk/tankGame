#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <cstdio> //printf

//#include <ctime>
#include <chrono>

#include "game.h"
#include "network.h"
#include "Controls.h"
#include "surface.h"
#include "tank.h"


//enum cBytes {
//	SPING = (char) 0x00,		// 0 - ping from server
//	CPING = (char) 0x01,		// 1 - ping from client
//	SUPDATE = (char) 0x02,	// 2 - update from server
//	CUPDATE = (char) 0x03	// 3 - update from Client
//};

namespace Tmpl8
{
	
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	
	Player* player;
	float life = 0.0f;
	int currentTime;
	//unsigned __int64 currentTime;


	//const int networkBufferLength = 1024;

	// -----------------------------------------------------------
	// Game init
	// -----------------------------------------------------------

	void Game::Init()
	{ 
		printf("init\n");
		networkBufferLength = 1024;
		io = new Connection((PCSTR)"212.182.134.29", 8009, networkBuffer, sizeof(networkBuffer));
		//io->ping();
		printf("pingTime: %i", io->ping());

		player = new Player();	
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		printf("goodbye\n");
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	
	void Game::Tick(float deltaTime) {
		time(&currentTime);
		//printf("pingTime: %i", io->ping());
		
		printf("pingTime: %i\n", io->ping());

		//char msg[5];
		//insertIntIntoBuffer(currentTime, msg, 1);
		//io->send(CPING, msg, sizeof(msg));	

		//if (io->recv() > 0) {
		//	//displayBuffer(networkBuffer, networkBufferLength);
		//	if (networkBuffer[0] == CPING) { // received back the cping
		//		printf("CPING recv\n");
		//		int serverTime = intFromBuf(networkBuffer, 1);
		//		printf("server time:  %i\n", serverTime);
		//		printf("current time: %i\n", currentTime);
		//		printf("difference:   %i\n\n", currentTime - serverTime);
		//	}
		//}
		screen->Clear(0);
		//recv();
		player->move(deltaTime);
		player->rotateTurret(mouseX, mouseY);
		player->draw(screen);
		//showPing(10, 10);

	}

	// -----------------------------------------------------------
	// other stuff
	// -----------------------------------------------------------

	void Game::MouseMove(int x, int y) {
		mouseX += x; mouseY += y;
		player->aimWithMouse = true;
	}

	void Game::KeyDown(int key) {
		//printf("key pressed: %i\n", key);
		if (GetAsyncKeyState(Controls::tLeft) || GetAsyncKeyState(Controls::tRight)) player->aimWithMouse = false; // nts ugh
		if (key == 44) { // spacebar
			displayBuffer(networkBuffer, networkBufferLength);
			char msg[6] = "1test";
			io->send(CUPDATE, msg, sizeof(msg)); // ping the server
		}
	}
	
	void displayBuffer(char* buf, int len) {
		printf("buffer:\n");
		for (int i = 0; i < len; i++) {
			printf("%c",buf[i]);
		}
		printf("\n");
	}

	void insertIntIntoBuffer(int i, char* buf, int offset) {
		for (int x = 0; x < sizeof(int); x++) {
			buf[offset + x] = (i >> (8 * x)) & 0xff;
		}
	}
	void insertIntIntoBuffer(unsigned __int64 i, char* buf, int offset) {
		//printf("-%x-", i);
		for (int x = 0; x < sizeof(int); x++) {
			buf[offset + x] = (i >> (8 * x)) & 0xff;
			//printf("-%x- ", buf[offset + x]);
		}
		//printf("\n");
	}

	void time(int *i) {
		*i = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	int intFromBuf(char* buf, int offset) {
		return *(reinterpret_cast<unsigned int*>(&buf[offset]));
	}

	void Game::showPing(int x, int y) {
		//int score = 123;
		char text[10];
		sprintf(text, "Ping: %d", io->ping());
		screen->Print(text, x, y, 0xff0000);
	}

};