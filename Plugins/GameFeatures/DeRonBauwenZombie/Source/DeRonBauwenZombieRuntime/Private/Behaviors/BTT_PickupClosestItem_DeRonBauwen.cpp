#include "Behaviors/BTT_PickupClosestItem_DeRonBauwen.h"

#include "AIController.h"
#include "InvHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"

EBTNodeResult::Type UBTT_PickupClosestItem_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	
	auto const ItemFromMap = BlackboardComp->GetValueAsObject(ClosestItemPtrKey.SelectedKeyName);
	if (!ItemFromMap) return EBTNodeResult::Failed;
	
	auto const ItemToCollect{Cast<ABaseItem>(ItemFromMap)};
	if (!ItemToCollect) return EBTNodeResult::Failed;
	
	AAIController const *AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn const *Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	auto const Inventory{Survivor->GetComponentByClass<UInventoryComponent>()};
	if (!Inventory) return EBTNodeResult::Failed;
	
	auto const PickupSlot{InvHelper::GetIndexForType(Inventory, ItemToCollect->GetItemType())};
	if (PickupSlot < 0) return EBTNodeResult::Failed;
	
	if (!Inventory->GrabItem(PickupSlot, ItemToCollect)) return EBTNodeResult::Failed;
	BlackboardComp->ClearValue(ClosestItemPtrKey.SelectedKeyName);
	
	return EBTNodeResult::Succeeded;
}
