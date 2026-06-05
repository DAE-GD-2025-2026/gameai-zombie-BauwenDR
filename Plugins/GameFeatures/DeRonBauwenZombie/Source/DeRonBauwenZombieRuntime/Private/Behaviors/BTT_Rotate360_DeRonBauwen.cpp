#include "Behaviors/BTT_Rotate360_DeRonBauwen.h"

#include "AIController.h"

struct FBTRotate360Memory
{
	float Elapsed;
	float StartYaw;
	bool bInitialized;
};

UBTT_Rotate360_DeRonBauwen::UBTT_Rotate360_DeRonBauwen()
{
	NodeName = TEXT("Rotate 360");

	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Rotate360_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTRotate360Memory* Memory = reinterpret_cast<FBTRotate360Memory*>(NodeMemory);
	Memory->Elapsed = 0.f;
	Memory->bInitialized = true;

	AAIController* AiController = OwnerComp.GetAIOwner();
	if (!AiController) return EBTNodeResult::Failed;

	APawn* Pawn = AiController->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	Memory->StartYaw = Pawn->GetActorRotation().Yaw;

	if (Duration <= 0.f)
	{
		FRotator NewRot = Pawn->GetActorRotation();
		NewRot.Yaw = FRotator::NormalizeAxis(Memory->StartYaw + 360.f);
		Pawn->SetActorRotation(NewRot);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTT_Rotate360_DeRonBauwen::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTRotate360Memory* Memory = reinterpret_cast<FBTRotate360Memory*>(NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	Memory->Elapsed += DeltaSeconds;
	float Alpha = FMath::Clamp(Memory->Elapsed / Duration, 0.f, 1.f);

	float YawOffset = 360.f * Alpha;
	float TargetYaw = Memory->StartYaw + YawOffset;
	TargetYaw = FRotator::NormalizeAxis(TargetYaw);

	FRotator NewRot = Pawn->GetActorRotation();
	NewRot.Yaw = TargetYaw;
	Pawn->SetActorRotation(NewRot);

	if (Alpha >= 1.f)
	{
		NewRot.Yaw = FRotator::NormalizeAxis(Memory->StartYaw + 360.f);
		Pawn->SetActorRotation(NewRot);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
