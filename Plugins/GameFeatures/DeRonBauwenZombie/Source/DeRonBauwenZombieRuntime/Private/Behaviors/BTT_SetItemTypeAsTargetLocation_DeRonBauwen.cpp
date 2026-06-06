#include "Behaviors/BTT_SetItemTypeAsTargetLocation_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeRonBauwenZombieRuntime/StudentPerceptor_DeRonBauwen.h"

#include "Items/BaseItem.h"

EBTNodeResult::Type UBTT_SetItemTypeAsTargetLocation_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	AAIController* AIController{OwnerComp.GetAIOwner()};
	ASurvivorPawn const *Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	UStudentPerceptor_DeRonBauwen* Perceptor{Survivor->GetComponentByClass<UStudentPerceptor_DeRonBauwen>()};
	
	ABaseItem* ClosestItem{Perceptor->GetClosestRememberedItemOfType(ItemType)};
	if (!ClosestItem || !IsValid(ClosestItem))
	{
		BlackboardComp->ClearValue(ResultItemKey.SelectedKeyName);
		BlackboardComp->ClearValue(ResultItemLocationKey.SelectedKeyName);
		return EBTNodeResult::Failed;
	}
	
	BlackboardComp->SetValueAsObject(ResultItemKey.SelectedKeyName, ClosestItem);
	BlackboardComp->SetValueAsVector(ResultItemLocationKey.SelectedKeyName, ClosestItem->GetActorLocation());
	return EBTNodeResult::Succeeded;
}
