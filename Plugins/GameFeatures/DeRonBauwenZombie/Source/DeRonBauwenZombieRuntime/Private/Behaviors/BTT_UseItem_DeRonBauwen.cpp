#include "Behaviors/BTT_UseItem_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Survivor/SurvivorPawn.h"

EBTNodeResult::Type UBTT_UseItem_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	
	auto const ItemIndex = BlackboardComp->GetValueAsInt(ItemToUseKey.SelectedKeyName);
	if (ItemIndex < 0) return EBTNodeResult::Failed;
	
	AAIController const *AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASurvivorPawn const *Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;
	
	auto const Inventory{Survivor->GetComponentByClass<UInventoryComponent>()};
	if (!Inventory) return EBTNodeResult::Failed;

	Inventory->UseItem(ItemIndex);
	Inventory->RemoveItem(ItemIndex);

	return EBTNodeResult::Succeeded;
}
