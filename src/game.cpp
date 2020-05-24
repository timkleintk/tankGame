#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <cstdio> //printf

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

	//const int networkBufferLength = 1024;

	// -----------------------------------------------------------
	// Game init
	// -----------------------------------------------------------

	void Game::Init()
	{ 
		printf("init\n");
		networkBufferLength = 1024;
		io = new Connection((PCSTR)"212.182.134.29", 8009, networkBuffer, sizeof(networkBuffer));

		io->ping();

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
		if (io->recv() > 0) {
			displayBuffer(networkBuffer, networkBufferLength);
		}
		screen->Clear(0);
		player->move(deltaTime);
		player->rotateTurret(mouseX, mouseY);
		player->draw(screen);
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
};