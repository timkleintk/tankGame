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
#include "functions.h"

namespace Tmpl8
{
	
	// -------------------------------------------------------------
	// Initialize the application
	// -------------------------------------------------------------
	
	Player* player;
	float life = 0.0f;
	int currentTime;
	int lastCping;
	int ping = -1;
	int maxPing, minPing;

	int timeOut = 3000;
	float fps;
	

	// -------------------------------------------------------------
	// Game init
	// -------------------------------------------------------------

	void Game::Init()
	{ 
		printf("init\n");
		player = new Player();	
		networkBufferLength = 1024;
		io = new Connection((PCSTR)"212.182.134.29", 8009, recvBuffer, sizeof(recvBuffer));

		// establishing connection to the server -------------------

		printf("connecting to server...\n");
		io->ping();
		time(&lastCping);

		int frame = 0;
		char animation[4] = { "\|/-" };
		printf("waiting on response of server...\n");

		[&] { // waiting for response
			while (ping < 0) {
				while (time() - lastCping < timeOut) {

					if (io->recv() >= 0) {
						if (recvBuffer[0] == (char)CPING) {
							//ping = minPing = maxPing = time() - intFromBuf(networkBuffer, 1);
							ping = minPing = maxPing = time() - getFromBuffer <int> (recvBuffer, 1);

							printf("\b\b\b\nRecvd response in %d miliseconds\n", ping);
							io->ping();
							return;
						}
					}
				}
				printf("Timed out! maybe the packet got lost. trying again...\n");
				io->ping();
				time(&lastCping);
			}
		}();
		
		// ---------------------------------------------------------

	}


	// -------------------------------------------------------------
	// Main application tick function
	// -------------------------------------------------------------
	
	void Game::Tick(float deltaTime) {
		time(&currentTime);


		// networking stuff ----------------------------------------

		if (io->recv() >= 0) { // handeling server recvs
			switch ((int)recvBuffer[0]) {
			case CPING:
				// return of the ping!
				ping = currentTime - lastCping; // currenttime has just been updated, so it seems fair to use it instead of time()
				if (ping < minPing) { minPing = ping; }
				if (ping > maxPing) { maxPing = ping; }

				lastCping = currentTime;
				//printf("PING! (%d)\n", ping);
				io->ping();
				break;
			case SPING:
				// return the ping!
				break;
			case CUPDATE:
				// this is not supposed to happen
				break;
			case SUPDATE:
				// update internal game state
				break;
			default:
				// this is not supposed to happen
				break;
			}
		}

		if (currentTime - lastCping > timeOut) {
			printf("cPing timed out, maybe the packet got lost\n");
			lastCping = currentTime;
			io->ping();
		}

		// ---------------------------------------------------------

		screen->Clear(0);


		// player stuff --------------------------------------------

		player->move(deltaTime);
		player->rotateTurret(mouseX, mouseY);
		player->draw(screen);
		
		player->toBuffer(sendBuffer);
		io->send(CUPDATE, sendBuffer, sizeof(sendBuffer));

		// debugging stuff -----------------------------------------

		fps = 1 / (deltaTime / 1000);
		char str[32];
		//sprintf(str, "fps:%.1f", fps);
		sprintf(str, "fps:%.1f", fps);
		screen->Print(str, 3, 3, 0xff0000);
		sprintf(str, "png: %d %d %d", minPing, ping, maxPing);
		screen->Print(str, 3, 11, 0xffffff);

	}


	// -----------------------------------------------------------
	// exiting
	// -----------------------------------------------------------

	void Game::Shutdown() { printf("goodbye\n"); }


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
			displayBuffer(recvBuffer, networkBufferLength);
			char msg[6] = "1test";
			io->send(CUPDATE, msg, sizeof(msg)); // ping the server
		}
	}
	
	// -------------------------------------------------------------
	// functions
	// -------------------------------------------------------------

	void displayBuffer(char* buf, int len) {
		printf("buffer:\n");
		for (int i = 0; i < len; i++) {
			printf("%c",buf[i]);
		}
		printf("\n");
	}

	void time(int *i) {*i = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();}

	int time() {return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();}

	/*int intFromBuf(char* buf, int offset) {
		return *(reinterpret_cast<unsigned int*>(&buf[offset]));
	}*/

	//void Game::showPing(int x, int y) {
	//	//int score = 123;
	//	char text[10];
	//	//sprintf(text, "Ping: %d", io->ping());
	//	screen->Print(text, x, y, 0xff0000);
	//}

};