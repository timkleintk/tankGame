#include <Math.h>
#include "tank.h"
#include "surface.h" // Pixel
#include "windows.h"
#include <stdio.h>

namespace Tmpl8
{
	
	struct Controls {
		static const int up = 0x57; // w
		static const int down = 0x53; // s
		static const int left = 0x41; // a
		static const int right = 0x44; // d
		//static const int tLeft = 0x5A; // z
		static const int tLeft = 0xBC; // ,
		//static const int tRight = 0x58; // x
		static const int tRight = 0xBE; // .
		static const int fire = 0x20; // " "
	}; 


	float rotationSpeed = 0.05f;
	float acceleration = 0.1f;

	float barrelLength = 10;
	int tankLength = 15;
	int tankWidth = 10;

	int turretLength = 10;
	int turretOffset = 3;

	float maxV = 3.0f;


	Tank::Tank() {
		x = 100;
		y = 100;
		w = tankLength;
		h = tankWidth;
		v = 0.0f;
		r = 0.0f;
		tr = 0.0f;
	}

	Tank::~Tank() {
	}

	void Tank::draw(Surface* screen) {
		//screen->Plot(x, y, 0xff00ff); // center
		
		// body
		float vx[8];
		this->getVerticis(vx);
		screen->Line(vx[6], vx[7], vx[0], vx[1], 0xffffff);
		for (int i = 0; i < 3; i++) {
			screen->Line(vx[2*i], vx[2*i+1], vx[2*i+2], vx[2*i+3], 0xffffff); 
		}
		// clarity lines above and below turret

		//screen->Line(
		//	x + cosf(r) * turretOffset, y + sinf(r) * turretOffset + 1, // base
		//	x + cosf(r) * turretOffset + cosf(tr) * turretLength, y + sinf(r) * turretOffset + sinf(tr) * turretLength + 1, // tip
		//	0);
		//
		//screen->Line(
		//	x + cosf(r) * turretOffset, y + sinf(r) * turretOffset - 1, // base
		//	x + cosf(r) * turretOffset + cosf(tr) * turretLength, y + sinf(r) * turretOffset + sinf(tr) * turretLength - 1, // tip
		//	0);
		//
		//screen->Line(
		//	x + cosf(r) * turretOffset + 1, y + sinf(r) * turretOffset, // base
		//	x + cosf(r) * turretOffset + cosf(tr) * turretLength + 1, y + sinf(r) * turretOffset + sinf(tr) * turretLength, // tip
		//	0);
		//
		//screen->Line(
		//	x + cosf(r) * turretOffset - 1, y + sinf(r) * turretOffset, // base
		//	x + cosf(r) * turretOffset + cosf(tr) * turretLength - 1, y + sinf(r) * turretOffset + sinf(tr) * turretLength, // tip
		//	0);

		// turret
		screen->Line(
			x + cosf(r) * turretOffset, y + sinf(r) * turretOffset, // base
			x + cosf(r) * turretOffset + cosf(tr) * turretLength, y + sinf(r) * turretOffset + sinf(tr) * turretLength, // tip
			0xff0000);
		

	}

	 void Tank::getVerticis(float *arr) { // takes a float array and inserts the verticis
		 
			arr[0] = x - w / 2.0f * cosf(r) - h / 2.0f * sinf(r); // linksonder
			arr[1] = y - w / 2.0f * sinf(r) + h / 2.0f * cosf(r);
			arr[2] = x + w / 2.0f * cosf(r) - h / 2.0f * sinf(r); // rechtsonder
			arr[3] = y + w / 2.0f * sinf(r) + h / 2.0f * cosf(r);
			arr[4] = x + w / 2.0f * cosf(r) + h / 2.0f * sinf(r); // rechtsboven
			arr[5] = y + w / 2.0f * sinf(r) - h / 2.0f * cosf(r);
			arr[6] = x - w / 2.0f * cosf(r) + h / 2.0f * sinf(r); // linksboven
			arr[7] = y - w / 2.0f * sinf(r) - h / 2.0f * cosf(r);
		
	 }

	void Tank::move(float dt) {
		//dt = 1.0f;

		// get button inputs
		if (GetAsyncKeyState(Controls::right)) r += rotationSpeed;
		if (GetAsyncKeyState(Controls::left)) r -= rotationSpeed;
		if (GetAsyncKeyState(Controls::down)) v -= acceleration;
		if (GetAsyncKeyState(Controls::up)) v += acceleration;


		// turret
		//tr mouse

		// limit values
		if (v < 0) v = 0;
		if (v > maxV) v = maxV;

		// update position
		x += cosf(r) * v;
		y += sinf(r) * v;
	}					

	void Tank::rotateTurret(int mx, int my) {
		tr = atan2f(my - y, mx - x);
	}

	void Tank::rotateTurret(float nr) {
		tr = nr;
	}
						  
}