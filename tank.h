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
		void move(float dt);

		int w, h;
		float x, y, r, v;
		//Pixel* c;

	private:
		

	};
}