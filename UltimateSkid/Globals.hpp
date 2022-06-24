#pragma once
namespace Globals 
{
    SDK::APlayerPawn_Generic_C* PlayerPawn;
    SDK::UGameplayStatics* GameplayStatics;
    SDK::UWorld** World;
    SDK::UFortEngine* Engine;
    SDK::ULevel* Level;
    SDK::TArray<SDK::AActor*>* Actors;
    SDK::UGameInstance* GameInstance;
    SDK::TArray<SDK::ULocalPlayer*> LocalPlayers;
    SDK::ULocalPlayer* LocalPlayer;
    SDK::APlayerController* PlayerController;
    SDK::UObject* (*StaticConstructObject_Internal)(SDK::UClass* InClass, SDK::UObject* InOuter, SDK::FName InName, int64_t InFlags, SDK::FUObjectItem::ObjectFlags InternalSetFlags, SDK::UObject* InTemplate, bool bCopyTransientsFromClassDefaults, void* InInstanceGraph, bool bAssumeTemplateIsArchetype);
}

