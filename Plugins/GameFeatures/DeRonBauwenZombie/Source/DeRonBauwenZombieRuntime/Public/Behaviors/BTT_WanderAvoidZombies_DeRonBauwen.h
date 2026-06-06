#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Steering/CombinedSteeringBehaviors.h"
#include "BTT_WanderAvoidZombies_DeRonBauwen.generated.h"

UCLASS()
class DERONBAUWENZOMBIERUNTIME_API UBTT_WanderAvoidZombies_DeRonBauwen : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_WanderAvoidZombies_DeRonBauwen();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Blackboard")
	FBlackboardKeySelector TimerCountdownKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float EvadeDistance{250.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float EvadePriority{0.7f};

private:
	std::unique_ptr<FBlendedSteering> Steering{};
	std::unique_ptr<FBlendedSteering::FWeightedBehavior> WanderBehavior{};

	TArray<std::unique_ptr<FBlendedSteering::FWeightedBehavior>> EvadeBehaviors{};
};
