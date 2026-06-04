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
	Duration = 1.0f;
	bUseRelativeStart = true;

	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Rotate360_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTRotate360Memory* Memory = reinterpret_cast<FBTRotate360Memory*>(NodeMemory);
	Memory->Elapsed = 0.f;
	Memory->bInitialized = false;

	AAIController* AiController = OwnerComp.GetAIOwner();
	if (!AiController) return EBTNodeResult::Failed;

	APawn* Pawn = AiController->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	FRotator StartRot = Pawn->GetActorRotation();
	Memory->StartYaw = StartRot.Yaw;

	if (Duration <= 0.f)
	{
		FRotator NewRot = StartRot;
		NewRot.Yaw = Memory->StartYaw + 360.f;
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

	if (!Memory->bInitialized)
	{
		Memory->StartYaw = Pawn->GetActorRotation().Yaw;
		Memory->Elapsed = 0.f;
		Memory->bInitialized = true;
	}

	Memory->Elapsed += DeltaSeconds;
	float Alpha = FMath::Clamp(Memory->Elapsed / Duration, 0.f, 1.f);
	
	float YawOffset = 360.f * Alpha;
	float TargetYaw = Memory->StartYaw + YawOffset;

	FRotator NewRot = Pawn->GetActorRotation();
	NewRot.Yaw = TargetYaw;
	Pawn->SetActorRotation(NewRot);

	if (Alpha >= 1.f)
	{
		// ensure exact final yaw
		NewRot.Yaw = Memory->StartYaw + 360.f;
		Pawn->SetActorRotation(NewRot);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
