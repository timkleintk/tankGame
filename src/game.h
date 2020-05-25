#pragma once

namespace Tmpl8 {

class Surface;
class Connection;

class Game {
	public:
		void SetTarget( Surface* surface ) { screen = surface; }
		void Init();
		void Shutdown();
		void Tick( float deltaTime );
		void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
		void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
		void MouseMove(int x, int y); // implementation in game.cpp
		void KeyUp( int key ) { /* implement if you want to handle keys */ }
		void KeyDown(int key); // implementation in game.cpp
	private:
		Surface* screen;
		Connection* io;
		char networkBuffer[1024];
		int mouseX, mouseY, networkBufferLength;
		void showPing(int x, int y);
};	

void displayBuffer(char* buf, int len); // just a function
void insertIntIntoBuffer(int i, char *buf, int offset);
void insertIntIntoBuffer(unsigned __int64 i, char *buf, int offset);
void time(int *i);
int intFromBuf(char *buf, int offset);
//void recv();


}; // namespace Tmpl8