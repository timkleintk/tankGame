#pragma once
namespace Tmpl8 
{

struct Controls {
	static const int up = 0x57; // w
	static const int down = 0x53; // s
	static const int left = 0x41; // a
	static const int right = 0x44; // d
	//static const int tLeft = 0x5A; // z
	static const int tLeft = 0xBC; // ,
	//static const int tRight = 0x58; // x
	static const int tRight = 0xBE; // .
	static const int fire = 0x20; // " "

	static const bool aimWithMouse = false;
};

}