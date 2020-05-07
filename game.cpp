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
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	Sprite noise(new Surface("assets/noise.png"), 1);
	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	
	int dir = 1; //right for even rows, left for odd
	int index = 399;
	
	void Game::Tick(float deltaTime)
	{
		noise.Draw(screen, 0, 0);
		Pixel* memory = screen->GetBuffer();
		if (memory[index + 800]) {
			memory[index] = 0xffffff;
			index += 800;
			memory[index] = 0x00ff00;
			dir *= -1;
		}
		else {
			memory[index] = 0xffffff;
			index += dir;
			memory[index] = 0x00ff00;
		}
		//printf("%X\n", memory[index]);
		index++;
	}
};