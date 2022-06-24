#pragma once

#include <consoleapi.h>
#include <cstdio>
#include "SDK.hpp"
#include <memoryapi.h>
#include <inttypes.h>
#include <Windows.h>
#include <psapi.h>
#include <stdio.h>
#include <winscard.h>
#include "Globals.h"

class Util
{
public:
	static void InitGameplaystatics()
	{
		Globals::GameplayStatics = reinterpret_cast<SDK::UGameplayStatics*>(SDK::UGameplayStatics::StaticClass());
	}

	static SDK::AActor* SpawnActor(SDK::UClass* ActorClass, SDK::FVector Location, SDK::FRotator Rotation)
	{
		SDK::FQuat Quat;
		SDK::FTransform Transform;
		Quat.W = 0;
		Quat.X = Rotation.Pitch;
		Quat.Y = Rotation.Roll;
		Quat.Z = Rotation.Yaw;

		Transform.Rotation = Quat;
		Transform.Scale3D = SDK::FVector{ 1,1,1 };
		Transform.Translation = Location;

		auto Actor = Globals::GameplayStatics->STATIC_BeginSpawningActorFromClass((*Globals::GWorld), ActorClass, Transform, false, nullptr);
		Globals::GameplayStatics->STATIC_FinishSpawningActor(Actor, Transform);
		return Actor;
	}

	static bool AreGuidsTheSame(SDK::FGuid guidA, SDK::FGuid guidB)
	{
		if (guidA.A == guidB.A && guidA.B == guidB.B && guidA.C == guidB.C && guidA.D == guidB.D)
			return true;
		else
			return false;
	}

	static VOID InitConsole()
	{
		AllocConsole();

		FILE* pFile;
		freopen_s(&pFile, "CONOUT$", "w", stdout);
	}

	static VOID InitSdk()
	{
		auto pUWorldAddress = Util::FindPattern("\x48\x8B\x1D\x00\x00\x00\x00\x00\x00\x00\x10\x4C\x8D\x4D\x00\x4C", "xxx???????xxxx?x");
		auto pUWorldOffset = *reinterpret_cast<uint32_t*>(pUWorldAddress + 3);
		Globals::GWorld = reinterpret_cast<SDK::UWorld**>(pUWorldAddress + 7 + pUWorldOffset);

		auto pGObjectAddress = Util::FindPattern("\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xD6", "xxx????x????x????x????xxx");
		auto pGObjectOffset = *reinterpret_cast<uint32_t*>(pGObjectAddress + 3);
		SDK::UObject::GObjects = reinterpret_cast<SDK::FUObjectArray*>(pGObjectAddress + 7 + pGObjectOffset);

		auto pGNameAddress = Util::FindPattern("\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x75\x50\xB9\x00\x00\x00\x00\x48\x89\x5C\x24", "xxx????xxxxxx????xxxx");
		auto pGNameOffset = *reinterpret_cast<uint32_t*>(pGNameAddress + 3);

		SDK::FName::GNames = *reinterpret_cast<SDK::TNameEntryArray**>(pGNameAddress + 7 + pGNameOffset);

	}

	static VOID InitGlobals()
	{
		uintptr_t pBaseAddress = Util::BaseAddress();
		if (!pBaseAddress)
		{
			printf("Base Address failed!\n");
			exit(0);
		}

		Globals::GLocalPlayer = (*Globals::GWorld)->OwningGameInstance->LocalPlayers[0];
		Globals::GActors = &(*Globals::GWorld)->PersistentLevel->Actors;
		Globals::GPlayerController = static_cast<SDK::AFortPlayerControllerAthena*>(Globals::GLocalPlayer->PlayerController);
		Globals::GPlayerState = static_cast<SDK::AFortPlayerStateAthena*>(Globals::GPlayerController->PlayerState);
	}

	static VOID InitPatches()
	{
		auto pAbilityPatchAddress = Util::FindPattern
		(
			"\xC0\x0F\x84\x3C\x02\x00\x00\x0F\x2F\xF7\x0F\x86\xF5\x00\x00\x00",
			"xxxxxxxxxxxxxxxx"
		);
		if (pAbilityPatchAddress)
		{
			DWORD dwProtection;
			VirtualProtect(pAbilityPatchAddress, 16, PAGE_EXECUTE_READWRITE, &dwProtection);

			reinterpret_cast<uint8_t*>(pAbilityPatchAddress)[2] = 0x85;
			reinterpret_cast<uint8_t*>(pAbilityPatchAddress)[11] = 0x8D;

			DWORD dwTemp;
			VirtualProtect(pAbilityPatchAddress, 16, dwProtection, &dwTemp);
		}
	}

	static uintptr_t BaseAddress()
	{
		return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
	}

	static PBYTE FindPattern(LPCSTR lpPattern, LPCSTR lpMask)
	{
		MODULEINFO info = { 0 };

		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

		return Util::FindPattern(info.lpBaseOfDll, info.SizeOfImage, lpPattern, lpMask);
	}

	static SDK::AActor* FindActor(SDK::UClass* pClass, int iSkip = 0) {
		for (int i = 0, j = 0; i < Globals::GActors->Num(); i++) {
			SDK::AActor* pActor = Globals::GActors->operator[](i);

			if (pActor != nullptr) {
				if (pActor->IsA(pClass)) {
					if (j >= iSkip)
						return pActor;
					else {
						j++;
						continue;
					}
				}
			}
		}

		return nullptr;
	}

	// Added for inv
	static SDK::FGuid* GenerateGuidPtr() {
		SDK::FGuid* Guid = new SDK::FGuid();
		Guid->A = rand() % 1000;
		Guid->B = rand() % 1000;
		Guid->C = rand() % 1000;
		Guid->D = rand() % 1000;
		return Guid;
	}

	static SDK::FGuid GenerateGuid() {
		SDK::FGuid Guid;
		Guid.A = rand() % 1000;
		Guid.B = rand() % 1000;
		Guid.C = rand() % 1000;
		Guid.D = rand() % 1000;
		return Guid;
	}
private:

	static BOOL MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask)
	{
		for (auto value = static_cast<PBYTE>(pBuffer); *lpMask; ++lpPattern, ++lpMask, ++value)
		{
			if (*lpMask == 'x' && *reinterpret_cast<LPCBYTE>(lpPattern) != *value)
				return false;
		}

		return true;
	}

	static PBYTE FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask)
	{
		dwSize -= static_cast<DWORD>(strlen(lpMask));

		for (auto i = 0UL; i < dwSize; ++i)
		{
			auto pAddress = static_cast<PBYTE>(pBase) + i;

			if (MaskCompare(pAddress, lpPattern, lpMask))
				return pAddress;
		}

		return NULL;
	}
};