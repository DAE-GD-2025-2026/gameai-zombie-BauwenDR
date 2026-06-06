#include "Behaviors/BTT_SteerTowardAvoidingZombies_DeRonBauwen.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeRonBauwenZombieRuntime/StudentPerceptor_DeRonBauwen.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Survivor/SurvivorPawn.h"

UBTT_SteerTowardAvoidingZombies_DeRonBauwen::UBTT_SteerTowardAvoidingZombies_DeRonBauwen()
{
	bNotifyTick = true;

	SeekBehavior = std::make_unique<FBlendedSteering::FWeightedBehavior>(std::make_unique<FSeek>(), 0.2f);
	WanderBehavior = std::make_unique<FBlendedSteering::FWeightedBehavior>(std::make_unique<FWander>(), 0.1f);
	Steering = std::make_unique<FBlendedSteering>(FBlendedSteering{{SeekBehavior.get(), WanderBehavior.get()}});
}

EBTNodeResult::Type UBTT_SteerTowardAvoidingZombies_DeRonBauwen::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp{OwnerComp.GetBlackboardComponent()};

	AAIController const*AiController{OwnerComp.GetAIOwner()};
	if (!AiController) return EBTNodeResult::Failed;

	ASurvivorPawn* Pawn{Cast<ASurvivorPawn>(AiController->GetPawn())};
	if (!Pawn) return EBTNodeResult::Failed;
	Pawn->StartRunning();
	
	if (!Steering) return EBTNodeResult::Failed;

	const auto Destination{BlackboardComp->GetValueAsVector(DestinationKey.SelectedKeyName)};
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys && Pawn)
	{
		auto Result{NavSys->FindPathToLocationSynchronously(GetWorld(), AiController->GetNavAgentLocation(), Destination, Pawn)};
		if (Result->IsValid() && Result->GetPath()->GetPathPoints().Num() > 0)
		{
			CurrentPath = Result->GetPath();
			CurrentPathIndex = 0;
			FVector FirstPoint = CurrentPath->GetPathPoints()[0].Location;
			SeekBehavior->Behavior->SetTarget(FTargetData{FirstPoint});
			LastLocation = Pawn->GetActorLocation();
			return EBTNodeResult::InProgress;
		}
	}
	
	return EBTNodeResult::Failed;
}

void UBTT_SteerTowardAvoidingZombies_DeRonBauwen::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const AAIController* AIController{OwnerComp.GetAIOwner()};
	if (!AIController)
	{
		TaskFinished(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	ASurvivorPawn* Pawn{Cast<ASurvivorPawn>(AIController->GetPawn())};
	if (!Pawn)
	{
		TaskFinished(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UStudentPerceptor_DeRonBauwen* Perceptor{Pawn->GetComponentByClass<UStudentPerceptor_DeRonBauwen>()};
	if (!Perceptor)
	{
		TaskFinished(OwnerComp, EBTNodeResult::Failed);
		return;
	}

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

	// Cancel task when we get stuck
	UPawnMovementComponent* MovementComponent{Pawn->GetMovementComponent()};
	auto const MovementSpeed{MovementComponent->GetMaxSpeed()};

	if (!Steering)
	{
		TaskFinished(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	float const MoveAmount = FVector::DistSquared(Pawn->GetActorLocation(), LastLocation);
	if (MoveAmount <= 1.0f) StuckTime += DeltaSeconds; else StuckTime = 0.f;
	LastLocation = Pawn->GetActorLocation();

	if (StuckTime > MaxStuckTime) {
		TaskFinished(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (CurrentPath->IsValid())
	{
		const TArray<FNavPathPoint>& Points = CurrentPath->GetPathPoints();
		if (CurrentPathIndex >= Points.Num())
		{
			TaskFinished(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		FVector Waypoint = Points[CurrentPathIndex].Location;
		Waypoint.Z = Pawn->GetActorLocation().Z; // keep on ground
		SeekBehavior->Behavior->SetTarget(FTargetData{Waypoint});

		const FSteeringOutput Out = Steering->CalculateSteering(DeltaSeconds, *Pawn);
		const FVector TargetVel{Out.LinearVelocity, 0.0f};
		Pawn->SetActorRotation(TargetVel.Rotation());
		Pawn->AddMovementInput(TargetVel * MovementSpeed * DeltaSeconds);

		if (FVector::DistSquared(Pawn->GetActorLocation(), Waypoint) <= FMath::Square(WaypointAcceptanceRadius))
		{
			CurrentPathIndex++;
			
			if (CurrentPathIndex < Points.Num())
			{
				FVector Next = Points[CurrentPathIndex].Location;
				SeekBehavior->Behavior->SetTarget(FTargetData{Next});
			}
		}
	}
	else
	{
		TaskFinished(OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTT_SteerTowardAvoidingZombies_DeRonBauwen::TaskFinished(UBehaviorTreeComponent& OwnerComp,
	EBTNodeResult::Type TaskResult) const
{
	const AAIController* AIController{OwnerComp.GetAIOwner()};
	if (!AIController) return;
	
	ASurvivorPawn* Pawn{Cast<ASurvivorPawn>(AIController->GetPawn())};
	if (!Pawn) return;

	Pawn->StopRunning();
	FinishLatentTask(OwnerComp, TaskResult);
}
