#include "InvHelper_DeRonBauwen.h"

#include <unordered_map>

#include "Items/ItemType.h"

enum class EItemType : uint8;

namespace
{
	const std::unordered_map<EItemType, int> INV_SLOTS {
			{EItemType::Medkit, 0},
			{EItemType::Shotgun, 1},
			{EItemType::Pistol, 1},
	};
}

int InvHelper_DeRonBauwen::GetIndexForType(UInventoryComponent const* Inventory, EItemType Type)
{
	if (Type != EItemType::Food)
	{
		return INV_SLOTS.at(Type);
	}
	
	for (int Index{2}; Index < Inventory->GetInventory().Num(); ++Index)
	{
		if (Inventory->GetInventory()[Index] == nullptr)
			return Index;
	}
	
	return -1;
}
