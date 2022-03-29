#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <iostream>
#include "hooks.h"
#include "IClientMode.h"
#include <Detours.h>
#include "IClientEntityList.h"
#define P_CLIENTMODE 0xFB3780
#if _WIN64 
#pragma comment(lib, "detours.lib")
#endif
#include "IVEngineClient.h"
using namespace std;

//Globals
HINSTANCE DllHandle;
float localViewAngles[3];
Vector3 ang;
int buttons;
int tick;
// hooks
hooks::CreateMoveFn originalCreateMove = nullptr;
C_UserCmd* cms;


template <typename T, std::uint32_t index, typename ...Arguments>
inline auto call_virtual(void* const class_base, Arguments... args) noexcept
{
    using fn = T(__thiscall***)(void*, Arguments...);
    return ((*static_cast<fn>(class_base))[index])(class_base, args...);
}

template<typename T> T* GetInterface(HMODULE hModule, const char* interfaceName)
{
    hooks::CreateInterfaceFn CreateInterface = reinterpret_cast<hooks::CreateInterfaceFn>(GetProcAddress(hModule, "CreateInterface"));
    uintptr_t pInterface = CreateInterface(interfaceName, NULL);
    return reinterpret_cast<T*>(pInterface);
}
unsigned int createVirtual(void* _class, unsigned int index) {
    return static_cast<unsigned int>((*static_cast<int**>(_class))[index]);
}
bool init = false;
void __fastcall CreateMoveHk(float sampleTime, C_UserCmd* cmd) {
    if (!init){
        MessageBoxA(0, "Hello", "Life from create move", 0);
        init = true;
    }
   // cmd->viewangles.y = 85.f;
    tick = cmd->tick_count;
    originalCreateMove(sampleTime, cmd);

}


void shutdown(FILE* fp, std::string reason) {
    std::cout << reason << std::endl;
    Sleep(1000);
   // Detours::X64::VTableRemove(*(uintptr_t*)clientmode , (uintptr_t)CreateMoveHk, 19);
    if (fp != nullptr)
        fclose(fp);
    FreeConsole();
    FreeLibraryAndExitThread(DllHandle, 0);
}



DWORD WINAPI Menue(HINSTANCE hModule) 
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout); //sets cout to be used with our newly created console
    uintptr_t clientBaseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle(L"client.dll"));
    IClientMode* clientmode = *reinterpret_cast<IClientMode**>(clientBaseAddr + P_CLIENTMODE);

    IClientEntityList* g_EntityList = GetInterface<IClientEntityList>(GetModuleHandle(L"client.dll"), "VClientEntityList003");
    std::cout << "g_EntityList: " << std::hex << g_EntityList << std::endl;
    // \x48\x8B\x05\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x8B\x05\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x28\x48\x8B\x01
    // xxx????xxxxxxxxxxxx????xxxxxxxxxxxxxxxx
   
   //IVEngineClient* g_engineClient = GetInterface< IVEngineClient>(GetModuleHandle(L"engine.dll"), "VEngineClient013");
    uintptr_t* localPlayer = reinterpret_cast<uintptr_t*>(g_EntityList->GetClientEntity(1));
    player_info_t pInfo;
    
    
    std::cout << "clientmode: " << std::hex << clientmode << std::endl;
    std::cout << "LocalPlayer: " << std::hex << localPlayer << std::endl;
    
   // originalCreateMove= (hooks::CreateMoveFn) Detours::X64::DetourClassVTable(*(uintptr_t*)clientmode,CreateMoveHk, 19);
    
    std::cout << "originalCreateMove: " <<std::hex << originalCreateMove << std::endl;
    std::cout << "buttons: " << std::hex << buttons << std::endl;
    std::cout << "cmd" << std::hex << cms << std::endl;
    while (true)
    {
      //  std::cout << "buttons: " << std::hex << buttons << std::endl;
       /// std::cout << "buttons: " << buttons << std::endl;
       // std::cout << "ANG X: " << ang.x << std::endl;
       // std::cout << "ANG Y: " << ang.y << std::endl;
       // std::cout << "ANG Z: " << ang.z << std::endl;
       // std::cout << "Tick: " << tick << std::endl;

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

