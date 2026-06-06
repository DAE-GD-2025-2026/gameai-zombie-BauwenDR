#include "Behaviors/BTT_SteerTowardAvoidingZombies_DeRonBauwen.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Survivor/SurvivorPawn.h"

float GetPawnRadius(APawn* Pawn)
{
	if (!Pawn) return 34.f;
	// If using Character with CapsuleComponent:
	if (ACharacter* Char = Cast<ACharacter>(Pawn))
	{
		if (UCapsuleComponent* Cap = Char->GetCapsuleComponent())
		{
			return Cap->GetScaledCapsuleRadius();
		}
	}

	const FNavAgentProperties& NavProps = Pawn->GetNavAgentPropertiesRef();
	if (NavProps.AgentRadius > 0.f) return NavProps.AgentRadius;

	return 34.f;
}

UBTT_SteerTowardAvoidingZombies_DeRonBauwen::UBTT_SteerTowardAvoidingZombies_DeRonBauwen()
{
	bNotifyTick = true;

	SeekBehavior = std::make_unique<FBlendedSteering::FWeightedBehavior>(std::make_unique<FSeek>(), 0.2f);
	Steering = std::make_unique<FBlendedSteering>(FBlendedSteering{{SeekBehavior.get()}});
}

EBTNodeResult::Type UBTT_SteerTowardAvoidingZombies_DeRonBauwen::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp{OwnerComp.GetBlackboardComponent()};

	AAIController* AiController{OwnerComp.GetAIOwner()};
	if (!AiController)
	{
		return EBTNodeResult::Failed;
	}

	ASurvivorPawn* Pawn{Cast<ASurvivorPawn>(AiController->GetPawn())};
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	if (!Steering)
	{
		return EBTNodeResult::Failed;
	}

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
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UPawnMovementComponent* MovementComponent{Pawn->GetMovementComponent()};
	auto const MovementSpeed{MovementComponent->GetMaxSpeed()};

	if (!Steering)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (CurrentPath->IsValid())
	{
		const TArray<FNavPathPoint>& Points = CurrentPath->GetPathPoints();
		if (CurrentPathIndex >= Points.Num())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
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
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
