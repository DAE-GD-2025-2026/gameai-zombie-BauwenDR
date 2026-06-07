#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_PerceptorsUpdated_DeRonBauwen.generated.h"

UCLASS()
class DERONBAUWENZOMBIERUNTIME_API UBTS_PerceptorsUpdated_DeRonBauwen : public UBTService
{
	GENERATED_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector PerceptorsUpdatedKey;

private:
	int LastItems{};
	int LastHouses{};
};
