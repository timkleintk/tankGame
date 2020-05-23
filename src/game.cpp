#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <cstdio> //printf

#include "game.h"
#include "Controls.h"
#include "surface.h"
#include "tank.h"

#include "socketCode/sio_client.h" // does this work?
#include "socketCode/sio_message.h"
#include "socketCode/sio_socket.h"

#include "socketCode/internal/sio_client_impl.h"
#include "socketCode/internal/sio_packet.h"

namespace Tmpl8
{
	
	sio::client h;
	h.connect("http://127.0.0.1:3000");

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	
	Player* player;
	float life = 0.0f;

	void Game::Init()
	{ 
		printf("init");
		//Controls::aimWithMouse = true;

		player = new Player();
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		printf("goodbye");
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	
	void Game::Tick(float deltaTime) {
		screen->Clear(0);
		player->move(deltaTime);
		player->rotateTurret(mouseX, mouseY);
		player->draw(screen);
	}

	void Game::MouseMove(int x, int y) {
		mouseX += x; mouseY += y;
		player->aimWithMouse = true;
	}

	void Game::KeyDown(int key) {
		//printf("key pressed: %i\n", key);
		if (GetAsyncKeyState(Controls::tLeft) || GetAsyncKeyState(Controls::tRight)) player->aimWithMouse = false; // nts ugh
	}
};