#include "Behaviors/BTT_UseItem_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Survivor/SurvivorPawn.h"

EBTNodeResult::Type UBTT_UseItem_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Blackboard{OwnerComp.GetBlackboardComponent()};
	if (!Blackboard) return EBTNodeResult::Failed;
	
	auto const ItemIndex{Blackboard->GetValueAsInt(ItemToUseKey.SelectedKeyName)};
	
	AAIController const *AIController{OwnerComp.GetAIOwner()};
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn const *Survivor{Cast<ASurvivorPawn>(AIController->GetPawn())};
	if (!Survivor) return EBTNodeResult::Failed;
	
	auto const Inventory{Survivor->GetComponentByClass<UInventoryComponent>()};
	if (!Inventory) return EBTNodeResult::Failed;
	
	if (ItemIndex < 0 || ItemIndex >= Inventory->GetInventoryCapacity()) return EBTNodeResult::Failed;
	if (Inventory->GetInventory()[ItemIndex] == nullptr) return EBTNodeResult::Failed;

	if (Inventory->GetInventory()[ItemIndex]->GetValue() <= 0)
	{
		Inventory->RemoveItem(ItemIndex);
	}

	Inventory->UseItem(ItemIndex);

	return EBTNodeResult::Succeeded;
}
