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

// -----------------------------------------------------------------

// notes go here

// -----------------------------------------------------------------

namespace Tmpl8
{
	
	// -------------------------------------------------------------
	// Declare variables
	// -------------------------------------------------------------
	
	Player* player;
	Tank* players[PLAYERS];
	bool connected[PLAYERS]; // nts look at type

	float life = 0.0f;
	int currentTime;
	int lastCping, lastSUpdate;
	int ping = -1;
	int maxPing, minPing;

	int timeOut = 3000;
	float fps;
	
	//int playerBufferOffset = 0; // nts bad name -_-

	// -------------------------------------------------------------
	// Game init
	// -------------------------------------------------------------

	void Game::Init()
	{ 
		printf("init\n");
		//player = new Player(&playerBuffer[0]);	
		player = new Player(playerBuffer);
		//players[0] = new Tank(playerBuffer);
		//networkBufferLength = 1024;
		io = new Connection((PCSTR)"212.182.134.29", 8009, recvBuffer, sizeof(recvBuffer));
		//io = new Connection((PCSTR)"192.168.2.18", 8009, recvBuffer, sizeof(recvBuffer));
		time(&lastSUpdate); // for if currentTime < 0;


		// establishing connection to the server -------------------
		printf("connecting to server...\n");
		player->id = io->connect();
		//connected[player->id] = 1;
		printf("connected! id: %d\n", (int)player->id);
	}


	// -------------------------------------------------------------
	// Main application tick function
	// -------------------------------------------------------------
	
	void Game::Tick(float deltaTime) {
		time(&currentTime);


		// networking stuff ----------------------------------------

		if (io->recv() >= 0) { // handeling server recvs
			int sTime;
			
			//printf("Incomming message! cb: %x | %i | %f.1f | \n", (int)recvBuffer[0], getFromBuffer <int>(recvBuffer, 1), getFromBuffer <int>(recvBuffer, 1));

			switch ((int)recvBuffer[0]) {
			case CPING:
				// return of the ping!
				lastCping = currentTime;

				ping = currentTime - lastCping; // currenttime has just been updated, so it seems fair to use it instead of time()
				if (ping < minPing) { minPing = ping; }
				if (ping > maxPing) { maxPing = ping; }

				//printf("PING! (%d)\n", ping);
				//io->ping();
				break;
			case SPING:
				// return the ping!
				break;
			case CUPDATE:
				// this is not supposed to happen
				break;
			case SUPDATE:
				// update internal game state
				sTime = getFromBuffer <int>(recvBuffer, TIMEOFFSET);

				if (sTime > lastSUpdate) { // newer
					lastSUpdate = sTime;
					updatePlayerBuffer();
				}
				
				break;
			case SASSIGN:
				player->id = getFromBuffer <char>(recvBuffer, 2);
				printf("new id: %i\n", (int)getFromBuffer <char>(recvBuffer, 2)); // nts this gets triggered sometimes seemingly random... wtf?
				break;
			default:
				// this is not supposed to happen
				break;
			}
		}


		// ---------------------------------------------------------

		screen->Clear(0);


		// player stuff --------------------------------------------

		updatePlayers(deltaTime);

		
		

		// update server -------------------------------------------

		player->toBuffer(sendBuffer);
		insertIntoBuffer <int> (&currentTime, sendBuffer, TIMEOFFSET);
		io->send(CUPDATE, sendBuffer, sizeof(sendBuffer));


		// debugging stuff -----------------------------------------

		fps = 1 / (deltaTime / 1000);
		char str[32];
		//sprintf(str, "fps:%.1f", fps);
		sprintf(str, "fps:%.1f", fps);
		screen->Print(str, 3, 3, 0xff0000);
		sprintf(str, " id: %i %i%i%i%i%i%i%i%i", player->id, connected[0], connected[1], connected[2], connected[3], connected[4], connected[5], connected[6], connected[7]);
		screen->Print(str, 3, 11, 0xffffff);
		
		//displayBuffers(screen->GetBuffer());
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
			player->fire();
			//displayBuffer(recvBuffer, networkBufferLength);
			//displayBuffer(playerBuffer, sizeof(playerBuffer));
		}
	}
	
	void Game::updatePlayerBuffer() {
		memcpy(updateBuffer, recvBuffer, sizeof(updateBuffer));

		for (int i = 0; i < PLAYERS; i++) { // for every player
			// update connected array
			bool n = getFromBuffer <INT8>(recvBuffer, SUPDATEHEADER + i * TNKSZ) > 0; // if controll byte is not null
			if (connected[i] < n) { // n == 1; c[i] == 0; new connection!
				printf("Player %i joined\n", i);
				connected[i] = true;
				players[i] = new Tank(
					&playerBuffer[INTERPRATIO * TNKSZ * i], 
					&updateBuffer[SUPDATEHEADER + i * TNKSZ]); 
				players[i]->id = (char)i; // make sure the tank understands its place
				//players[i]->nb = &playerBuffer[INTERPRATIO * TNKSZ * i + playerBufferOffset * TNKSZ];
				//players[i]->ob = &playerBuffer[INTERPRATIO * TNKSZ * i + (playerBufferOffset + 1) % INTERPRATIO * TNKSZ];
			} else if (connected[i] > n) { // n == 0; c[i] == 1; lost connection!
				printf("player %i left\n", i);
				connected[i] = false;
				// delete current tank? maybe not???
			} else if (n) { // player is still connected
				players[i]->u = true;
			}
		}
		//playerBufferOffset = 1 - playerBufferOffset; // flip the value
		//memcpy(playerBuffer, &recvBuffer[6], sizeof(playerBuffer));
	}
	
	void Game::updatePlayers(float deltaTime) {
		// update the player ---------------------------------------
	
		//players[0]->update(playerBuffer[6]);

		
		/*players[0]->x = getFromBuffer <float>(recvBuffer, 5);
		players[0]->y = getFromBuffer <float>(recvBuffer, 9);
		players[0]->r = getFromBuffer <float>(recvBuffer, 13);*/

		for (int i = 0; i < 8; i++) {
			if (connected[i]) {
				players[i]->update(deltaTime);
				players[i]->draw(screen);
			}
		} 

		//players[0]->update();

		player->move(deltaTime);
		player->rotateTurret(mouseX, mouseY);
		player->draw(screen);
		//screen->Plot(player->x, player->y, 0xffffff);
		
		
		//players[0]->draw(screen);
	}

	// -------------------------------------------------------------
	// functions
	// -------------------------------------------------------------

	//void displayBuffers(Pixel* mem) {
	//	for (int i = 0; i < 0; i++) {

	//	}
	//}

	void displayBuffer(char* buf, int len) {
		printf("buffer:\n");
		for (int i = 0; i < len; i++) {
			printf("%x",buf[i]);
		}
		printf("\n");
	}

	void time( int *i) {*i = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();}

	int time() {return (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();}


};