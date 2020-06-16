#include <Math.h>
#include "game.h" // macros
#include "tank.h"
#include "surface.h" // Pixel
#include "Controls.h" // controls
#include "functions.h" // mostly buffer stuff
#include "windows.h"
#include <stdio.h>


/* SUPDATE:
in	sz	description
0	1	control byte
1	4	time int
5	47	client 0
52	47	client 1
99	47	client 2
146	47	client 3
193	47	client 4
240	47	client 5
287	47	client 6
334	47	client 7
*/

/* CUPDATE
in	sz 	description
0	1	control byte
1	4	time int
5	4	x float
9	4 	y float
13	4	r float
17	4	v float
21	4	tr float
25	4	bx float
29	4 	by float
33	4	br float
37	1	id u int
38	8	name chars
46	1	color byte
*/

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

	float maxV = 200.0f; // px s-1
	float correctionV = 300.0f; // px s-1


	Tank::Tank(char* buffer) {
		
		buf = buffer;
		
		bufferOffset = 0; // the newest place
		//stateBuffers = new char*[INTERPRATIO];
		for (int i = 0; i < INTERPRATIO; i++) {
			stateBuffers[i] = &buf[i * TNKSZ];
		}

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

		time(&lu);

		//insertIntoBuffer <int>(&t, stateBuffers[0], TIMEOFFSET); // nts ugly

		newTime = getFromBuffer <int>(nb, TIMEOFFSET);
		oldTime = getFromBuffer <int>(ob, TIMEOFFSET);
	}

	Tank::~Tank() {
	}
	
	void Tank::update(char* updateBuffer) {

		if (u) {

			int nu = getFromBuffer <int>(updateBuffer, TIMEOFFSET);

			printf("lu: %i, nu: %i \n", lu, nu);


			if (nu > lu) { // insert into buffer
				lu = nu;
				for (int i = INTERPRATIO - 1; i > 0; i--) { // loop backwards through stateBuffers
					stateBuffers[i] = stateBuffers[i - 1]; // shift them all one to the right.
				}
				bufferOffset = (bufferOffset + 1) % INTERPRATIO; // change newest to the next
				stateBuffers[0] = &buf[TNKSZ * bufferOffset]; // point to newest stateBuffer
				memcpy(stateBuffers[0], &updateBuffer[SUPDATEHEADER], TNKSZ); // fill stateBuffer with the newest data
			}

			// swap old and new
			/*char* temp = ob;
			ob = nb;
			nb = temp;

			newTime = getFromBuffer <int>(nb, TIMEOFFSET);
			oldTime = getFromBuffer <int>(ob, TIMEOFFSET);*/

			/*if (oldTime < newTime) {
				time(&lu);

				dt = newTime - oldTime;

				ox = getFromBuffer <float>(ob, XOFFSET);
				oy = getFromBuffer <float>(ob, YOFFSET);
				or = getFromBuffer <float>(ob, ROFFSET);

				dx = getFromBuffer <float>(nb, XOFFSET) - ox;
				dy = getFromBuffer <float>(nb, YOFFSET) - oy;
				dr = getFromBuffer <float>(nb, ROFFSET) - or;
			}*/
			u = false; // update has been done
		}

		//float progress = (float)(time() - lu) / (float)(dt);
		//int timePassed = time() - lu;

		////printf("progress: %i%%\n", (int)(progress * 100));
		////x = ox + progress * dx;
		//float nx = ox + dx / dt * timePassed;
		//float dx2 = nx - x;

		////y = oy + progress * dy;
		//float ny = oy + dy / dt * timePassed;
		//float dy2 = ny - y;


		////r = or + progress * dr;
		//r = or + dr / dt * timePassed;

		//float dist = sqrtf(powf(nx - x, 2) + powf(ny - y, 2));
		////printf("dist: %.1f\n", dist);
		//x = nx;
		//y = ny;

		x = getFromBuffer <float>(stateBuffers[0], XOFFSET);
		y = getFromBuffer <float>(stateBuffers[0], YOFFSET);
		r = getFromBuffer <float>(stateBuffers[0], ROFFSET);
		v = getFromBuffer <float>(stateBuffers[0], VOFFSET);
		tr = getFromBuffer <float>(stateBuffers[0], TROFFSET);
		//bx = getFromBuffer <float>(stateBuffers[0], YOFFSET);
		//by = getFromBuffer <float>(stateBuffers[0], YOFFSET);


		
	}

	void Tank::draw(Surface* screen) {
		//screen->Plot(x, y, 0xff00ff); // center

		for (int i = 0; i < INTERPRATIO; i++) {
			screen->Plot(
				(int)getFromBuffer <float>(stateBuffers[i], XOFFSET), 
				(int)getFromBuffer <float>(stateBuffers[i], YOFFSET), 
				0xff0000);

		}

		//screen->Plot((int)getFromBuffer <float>(nb, XOFFSET), (int)getFromBuffer <float>(nb, YOFFSET), 0x00ff00);
		// body

		//float vx[8];
		//this->getVerticis(vx);
		//screen->Line(vx[6], vx[7], vx[0], vx[1], 0xffffff);
		//for (int i = 0; i < 3; i++) {
		//	screen->Line(vx[2*i], vx[2*i+1], vx[2*i+2], vx[2*i+3], 0xffffff); 
		//}
		//
		//// turret
		//screen->Line(
		//	x + cosf(r) * turretOffset, y + sinf(r) * turretOffset, // base
		//	x + cosf(r) * turretOffset + cosf(tr) * turretLength, y + sinf(r) * turretOffset + sinf(tr) * turretLength, // tip
		//	0xff0000);
		

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