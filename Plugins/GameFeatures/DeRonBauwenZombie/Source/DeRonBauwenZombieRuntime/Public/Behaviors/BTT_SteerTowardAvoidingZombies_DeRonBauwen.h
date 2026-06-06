#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "NavigationPath.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Steering/CombinedSteeringBehaviors.h"
#include "UObject/Object.h"
#include "BTT_SteerTowardAvoidingZombies_DeRonBauwen.generated.h"

UCLASS()
class DERONBAUWENZOMBIERUNTIME_API UBTT_SteerTowardAvoidingZombies_DeRonBauwen : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SteerTowardAvoidingZombies_DeRonBauwen();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector DestinationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Blackboard")
	FBlackboardKeySelector IsZombieTooCloseKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float ArriveDistance{50.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float EvadeDistance{250.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float MaxStuckTime{0.3f};

private:
	std::unique_ptr<FBlendedSteering> Steering{};
	std::unique_ptr<FBlendedSteering::FWeightedBehavior> SeekBehavior{};

	TArray<std::unique_ptr<FBlendedSteering::FWeightedBehavior>> FleeBehaviors{};

	FVector LastLocation{};
	FNavPathSharedPtr CurrentPath{};
	int32 CurrentPathIndex = 0;
	float WaypointAcceptanceRadius = 100.f;
	float StuckTime{};
};
