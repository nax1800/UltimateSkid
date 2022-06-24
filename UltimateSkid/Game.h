#pragma once 

#include <Windows.h>
#include "Util.h"
#include "Globals.h"
#include <ostream>
#include <iostream>
#include <map>
#include "FixedStructs.h"

using namespace Globals;

namespace Game
{
	//array0x mode was on full
	static void SetupInventory()
	{
		auto EditToolDef = SDK::UObject::FindObject<SDK::UFortEditToolItemDefinition>("FortEditToolItemDefinition EditTool.EditTool");
		auto WallBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall");
		auto FloorBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor");
		auto StairBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W");
		auto RoofBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS");
		auto pWood = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortResourceItemDefinition WoodItemData.WoodItemData");
		auto pMetal = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortResourceItemDefinition MetalItemData.MetalItemData");
		auto pStone = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortResourceItemDefinition StoneItemData.StoneItemData");
		auto pRockets = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortAmmoItemDefinition AmmoDataRockets.AmmoDataRockets");
		PickaxeDef = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01");
		Items.insert_or_assign(Util::GenerateGuidPtr(), pWood);
		Items.insert_or_assign(Util::GenerateGuidPtr(), pMetal);
		Items.insert_or_assign(Util::GenerateGuidPtr(), pStone);
		Items.insert_or_assign(Util::GenerateGuidPtr(), pRockets);

		auto FortController = reinterpret_cast<SDK::AFortPlayerController*>(Globals::GPlayerController);
		FortInventory = reinterpret_cast<WorldInventory*>(Globals::GPlayerController)->WorldInventory;

		auto PickaxeItem = PickaxeDef->CreateTemporaryItemInstanceBP(1, 0);
		auto WorldPickaxeItem = reinterpret_cast<SDK::UFortWorldItem*>(PickaxeItem);
		WorldPickaxeItem->ItemEntry.Count = 1;
		FortInventory->Inventory.ReplicatedEntries.Add(WorldPickaxeItem->ItemEntry);
		FortInventory->Inventory.ItemInstances.Add(WorldPickaxeItem);
		QuickBars->ServerAddItemInternal(WorldPickaxeItem->GetItemGuid(), SDK::EFortQuickBars::Primary, 0);

		auto WallBuildItem = WallBuildDef->CreateTemporaryItemInstanceBP(1, 0);
		auto WallWorldBuildItem = reinterpret_cast<SDK::UFortWorldItem*>(WallBuildItem);
		FortInventory->Inventory.ReplicatedEntries.Add(WallWorldBuildItem->ItemEntry);
		FortInventory->Inventory.ItemInstances.Add(WallWorldBuildItem);
		QuickBars->ServerAddItemInternal(WallWorldBuildItem->GetItemGuid(), SDK::EFortQuickBars::Secondary, 0);
		WallGuid = WallWorldBuildItem->GetItemGuid();

		auto FloorBuildItem = FloorBuildDef->CreateTemporaryItemInstanceBP(1, 0);
		auto FloorWorldBuildItem = reinterpret_cast<SDK::UFortWorldItem*>(FloorBuildItem);
		FortInventory->Inventory.ReplicatedEntries.Add(FloorWorldBuildItem->ItemEntry);
		FortInventory->Inventory.ItemInstances.Add(FloorWorldBuildItem);
		QuickBars->ServerAddItemInternal(FloorWorldBuildItem->GetItemGuid(), SDK::EFortQuickBars::Secondary, 1);
		FloorGuid = FloorWorldBuildItem->GetItemGuid();

		auto StairBuildItem = StairBuildDef->CreateTemporaryItemInstanceBP(1, 0);
		auto StairWorldBuildItem = reinterpret_cast<SDK::UFortWorldItem*>(StairBuildItem);
		FortInventory->Inventory.ReplicatedEntries.Add(StairWorldBuildItem->ItemEntry);
		FortInventory->Inventory.ItemInstances.Add(StairWorldBuildItem);
		QuickBars->ServerAddItemInternal(StairWorldBuildItem->GetItemGuid(), SDK::EFortQuickBars::Secondary, 2);
		StairGuid = StairBuildItem->GetItemGuid();

		auto RoofBuildItem = RoofBuildDef->CreateTemporaryItemInstanceBP(1, 0);
		auto RoofWorldBuildItem = reinterpret_cast<SDK::UFortWorldItem*>(RoofBuildItem);
		FortInventory->Inventory.ReplicatedEntries.Add(RoofWorldBuildItem->ItemEntry);
		FortInventory->Inventory.ItemInstances.Add(RoofWorldBuildItem);
		QuickBars->ServerAddItemInternal(RoofWorldBuildItem->GetItemGuid(), SDK::EFortQuickBars::Secondary, 3);
		RoofGuid = RoofWorldBuildItem->GetItemGuid();

		FortInventory->HandleInventoryLocalUpdate();
		static_cast<SDK::AFortPlayerController*>(Globals::GPlayerController)->HandleWorldInventoryLocalUpdate();
	}

	static void SetupQuickBars()
	{
		auto FortController = reinterpret_cast<SDK::AFortPlayerController*>(Globals::GPlayerController);
		QuickBars = reinterpret_cast<SDK::AFortQuickBars*>(Util::SpawnActor(SDK::AFortQuickBars::StaticClass(), SDK::FVector{ 0,0,3029 }, SDK::FRotator()));
		reinterpret_cast<AFortAsQuickBars*>(Globals::GPlayerController)->QuickBars = QuickBars;
		QuickBars->SetOwner(Globals::GPlayerController);

		Globals::GPlayerController->OnRep_QuickBar();
		QuickBars->OnRep_PrimaryQuickBar();
		QuickBars->OnRep_SecondaryQuickBar();
	}

	static void UpdateInventory()
	{
		FortInventory->HandleInventoryLocalUpdate();
		static_cast<SDK::AFortPlayerController*>(Globals::GPlayerController)->HandleWorldInventoryLocalUpdate();
		static_cast<SDK::AFortPlayerController*>(Globals::GPlayerController)->OnRep_QuickBar();
		QuickBars->OnRep_PrimaryQuickBar();
		QuickBars->OnRep_SecondaryQuickBar();
	}

	static void ExecuteInventoryItem(SDK::FGuid* Guid)
	{
		auto ItemInstances = FortInventory->Inventory.ItemInstances;

		for (int i = 0; i < ItemInstances.Num(); i++)
		{
			auto ItemInstance = ItemInstances.operator[](i);

			if (Util::AreGuidsTheSame(ItemInstance->GetItemGuid(), (*Guid)))
			{
				Globals::GPawn->EquipWeaponDefinition((SDK::UFortWeaponItemDefinition*)ItemInstance->GetItemDefinitionBP(), (*Guid));
			}
		}
	}

	static void DropPickupAtLocation(SDK::UFortItemDefinition* ItemDef, int Count)
	{
		auto FortPickup = reinterpret_cast<SDK::AFortPickupBackpack*>(Util::SpawnActor(SDK::AFortPickupBackpack::StaticClass(), Globals::GPawn->K2_GetActorLocation(), SDK::FRotator()));
		FortPickup->PrimaryPickupItemEntry.ItemDefinition = ItemDef;
		FortPickup->PrimaryPickupItemEntry.Count = Count;
		FortPickup->OnRep_PrimaryPickupItemEntry();
		FortPickup->TossPickup(Globals::GPawn->K2_GetActorLocation(), Globals::GPawn, 999, true);
	}
}