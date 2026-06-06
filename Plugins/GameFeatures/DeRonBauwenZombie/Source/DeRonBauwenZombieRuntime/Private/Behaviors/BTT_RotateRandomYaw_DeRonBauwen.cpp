#include "Behaviors/BTT_RotateRandomYaw_DeRonBauwen.h"

#include "AIController.h"

EBTNodeResult::Type UBTT_RotateRandomYaw_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController const *AiController{OwnerComp.GetAIOwner()};
	if (!AiController) return EBTNodeResult::Failed;
	
	APawn* Pawn{AiController->GetPawn()};
	if (!Pawn) return EBTNodeResult::Failed;

	const float RandomYaw{FMath::FRandRange(MinYaw, MaxYaw)};
	FRotator NewRot{Pawn->GetActorRotation()};
	NewRot.Yaw += RandomYaw;

	Pawn->SetActorRotation(NewRot);

	return EBTNodeResult::Succeeded;
}
