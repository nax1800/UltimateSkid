#pragma once

#include "SDK.hpp"


struct AFortAsQuickBars
{
public:
	unsigned char                                      UnknownData00[0x1A88];
	class SDK::AFortQuickBars* QuickBars;
};
struct WorldInventory
{
	unsigned char UnknownData00[0x1D28];
	class SDK::AFortInventory* WorldInventory;
};
struct ParentInventory
{
	unsigned char UnknownData00[0x0098];
	class SDK::AFortInventory               ParentInventory;
};
struct OwnerInventory
{
	unsigned char UnknownData00[0x01D8];
	class SDK::AFortInventory* OwnerInventory;
};
struct AFortAsBuildPreview
{
public:
	unsigned char UnknownData00[0x1788];
	class SDK::ABuildingPlayerPrimitivePreview* BuildPreviewMarker;
};
struct AFortAsCurrentBuildable
{
public:
	unsigned char UnknownData00[0x1940];
	class SDK::UClass* CurrentBuildableClass;
};
struct AFortAsLastBuildable
{
public:
	unsigned char UnknownData00[0x1948];
	class SDK::UClass* PreviousBuildableClass;
};
struct AFortAsEditActor
{
public:
	unsigned char UnknownData00[0x1A48];
	class SDK::ABuildingSMActor* EditBuildingActor;
};
struct AFortAsBuildPreviewMID
{
public:
	unsigned char UnknownData00[0x1928];
	class SDK::UMaterialInstanceDynamic* BuildPreviewMarkerMID;
};