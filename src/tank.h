#pragma once

namespace Tmpl8 
{

class Surface;

class Tank {
	public:
		Tank(char* buffer, char* updateBuffer);
		~Tank();

		void update(float deltaTime);
		void draw(Surface* screen);
		void getVerticis(float* arr);
		void rotateTurret(float nr);
		void fromBuffer(char* buf);

		char id;
		bool u;
		char name[9] = "timklein";
		float x, y, r, tr, v;
		
		bool BULLET;
		float bx, by, br;

		int c;

	private:
		int w, h;


		float ox, nx, dx;
		float oy, ny, dy;
		int	  ou, nu;
		int   ot, nt, dt;
		float obx, oby;

		bool extrapolating;
		float ov, nv, dv, dvdt;
		float or, nr, dr, drdt;
		float otr, ntr, dtr;

		int timings[INTERPRATIO];
		char* buf;
		int bufferOffset, latency;
		char* updateBuffer;
		char* stateBuffers[INTERPRATIO];

};

class Player : public Tank {
	public:
		Player(char* buffer);
		void rotateTurret(int mx, int my);
		void toBuffer(char* buf);
		void move(float dt);
		void fire();
		void destroyBullet();
		bool aimWithMouse = true;

	//private:

};

}