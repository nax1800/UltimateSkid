#pragma once

#include <wtypes.h>
#include "Globals.hpp"
#include "Util.h"
#include <stdio.h>
#include "SDK.hpp"
#include <cstdint>
#include "Offsets.h"

namespace Init
{
    static void InitGameplaystatics()
    {
        Globals::GameplayStatics = reinterpret_cast<SDK::UGameplayStatics*>(SDK::UGameplayStatics::StaticClass());
    }

    static VOID InitSdk()
    {
        auto pUWorldAddress = Util::FindPattern("\x48\x8B\x1D\x00\x00\x00\x00\x00\x00\x00\x10\x4C\x8D\x4D\x00\x4C", "xxx???????xxxx?x");
        auto pUWorldOffset = *reinterpret_cast<uint32_t*>(pUWorldAddress + 3);
            Globals::World = reinterpret_cast<SDK::UWorld**>(pUWorldAddress + 7 + pUWorldOffset);

        auto pGObjectAddress = Util::FindPattern("\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xD6", "xxx????x????x????x????xxx");
        auto pGObjectOffset = *reinterpret_cast<uint32_t*>(pGObjectAddress + 3);
        SDK::UObject::GObjects = reinterpret_cast<SDK::FUObjectArray*>(pGObjectAddress + 7 + pGObjectOffset);

        auto pGNameAddress = Util::FindPattern("\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x75\x50\xB9\x00\x00\x00\x00\x48\x89\x5C\x24", "xxx????xxxxxx????xxxx");
        auto pGNameOffset = *reinterpret_cast<uint32_t*>(pGNameAddress + 3);
        SDK::FName::GNames = *reinterpret_cast<SDK::TNameEntryArray**>(pGNameAddress + 7 + pGNameOffset);

        auto pStaticConstructObject_InternalOffset = Util::FindPattern("\xE8\x00\x00\x00\x00\x89\x78\x38", "x????xxx");
        if (!pStaticConstructObject_InternalOffset)
            printf("Finding pattern for StaticConstructObject_Internal has failed. Please relaunch Fortnite and try again!");
        auto pStaticConstructObject_InternalAddress = pStaticConstructObject_InternalOffset + 5 + *reinterpret_cast<int32_t*>(pStaticConstructObject_InternalOffset + 1);

        Globals::StaticConstructObject_Internal = reinterpret_cast<decltype(Globals::StaticConstructObject_Internal)>(pStaticConstructObject_InternalAddress);
        StaticLoadObject = reinterpret_cast<decltype(StaticLoadObject)>(Util::BaseAddress() + 0x142E560);

    }

    static VOID InitSdkk()
    {
        uintptr_t ModuleBaseAddr = (uintptr_t)GetModuleHandle(NULL);
        Globals::Engine = *reinterpret_cast<SDK::UFortEngine**>(Util::Offset<uintptr_t**>(Offsets::GEngineOffset));
        SDK::FName::GNames = *reinterpret_cast<SDK::TNameEntryArray**>(Util::Offset<uintptr_t**>(Offsets::GNamesOffset));
        SDK::UObject::GObjects = reinterpret_cast<SDK::FUObjectArray*>(Util::Offset<uintptr_t*>(Offsets::GUObjectArrayOffset));

    }

    static VOID InitCores()
    {
       // uintptr_t pBaseAddress = Util::BaseAddress();
       // if (!pBaseAddress)
       // {
        uintptr_t pBaseAddress = (uintptr_t)GetModuleHandle(NULL);
       // }

        if (!Globals::World)
        {

        }

        Globals::Level = (*Globals::World)->PersistentLevel;
        Globals::GameInstance = (*Globals::World)->OwningGameInstance;
        Globals::LocalPlayers = Globals::GameInstance->LocalPlayers;
        Globals::LocalPlayer = Globals::LocalPlayers[0];
        Globals::Actors = &Globals::Level->Actors;
        Globals::PlayerController = Globals::LocalPlayer->PlayerController;
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
}
