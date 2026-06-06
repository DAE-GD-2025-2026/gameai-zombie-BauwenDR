#include "Behaviors/BTT_WanderAvoidZombies_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeRonBauwenZombieRuntime/StudentPerceptor_DeRonBauwen.h"
#include "Survivor/SurvivorPawn.h"

UBTT_WanderAvoidZombies_DeRonBauwen::UBTT_WanderAvoidZombies_DeRonBauwen()
{
	bNotifyTick = true;

	WanderBehavior = std::make_unique<FBlendedSteering::FWeightedBehavior>(std::make_unique<FWander>(), 0.1f);
	Steering = std::make_unique<FBlendedSteering>(FBlendedSteering{{WanderBehavior.get()}});
}

EBTNodeResult::Type UBTT_WanderAvoidZombies_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTT_WanderAvoidZombies_DeRonBauwen::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp{OwnerComp.GetBlackboardComponent()};

	const float NewCountdown{BlackboardComp->GetValueAsFloat(TimerCountdownKey.SelectedKeyName) - DeltaSeconds};
	BlackboardComp->SetValueAsFloat(TimerCountdownKey.SelectedKeyName, NewCountdown);

	if (NewCountdown <= 0.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	const AAIController* AIController{OwnerComp.GetAIOwner()};
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	ASurvivorPawn* Pawn{Cast<ASurvivorPawn>(AIController->GetPawn())};
	if (!Pawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UStudentPerceptor_DeRonBauwen* Perceptor{Pawn->GetComponentByClass<UStudentPerceptor_DeRonBauwen>()};
	if (!Perceptor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	UPawnMovementComponent* MovementComponent{Pawn->GetMovementComponent()};
	auto const MovementSpeed{MovementComponent->GetMaxSpeed()};

	// Evade zombies
	for (int It{0}; It < Perceptor->GetZombies().Num(); ++It)
	{
		if (EvadeBehaviors.Num() <= It)
		{
			EvadeBehaviors.Add(std::make_unique<FBlendedSteering::FWeightedBehavior>(std::make_unique<FEvade>(EvadeDistance), EvadePriority));
			Steering->AddBehaviour(EvadeBehaviors.Last().get());
		}

		EvadeBehaviors[It]->Behavior->SetTarget(FTargetData
		{
			Perceptor->GetZombies()[It]->GetActorLocation(),
			0.0f,
			Perceptor->GetZombies()[It]->GetActorForwardVector(),
		});
	}

	for (int It{Perceptor->GetZombies().Num()}; It < EvadeBehaviors.Num(); ++It)
	{
		EvadeBehaviors[It]->Behavior->SetTarget(FTargetData{FVector{0.0f, 0.0f, -2.0f * EvadeDistance}});
	}
	
	const FSteeringOutput Out = Steering->CalculateSteering(DeltaSeconds, *Pawn);
	const FVector TargetVel{Out.LinearVelocity, 0.0f};
	Pawn->SetActorRotation(TargetVel.Rotation());
	Pawn->AddMovementInput(TargetVel * MovementSpeed * DeltaSeconds);
}
