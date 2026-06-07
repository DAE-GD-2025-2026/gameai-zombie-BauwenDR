#include "Behaviors/TBD_OwnsItemOfType_DeRonBauwen.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "Survivor/SurvivorPawn.h"

bool UTBD_OwnsItemOfType_DeRonBauwen::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController const *AIController{OwnerComp.GetAIOwner()};
	if (!AIController) return false;

	ASurvivorPawn const *Survivor{Cast<ASurvivorPawn>(AIController->GetPawn())};
	if (!Survivor) return false;

	auto const *Inventory{Survivor->GetComponentByClass<UInventoryComponent>()};
	if (!Inventory) return false;
	
	return Inventory->GetInventory().ContainsByPredicate([this](ABaseItem const *Item)
	{
		return Item == nullptr ? false : Item->GetItemType() == ItemType;
	});
}
