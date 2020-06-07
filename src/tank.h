#pragma once
#define TNKSZ 46

namespace Tmpl8 
{

class Surface;

class Tank {
	public:
		Tank(char* buffer);
		~Tank();

		void update();
		void draw(Surface* screen);
		void getVerticis(float* arr);
		void move(float dt);
		void rotateTurret(float nr);
		void fromBuffer(char* buf);

		int w, h, newTime, oldTime, lu;
		char id;
		char name[9] = "timklein";
		float x, y, r, tr, v;
		bool u;
		char* nb, * ob;
	private:
		//char* buf, * nb, * ob;
		char* buf;
		float dx, dy, dr, ox, oy, or;


		//int lu;
		//float lx, ly, lr, lv;
		//float dr, dv;
};

class Player : public Tank {
	public:
		Player(char* buffer);
		void rotateTurret(int mx, int my);
		void toBuffer(char* buf);
		
		bool aimWithMouse = true;

	//private:

};

}