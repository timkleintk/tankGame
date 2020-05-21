//#include "surface.h" // Pixel

class Surface;

class Tank
{
public:
	Tank();
	~Tank();

	void draw(Surface* screen);

	int x, y, w, h;
	float r;
	//Pixel* c;
private:

};
