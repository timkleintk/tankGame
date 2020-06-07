#include <Math.h>
#include "tank.h"
#include "surface.h" // Pixel
#include "Controls.h" // controls
#include "functions.h" // mostly buffer stuff
#include "windows.h"
#include <stdio.h>

/*

in	sz 	description
0	4	time int
4	4	x float
8	4 	y float
12	4	r float
16	4	v float
20	4	tr float
24	4	bx float
28	4 	by float
32	4	br float
36	1	id
37	8	name chars
45	1	color byte

*/

#define TIMEOFFSET 0
#define XOFFSET 4
#define YOFFSET 8
#define ROFFSET 12
#define VOFFSET 16
#define TROFFSET 20
#define BXOFFSET 24
#define BYOFFSET 28
#define BROFFSET 32
#define IDOFFSET 36
#define NAMEOFFSET 37
#define COFFSET 45


namespace Tmpl8
{

	float rotationSpeed = 5.0f; // rad s-1
	float acceleration = 100.0f; // px s-2

	float barrelLength = 10; // px
	int tankLength = 15; // px
	int tankWidth = 10; // px

	int turretLength = 10; // px
	int turretOffset = 3; // px
	float turretRotationSpeed = 0.1f; // rad s-1

	float maxV = 300.0f; // px s-1


	Tank::Tank(char* buffer) {
		
		buf = buffer;
		nb = buffer; // new buffer
		ob = buffer + TNKSZ; // old buffer
		ox = x = getFromBuffer <float>(ob, XOFFSET);
		oy = y = getFromBuffer <float>(ob, YOFFSET);
		or = r = getFromBuffer <float>(ob, ROFFSET);

		w = tankLength;
		h = tankWidth;
		
		id = (char)-1;

		u = false;
		//lu = getFromBuffer <float>(buf, TIMEOFFSET);
		//name = "timklein";

		newTime = getFromBuffer <int>(nb, TIMEOFFSET);
		oldTime = getFromBuffer <int>(ob, TIMEOFFSET);
	}

	Tank::~Tank() {
	}

	void Tank::update() {

		if (u) {
			// swap old and new



			char* temp = ob;
			ob = nb;
			nb = temp;

			time(&lu);
			newTime = getFromBuffer <int>(nb, TIMEOFFSET);
			oldTime = getFromBuffer <int>(ob, TIMEOFFSET);

			printf("ot: %i, nt: %i, lu: %i, ct: %i\n", newTime, oldTime, lu, time());

			ox = getFromBuffer <float>(ob, XOFFSET);
			oy = getFromBuffer <float>(ob, YOFFSET);
			or = getFromBuffer <float>(ob, ROFFSET);

			dx = getFromBuffer <float>(nb, XOFFSET) - ox;
			dy = getFromBuffer <float>(nb, YOFFSET) - oy;
			dr = getFromBuffer <float>(nb, ROFFSET) - or;
			u = false;
		}

		float progress = (float)(time() - lu) / (float)(newTime - oldTime);
		x = ox + progress * dx;
		y = oy + progress * dy;
		r = or + progress * dr;
		
	}

	void Tank::draw(Surface* screen) {
		//screen->Plot(x, y, 0xff00ff); // center
		screen->Plot((int)getFromBuffer <float>(ob, XOFFSET), (int)getFromBuffer <float>(ob, YOFFSET), 0xff0000);
		screen->Plot((int)getFromBuffer <float>(nb, XOFFSET), (int)getFromBuffer <float>(nb, YOFFSET), 0x00ff00);
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

	void Tank::move(float deltaTime) {
		float dt =  deltaTime / 1000; // in seconds
		// get button inputs
		if (GetAsyncKeyState(Controls::tRight)) tr += turretRotationSpeed * dt;
		if (GetAsyncKeyState(Controls::tLeft)) tr -= turretRotationSpeed * dt;
		if (GetAsyncKeyState(Controls::right)) r += rotationSpeed * dt;
		if (GetAsyncKeyState(Controls::left)) r -= rotationSpeed * dt;
		if (GetAsyncKeyState(Controls::down)) v -= acceleration * dt;
		if (GetAsyncKeyState(Controls::up)) v += acceleration * dt;


		// turret
		//tr mouse

		// limit values
		if (v < 0) v = 0;
		if (v > maxV) v = maxV;

		// update position
		x += cosf(r) * v * dt;
		y += sinf(r) * v * dt;
	}					

	/*void Tank::rotateTurret(int mx, int my) {
		tr = atan2f(my - y, mx - x);
	}
	*/
	void Tank::rotateTurret(float nr) {
		tr = nr;
	}

	void Tank::fromBuffer(char* buf) {

	}




	// -------------------------------------------------------------

	Player::Player(char* buffer): Tank(buffer) {
		// wejoo gekke constructor structure
	}

	void Player::rotateTurret(int mx, int my) {
		if (aimWithMouse) tr = atan2f(my - y, mx - x);
	}

	void Player::toBuffer(char* buf) {
		insertIntoBuffer <float> (&x, buf, 5);
		insertIntoBuffer <float> (&y, buf, 9);
		insertIntoBuffer <float> (&r, buf, 13);
		insertIntoBuffer <float> (&v, buf, 17);
		insertIntoBuffer <float> (&tr, buf, 21);
		insertIntoBuffer <char> (&id, buf, 37);
		insertIntoBuffer <char[9]> (&name, buf, 38); // nts inelegant?
	}

						  
}