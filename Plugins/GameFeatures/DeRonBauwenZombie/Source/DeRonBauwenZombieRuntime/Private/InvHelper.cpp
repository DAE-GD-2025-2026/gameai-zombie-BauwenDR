#include "InvHelper.h"

#include <unordered_map>

#include "Items/ItemType.h"

enum class EItemType : uint8;

namespace
{
	const std::unordered_map<EItemType, int> INV_SLOTS {
			{EItemType::Medkit, 1},
			{EItemType::Shotgun, 2},
			{EItemType::Pistol, 2},
	};
}

int InvHelper::GetIndexForType(UInventoryComponent const* Inventory, EItemType Type)
{
	if (Type != EItemType::Food)
	{
		return INV_SLOTS.at(Type);
	}
	
	for (int Index{3}; Index < Inventory->GetInventory().Num(); ++Index)
	{
		if (Inventory->GetInventory()[Index] == nullptr)
			return Index;
	}
	
	return -1;
}
