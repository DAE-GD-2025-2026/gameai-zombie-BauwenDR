#include "Behaviors/BTT_SetItemTypeAsTargetLocation_DeRonBauwen.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "DeRonBauwenZombieRuntime/StudentPerceptor_DeRonBauwen.h"

#include "Items/BaseItem.h"

EBTNodeResult::Type UBTT_SetItemTypeAsTargetLocation_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	
	const ABaseItem* ClosestItem{Cast<ABaseItem>(BlackboardComp->GetValueAsObject(ClosestItemPtrKey.SelectedKeyName))};
	if (!ClosestItem)
	{
		return EBTNodeResult::Failed;
	}
	
	BlackboardComp->SetValueAsVector(ResultItemKey.SelectedKeyName, ClosestItem->GetActorLocation());
	return EBTNodeResult::Succeeded;
}
