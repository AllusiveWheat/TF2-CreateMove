#pragma once
#include "hooks.cpp"
 
typedef struct player_info_s
{
	char            szName[128];        //0x0010 - Player Name
	int             userId;             //0x0090 - Unique Server Identifier
} player_info_t;



class IVEngineClient
{
public:
	int GetLocalPlayer()
	{
		return vFunc<int(__fastcall*)(void*)>(this, 41)(this);
	}
	/*
	bool GetPlayerInfo(int ent_num, player_info_s *pinfo)
	{
		typedef bool(__thiscall* GetPlayerInfoFn)(int, player_info_s*);
		GetPlayerInfoFn pGetPlayerInfo = (GetPlayerInfoFn)((*(uintptr_t*)this)[8]);
		return pGetPlayerInfo(ent_num, pinfo);
	}
	*/
	};
