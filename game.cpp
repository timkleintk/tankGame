#include "game.h"
#include "surface.h"
#include <cstdio> //printf

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

	Sprite noise(new Surface("assets/noise.png"), 1);
	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	
	int dir = 1; //right for even rows, left for odd
	int index = 399;
	
	void Game::Tick(float deltaTime)
	{
		screen->Box(10, 10, 100, 100, 0x00ff00);
	}
};