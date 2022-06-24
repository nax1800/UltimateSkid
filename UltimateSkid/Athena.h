#pragma once 

#include <Windows.h>
#include "Util.h"
#include "minhook/include/MinHook.h"
#include "Globals.h"
#include <ostream>
#include <iostream>
#include "Game.h"
#include <vector>

namespace Athena
{
	bool bInGame = false;
	bool bLSDropped = false;
	bool bReady = false;

	PVOID(*ProcessEvent)(SDK::UObject*, SDK::UFunction*, PVOID) = nullptr;

	PVOID ProcessEventHook(SDK::UObject* Obj, SDK::UFunction* Func, PVOID Params)
	{
		auto FuncName = Func->GetName();

		if (FuncName.find("BP_PlayButton") != std::string::npos)
		{
			Globals::GPlayerController->SwitchLevel(L"Athena_Terrain");
			bReady = true;
		}

		if (FuncName.find("ReadyToStartMatch") != std::string::npos && bReady)
		{
			if (bReady)
			{
				Util::InitGlobals();

				Globals::GameMode = static_cast<SDK::AFortGameModeAthena*>((*Globals::GWorld)->AuthorityGameMode);
				Globals::GameState = static_cast<SDK::AFortGameStateAthena*>((*Globals::GWorld)->GameState);

				Globals::GPlayerController->CheatManager->Summon(L"PlayerPawn_Athena_C");
				Globals::GPawn = static_cast<SDK::APlayerPawn_Athena_C*>(Util::FindActor(SDK::APlayerPawn_Athena_C::StaticClass()));
				Globals::GPlayerController->Possess(Globals::GPawn);
				Globals::GPlayerController->CheatManager->God();

				Globals::GPawn->ServerChoosePart(SDK::EFortCustomPartType::Head, SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1"));
				Globals::GPawn->ServerChoosePart(SDK::EFortCustomPartType::Body, SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01"));
				Globals::GPlayerState->OnRep_CharacterParts();

				bInGame = true;
				bReady = false;

				Globals::GPlayerController->ServerReadyToStartMatch();
				Globals::GameMode->StartMatch();

			}

		}

		if (FuncName.find("Tick") != std::string::npos && bInGame)
		{
			if (GetAsyncKeyState(VK_SPACE) && 0x01) {
				if (Globals::GPawn->CanJump() && !Globals::GPawn->IsJumpProvidingForce()) {
					Globals::GPawn->Jump();
				}
			}

			Globals::GPawn->CurrentMovementStyle = SDK::EFortMovementStyle::Sprinting;
		}

		if (FuncName.find("ServerAttemptAircraftJump") != std::string::npos && bInGame)
		{
			auto Bus = Util::FindActor(SDK::AAthenaAircraft_C::StaticClass());
			auto Pawn = Util::SpawnActor(SDK::APlayerPawn_Athena_C::StaticClass(), Bus->K2_GetActorLocation(), {});
			Globals::GPawn = static_cast<SDK::APlayerPawn_Athena_C*>(Pawn);
			Globals::GPlayerController->Possess(Globals::GPawn);
			Globals::GPlayerController->CheatManager->God();

			Globals::GPawn->ServerChoosePart(SDK::EFortCustomPartType::Head, SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1"));
			Globals::GPawn->ServerChoosePart(SDK::EFortCustomPartType::Body, SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01"));
			Globals::GPlayerState->OnRep_CharacterParts();
		}

		if (FuncName.find("ServerLoadingScreenDropped") != std::string::npos && bInGame)
		{
			Game::SetupQuickBars();
			Game::SetupInventory();
			Game::UpdateInventory();
			bLSDropped = true;
		}

		if (FuncName.find("ServerHandlePickup") != std::string::npos && bInGame)
		{
			//skid
		}

		if (FuncName.find("ServerExecuteInventoryItem") != std::string::npos && bInGame)
		{
			Game::ExecuteInventoryItem(reinterpret_cast<SDK::FGuid*>(Params));
		}

		return ProcessEvent(Obj, Func, Params);
	}

	static void AthenaHook()
	{
		auto ProcessEventAddress = Util::FindPattern("\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x48\x63\x41\x0C", "xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxx");

		MH_CreateHook((LPVOID)(ProcessEventAddress), ProcessEventHook, (LPVOID*)(&ProcessEvent));
		MH_EnableHook((LPVOID)(ProcessEventAddress));
	}
}