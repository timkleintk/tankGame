#include <Math.h>
#include "tank.h"
#include "surface.h" // Pixel
#include "Controls.h" // controls
#include "functions.h" // mostly buffer stuff
#include "windows.h"
#include <stdio.h>


namespace Tmpl8
{

	float rotationSpeed = 0.05f;
	float acceleration = 0.1f;

	float barrelLength = 10;
	int tankLength = 15;
	int tankWidth = 10;

	int turretLength = 10;
	int turretOffset = 3;
	float turretRotationSpeed = 0.1f;

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

		// get button inputs
		if (GetAsyncKeyState(Controls::tRight)) tr += turretRotationSpeed;
		if (GetAsyncKeyState(Controls::tLeft)) tr -= turretRotationSpeed;
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

	/*void Tank::rotateTurret(int mx, int my) {
		tr = atan2f(my - y, mx - x);
	}
	*/
	void Tank::rotateTurret(float nr) {
		tr = nr;
	}

	void Player::rotateTurret(int mx, int my) {
		if (aimWithMouse) tr = atan2f(my - y, mx - x);
	}

	void Player::toBuffer(char* buf) {
		insertIntoBuffer <float> (&x, buf, 1);
		insertIntoBuffer <float> (&y, buf, 5);
		insertIntoBuffer <float> (&r, buf, 9);
		insertIntoBuffer <float> (&v, buf, 13);
		insertIntoBuffer <float> (&tr, buf, 17);
		//insertIntoBuffer <float>(bx, buf, 21);
		//insertIntoBuffer <float>(by, buf, 25);
		//insertIntoBuffer <float>(br, buf, 29);
	}

						  
}