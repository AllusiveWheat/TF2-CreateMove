#pragma once

#include <MinHook.h>
#include "C_UserCmd.h"

namespace hooks {
	using CreateMoveFn = bool(__stdcall*)(float sampleTime, C_UserCmd* cmd);

}
//hooks::CreateMoveFn originalCreateMove = nullptr;
