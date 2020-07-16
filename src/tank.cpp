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

	int lag = 300; // ms

	Tank::Tank(char* buf, char* updateBuffer) {
		
		this->buf = buf;
		this->updateBuffer = updateBuffer;

		bufferOffset = 0; // the newest place
		//stateBuffers = new char*[INTERPRATIO];
		for (int i = 0; i < INTERPRATIO; i++) {
			stateBuffers[i] = &buf[i * TNKSZ];
		}

		w = tankLength;
		h = tankWidth;
		
		id = (char)-1;

		c = 0x000000;

		u = false;
		//ou = getFromBuffer <float>(buf, TIMEOFFSET);
		//name = "timklein";

		time(&ou);

		//insertIntoBuffer <int>(&t, stateBuffers[0], TIMEOFFSET); // nts ugly

		//newTime = getFromBuffer <int>(nb, TIMEOFFSET);
		//oldTime = getFromBuffer <int>(ob, TIMEOFFSET);
	}

	Tank::~Tank() {
	}
	
	void Tank::update(float deltaTime) {

		if (u) {
			nu = getFromBuffer <int>(updateBuffer, TIMEOFFSET); // new update time

			if (nu > ou) { // if update is newer then the most recent known update
				ou = nu; // old update is the new update
				for (int i = INTERPRATIO - 1; i > 0; i--) { // loop backwards through stateBuffers
					stateBuffers[i] = stateBuffers[i - 1]; // shift them all one to the right.
					timings[i] = timings[i - 1];
				}
				bufferOffset = (bufferOffset + 1) % INTERPRATIO; // change newest to the next
				stateBuffers[0] = &buf[TNKSZ * bufferOffset]; // point to newest stateBuffer
				timings[0] = nu;
				memcpy(stateBuffers[0], updateBuffer, TNKSZ); // fill stateBuffer with the newest data
			}

			u = false; // update has been done
		}

		int now = time() - lag;

		for (int i = 0; i < INTERPRATIO; i++) {

			if (i == INTERPRATIO - 1) {
				// extrapolate
				printf("Xtrapol8\n");
				break;
			}

			

			if (timings[i] >= now && timings[i + 1] <= now) {

				ot = timings[i + 1];
				nt = timings[i];
				dt = nt - ot;

				ox = x;
				ox = getFromBuffer <float>(stateBuffers[i + 1], XOFFSET);
				nx = getFromBuffer <float>(stateBuffers[i], XOFFSET);
				dx = nx - ox;

				oy = y;
				oy = getFromBuffer <float>(stateBuffers[i + 1], YOFFSET);
				ny = getFromBuffer <float>(stateBuffers[i], YOFFSET);
				dy = ny - oy;

				or = r;
				or = getFromBuffer <float>(stateBuffers[i + 1], ROFFSET);
				nr = getFromBuffer <float>(stateBuffers[i], ROFFSET);
				dr = nr - or ;
				drdt = dr / (float)dt;

				ov = getFromBuffer<float>(stateBuffers[i + 1], VOFFSET);
				nv = getFromBuffer<float>(stateBuffers[i], VOFFSET);
				dv = nv - ov;
				dvdt = dv / (float)dt;

				otr = getFromBuffer<float>(stateBuffers[i + 1], TROFFSET);
				ntr = getFromBuffer<float>(stateBuffers[i], TROFFSET);
				dtr = ntr - otr;



				break;
			}
		}


		// nts make dependant on interpratio!
		//int now = time() - lag
		/*if (now < timings[1]) {
			printf("time travel\n");
		} else if (now > timings[0]) { // extrapolating
			//printf("extrapolate\n");
			if (!extrapolating) { // start extrapolating
				or = getFromBuffer<float>(stateBuffers[1], ROFFSET);
				nr = getFromBuffer<float>(stateBuffers[0], ROFFSET);
				dr = nr - or;
				drdt = dr / (float)dt;
				
				ov = getFromBuffer<float>(stateBuffers[1], VOFFSET);
				nv = getFromBuffer<float>(stateBuffers[0], VOFFSET);
				dv = nv - ov;
				dvdt = dv / (float)dt;

				v = nv;
				r = nr;

				extrapolating = true;
				c = 0xffff00;
			}

			v += dvdt * deltaTime;
			r += drdt * deltaTime;

			x += cosf(r) * v * deltaTime / 1000; // v is in px s-2, deltaTime in ms
			y += sinf(r) * v * deltaTime / 1000;
			


		} else { // interpolate
			extrapolating = false; */
			//printf("interpolate\n");
		float progress = (float)(now - ot) / (float)dt;
		//printf("progress: %f\n", progress);
		if (progress > 1) { // Xtrapolating
			printf("ot: %i, now: %i, nt: %i\n", ot - ot, now - ot, nt - ot);
			c = 0x00ff00;
			v += dvdt * deltaTime;
			r += drdt * deltaTime;

			x += cosf(r) * v * deltaTime / 1000; // v is in px s-2, deltaTime in ms
			y += sinf(r) * v * deltaTime / 1000;
		} else { // interpolating
			c = 0xffffff;
			x = ox + dx * progress;
			y = oy + dy * progress;
			r = or + dr * progress;
			tr = otr + dtr * progress;

		}
		//}
		
		

		//x = getFromBuffer <float>(stateBuffers[0], XOFFSET);
		//y = getFromBuffer <float>(stateBuffers[0], YOFFSET);
		//r = getFromBuffer <float>(stateBuffers[0], ROFFSET);
		//v = getFromBuffer <float>(stateBuffers[0], VOFFSET);
		//tr = getFromBuffer <float>(stateBuffers[0], TROFFSET);
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

		screen->Plot(
			(int)x,
			(int)y,
			c
		);

		//screen->Plot((int)getFromBuffer <float>(nb, XOFFSET), (int)getFromBuffer <float>(nb, YOFFSET), 0x00ff00);
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

	void Player::move(float deltaTime) {
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

		// collision
		if (x < 0) {
			x = 0;
		} else if (x > 800) {
			x = 800;
		}
		if (y < 0) {
			y = 0;
		} else if (y > 500) {
			y = 500;
		}
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

	Player::Player(char* buffer): Tank(buffer, buffer) { // nts make enemy class
		// wejoo gekke constructor structure
	}

	void Player::rotateTurret(int mx, int my) {
		if (aimWithMouse) tr = atan2f(my - y, mx - x);
	}

	void Player::toBuffer(char* buf) {
		insertIntoBuffer <float> (&x, buf, XOFFSET);
		insertIntoBuffer <float> (&y, buf, YOFFSET);
		insertIntoBuffer <float> (&r, buf, ROFFSET);
		insertIntoBuffer <float> (&v, buf, VOFFSET);
		insertIntoBuffer <float> (&tr, buf, TROFFSET);
		insertIntoBuffer <char> (&id, buf, IDOFFSET);
		insertIntoBuffer <char[9]> (&name, buf, NAMEOFFSET); // nts inelegant?
	}

						  
}