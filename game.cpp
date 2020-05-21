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
	
	void Game::Init()
	{ 
		printf("init");
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
		
	}
};