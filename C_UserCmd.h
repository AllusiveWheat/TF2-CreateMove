#pragma once
#include "Vector3.h"

class C_UserCmd
{
public:
	int command_number;
	int tick_count;
	float command_time;
	Vector3 worldViewAngles;
	Vector3 localViewAngles;
	Vector3 attackangles;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	char impulse;
};