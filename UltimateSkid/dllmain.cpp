#include <Windows.h>
#include "Util.h"
#include "minhook/include/MinHook.h"
#include <ostream>
#include <iostream>
#include "Athena.h"

DWORD WINAPI MainThread(LPVOID)
{

	MH_Initialize();

	Util::InitSdk();
	Util::InitGlobals();
	Util::InitGameplaystatics();

	Athena::AthenaHook();

	return TRUE;
}

// basic dll injection shit lol
BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID res)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, MainThread, mod, 0, 0);
	}

	return TRUE;
}
