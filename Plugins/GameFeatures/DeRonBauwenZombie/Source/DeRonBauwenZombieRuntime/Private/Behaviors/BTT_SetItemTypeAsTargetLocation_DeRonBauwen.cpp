#include "Behaviors/BTT_SetItemTypeAsTargetLocation_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeRonBauwenZombieRuntime/StudentPerceptor_DeRonBauwen.h"

#include "Items/BaseItem.h"

EBTNodeResult::Type UBTT_SetItemTypeAsTargetLocation_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard{OwnerComp.GetBlackboardComponent()};
	if (!Blackboard) return EBTNodeResult::Failed;

	AAIController const *AiController{OwnerComp.GetAIOwner()};
	if (!AiController) return EBTNodeResult::Failed;
	
	ASurvivorPawn const *Survivor{Cast<ASurvivorPawn>(AiController->GetPawn())};
	if (!Survivor) return EBTNodeResult::Failed;
	
	UStudentPerceptor_DeRonBauwen* Perceptor{Survivor->GetComponentByClass<UStudentPerceptor_DeRonBauwen>()};
	if (!Perceptor) return EBTNodeResult::Failed;
	
	ABaseItem* ClosestItem{Perceptor->GetClosestRememberedItemOfType(ItemType)};
	if (!ClosestItem || !IsValid(ClosestItem))
	{
		Blackboard->ClearValue(ResultItemKey.SelectedKeyName);
		Blackboard->ClearValue(ResultItemLocationKey.SelectedKeyName);
		return EBTNodeResult::Failed;
	}
	
	Blackboard->SetValueAsObject(ResultItemKey.SelectedKeyName, ClosestItem);
	Blackboard->SetValueAsVector(ResultItemLocationKey.SelectedKeyName, ClosestItem->GetActorLocation());
	
	return EBTNodeResult::Succeeded;
}
