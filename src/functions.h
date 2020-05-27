#pragma once

namespace Tmpl8 
{

void displayBuffer(char* buf, int len); // just a function

template <typename type>
void insertIntoBuffer(type* t, char* buf, int offset) {
	memcpy(&buf[offset], t, sizeof(t));
}

template <typename type>
type getFromBuffer(char* buf, int offset) {
	return *(reinterpret_cast<type*>(&buf[offset]));
}

void time(int* i); int time();

}