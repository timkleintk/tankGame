#pragma once

#define PLAYERS 8
#define INTERPRATIO 2


#define TNKSZ 47

#define SUPDATEHEADER 5

#define TIMEOFFSET 1
#define XOFFSET 5
#define YOFFSET 9
#define ROFFSET 13
#define VOFFSET 17
#define TROFFSET 21
#define BXOFFSET 25
#define BYOFFSET 29
#define BROFFSET 33
#define IDOFFSET 37
#define NAMEOFFSET 38
#define COFFSET 46


namespace Tmpl8 {

class Surface;
class Connection;

class Game {
	public:
		void SetTarget( Surface* surface ) { screen = surface; }
		void Init();
		void Shutdown() {};
		void Tick( float deltaTime );
		void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
		void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
		void MouseMove(int x, int y); // implementation in game.cpp
		void KeyUp( int key ) { /* implement if you want to handle keys */ }
		void KeyDown(int key); // implementation in game.cpp
	private:
		void updatePlayers(float deltaTime);
		void updatePlayerBuffer();
		Surface* screen;
		Connection* io;
		char recvBuffer[1024];
		char sendBuffer[47]; // nts parameterize this
		char updateBuffer[PLAYERS * TNKSZ + SUPDATEHEADER];
		char playerBuffer[INTERPRATIO * PLAYERS * TNKSZ]; // nts parameterize this

		int mouseX, mouseY, networkBufferLength; // nbl kan weg
		//void showPing(int x, int y);
};	


}; // namespace Tmpl8