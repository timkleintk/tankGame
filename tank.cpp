#include <Math.h>
#include "tank.h"
#include "surface.h" // Pixel
#include "windows.h"
#include <stdio.h>

namespace Tmpl8
{


	float rotationSpeed = 0.01f;
	float acceleration = 0.01f;

	float barrelLength = 10;
	int tankLength = 15;
	int tankWidth = 10;




	float maxV = 3.0f;


	Tank::Tank() {
		x = 100;
		y = 100;
		w = tankLength;
		h = tankWidth;
		v = 0.0f;
		r = 0.0f;
	}

	Tank::~Tank() {
	}

	void Tank::draw(Surface* screen) {
		screen->Plot(x, y, 0xffffff);
		//screen->Plot(x + w / 2.0f, y + h / 2.0f, 0xff0000);
		
		screen->Plot(
			x + w / 2.0f * cosf(r) - h / 2.0f * sinf(r),
			y + w / 2.0f * sinf(r) + h / 2.0f * cosf(r),
			0xff0000);
		screen->Plot(
			x - w / 2.0f * cosf(r) - h / 2.0f * sinf(r),
			y - w / 2.0f * sinf(r) + h / 2.0f * cosf(r),
			0xff0000);
		screen->Plot(
			x + w / 2.0f * cosf(r) + h / 2.0f * sinf(r),
			y + w / 2.0f * sinf(r) - h / 2.0f * cosf(r),
			0xff0000);
		screen->Plot(
			x - w / 2.0f * cosf(r) + h / 2.0f * sinf(r),
			y - w / 2.0f * sinf(r) - h / 2.0f * cosf(r),
			0xff0000);
	}



	void Tank::move(float dt) {
		dt = 1.0f;
		if (GetAsyncKeyState(VK_LEFT)) r -= rotationSpeed;
		if (GetAsyncKeyState(VK_RIGHT)) r += rotationSpeed;
		if (GetAsyncKeyState(VK_DOWN)) v -= acceleration;
		if (GetAsyncKeyState(VK_UP)) v += acceleration;

		if (v < 0) v = 0;
		if (v > maxV) v = maxV;
		
		//printf("x: %f, v: %f, r: %f \n up: %f \n", x, v, r, GetAsyncKeyState(VK_UP));
		//printf("up: %i \n", (bool) GetAsyncKeyState(VK_UP));


		x += cosf(r) * v;
		y += sinf(r) * v;
												  
	}											  
												  
}