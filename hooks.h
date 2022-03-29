#pragma once

#include <MinHook.h>
#include "C_UserCmd.h"

namespace hooks {
	using CreateMoveFn = void(__stdcall*)(float sampleTime, C_UserCmd* cmd);
	using CreateInterfaceFn = uintptr_t(__fastcall*)(const char*, uintptr_t);
	
}
//hooks::CreateMoveFn originalCreateMove = nullptr;
