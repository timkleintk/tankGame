#pragma once
//#include "surface.h" // Pixel
//#include "surface.h"


namespace Tmpl8 {

class Surface;

class Tank {
	public:
		Tank();
		~Tank();

		void draw(Surface* screen);
		void getVerticis(float* arr);
		void move(float dt);

		void rotateTurret(int mx, int my);
		void rotateTurret(float nr);

		int w, h;
		float x, y, r, tr, v;
		//Pixel* c;

	private:
		

	};
}