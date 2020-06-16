#pragma once

namespace Tmpl8 
{

void displayBuffer(char* buf, int len); // just a function
//void displayBuffers(Pixel* mem);

template <typename type>
void insertIntoBuffer(type* t, char* buf, int offset) {
	memcpy(&buf[offset], t, sizeof(t));
}

template <typename type>
type getFromBuffer(char* buf, int offset) {
	return *(reinterpret_cast<type*>(&buf[offset]));
}

void time( int* i);  int time();

}

//int cap(int x, int min, int max) {
//	if (x < min) {
//		return min;
//	} else if (x > max) {
//		return max;
//	} else {
//		return x;
//	}
//}
//
//float capf(float x, float min, float max) {
//	if (x < min) {
//		return min;
//	} else if (x > max) {
//		return max;
//	} else {
//		return x;
//	}
//}