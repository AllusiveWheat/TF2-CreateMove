#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <iostream>
#include "hooks.h"
#include "IClientMode.h"
#include <Detours.h>
#define P_CLIENTMODE 0xFB3780
#if _WIN64 
#pragma comment(lib, "detours.lib")
#endif


//Globals
HINSTANCE DllHandle;
IClientMode* clientmode;
float localViewAngles[3];

// hooks
hooks::CreateMoveFn originalCreateMove = nullptr;

DWORD __stdcall EjectThread(LPVOID lpParameter) {
    Sleep(100);
    FreeLibraryAndExitThread(DllHandle, 0);
    return 0;
}
unsigned int createVirtual(void* _class, unsigned int index) {
    return static_cast<unsigned int>((*static_cast<int**>(_class))[index]);
}

void __fastcall CreateMoveHk(float sampleTime, C_UserCmd* cmd) {

    MessageBoxA(0, "Hello from create move!", "Hello", 0);



    originalCreateMove(sampleTime, cmd);
}


void shutdown(FILE* fp, std::string reason) {

    std::cout << reason << std::endl;
    Sleep(1000);
    Detours::X64::VTableRemove(*(uintptr_t*)clientmode , (uintptr_t)CreateMoveHk, 19);
    if (fp != nullptr)
        fclose(fp);
    FreeConsole();
    CreateThread(0, 0, EjectThread, 0, 0, 0);
    return;
}



DWORD WINAPI Menue(HINSTANCE hModule) {
  

    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout); //sets cout to be used with our newly created console

    uintptr_t clientBaseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle(L"client.dll"));
    clientmode = *reinterpret_cast<IClientMode**>(clientBaseAddr + P_CLIENTMODE);
    std::cout << std::hex << clientmode << std::endl;

    originalCreateMove= (hooks::CreateMoveFn) Detours::X64::DetourClassVTable(*(uintptr_t*)clientmode, CreateMoveHk, 19);
    std::cout << "originalCreateMove: " <<std::hex << originalCreateMove << std::endl;
  
    while (true)
    {

        if (GetAsyncKeyState(VK_F10) & 1) {
            shutdown(fp, "Byby");
            return 0;
        }
    }
    

}


BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DllHandle = hModule;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Menue, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

