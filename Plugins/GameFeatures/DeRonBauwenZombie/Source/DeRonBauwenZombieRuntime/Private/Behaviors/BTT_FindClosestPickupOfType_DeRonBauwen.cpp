#include "Behaviors/BTT_FindClosestPickupOfType_DeRonBauwen.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "DeRonBauwenZombieRuntime/StudentPerceptor_DeRonBauwen.h"

#include "Items/BaseItem.h"

EBTNodeResult::Type UBTT_FindClosestPickupOfType_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	const APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	
	ABaseItem *ClosestItem{};
	float ClosestDistance{MaxSearchDistance * MaxSearchDistance};
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* Actor = *ActorItr;
		if (!Actor) continue;
		
		ABaseItem *Item = Cast<ABaseItem>(Actor);
		if (!Item) continue;
		
		AAIController const *AIController = OwnerComp.GetAIOwner();
		if (!AIController) return EBTNodeResult::Failed;

		ASurvivorPawn const *Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
		if (!Survivor) return EBTNodeResult::Failed;
	
		auto const Inventory{Survivor->GetComponentByClass<UInventoryComponent>()};
		if (!Inventory) return EBTNodeResult::Failed;
		
		if (Item->GetItemType() != TargetItemType) continue;
		if (Item->IsHidden()) continue;
		if (Inventory->GetInventory().Contains(Item)) continue;
		
		float const Distance = FVector::DistSquared(OwnerPawn->GetActorLocation(), Item->GetActorLocation());
                
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestItem = Item;
		}
	}
	
	if (!ClosestItem)
	{
		return EBTNodeResult::Failed;
	}
	
	BlackboardComp->SetValueAsVector(ResultItemKey.SelectedKeyName, ClosestItem->GetActorLocation());
	BlackboardComp->SetValueAsObject(ClosestItemPtrKey.SelectedKeyName, ClosestItem);

	return EBTNodeResult::Succeeded;
}
