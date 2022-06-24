#pragma once

#include "SDK.hpp"
#include <map>
#include <vector>

namespace Globals {
	SDK::APlayerPawn_Athena_C* GPawn;
	SDK::UGameplayStatics* GameplayStatics;
	SDK::UWorld** GWorld;
	SDK::TArray<SDK::AActor*>* GActors;
	SDK::ULocalPlayer* GLocalPlayer;
	SDK::AFortPlayerControllerAthena* GPlayerController;
	SDK::AFortPlayerStateAthena* GPlayerState;
	SDK::AFortGameModeAthena* GameMode;
	SDK::AFortGameStateAthena* GameState;

	SDK::AFortQuickBars* QuickBars;
	SDK::FGuid EditToolGuid;
	SDK::FGuid PickaxeGuid;
	SDK::FGuid WallGuid;
	SDK::FGuid FloorGuid;
	SDK::FGuid StairGuid;
	SDK::FGuid RoofGuid;
	SDK::UFortWeaponMeleeItemDefinition* PickaxeDef;
	std::map<SDK::FGuid*, SDK::UFortWeaponItemDefinition*> Items;
	std::map<SDK::FGuid*, SDK::UFortTrapItemDefinition*> Traps;
	SDK::AFortInventory* FortInventory;
}
