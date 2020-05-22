#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <cstdio> //printf

#include "game.h"
#include "surface.h"
#include "tank.h"

namespace Tmpl8
{

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	
	Tank* player;
	float life = 0.0f;

	void Game::Init()
	{ 
		printf("init");


		player = new Tank();
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
		//player->rotateTurret(100, 100);
		player->draw(screen);
		//life += deltaTime;
		//printf("dt: %f\n", life);		
	}
};