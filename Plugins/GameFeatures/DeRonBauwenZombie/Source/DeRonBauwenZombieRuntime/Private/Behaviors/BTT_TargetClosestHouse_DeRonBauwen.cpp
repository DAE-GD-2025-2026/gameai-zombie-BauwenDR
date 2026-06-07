#include "Behaviors/BTT_TargetClosestHouse_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeRonBauwenZombieRuntime/StudentPerceptor_DeRonBauwen.h"
#include "Village/House/House.h"

EBTNodeResult::Type UBTT_TargetClosestHouse_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard{OwnerComp.GetBlackboardComponent()};
	if (!Blackboard) return EBTNodeResult::Failed;

	AAIController const *AiController{OwnerComp.GetAIOwner()};
	if (!AiController) return EBTNodeResult::Failed;
	
	ASurvivorPawn const *Survivor{Cast<ASurvivorPawn>(AiController->GetPawn())};
	if (!Survivor) return EBTNodeResult::Failed;
	
	UStudentPerceptor_DeRonBauwen* Perceptor{Survivor->GetComponentByClass<UStudentPerceptor_DeRonBauwen>()};
	if (!Perceptor) return EBTNodeResult::Failed;
	
	AHouse const *ClosestHouse{Perceptor->GetClosestRememberedHouse()};
	if (!ClosestHouse) return EBTNodeResult::Failed;
	
	Blackboard->SetValueAsVector(ResultPositionKey.SelectedKeyName, ClosestHouse->GetActorLocation());
	return EBTNodeResult::Succeeded;
}
