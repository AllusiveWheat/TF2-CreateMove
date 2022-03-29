#pragma once
#include "Vector3.h"
#include <cstdint>
struct C_UserCmd
{
	int command_number; //0
	int tick_count; //4
	int command_time;//8
	Vector3 viewangles; //12
	char pad1[28];//24
	float			forwardmove;	 // members clamped [-1;1]//52			
	float			sidemove;
	float			upmove;
	int	  buttons;//64
	char    impulse; //68
	char pad2[0x68];
};