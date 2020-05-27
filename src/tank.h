#pragma once

namespace Tmpl8 
{

class Surface;

class Tank {
	public:
		Tank();
		~Tank();

		void draw(Surface* screen);
		void getVerticis(float* arr);
		void move(float dt);
		void rotateTurret(float nr);
		void fromBuffer(char* buf);

		int w, h;
		float x, y, r, tr, v;
};

class Player : public Tank {
	public:
		void rotateTurret(int mx, int my);
		void toBuffer(char* buf);
		
		bool aimWithMouse = true;

	//private:

};

}