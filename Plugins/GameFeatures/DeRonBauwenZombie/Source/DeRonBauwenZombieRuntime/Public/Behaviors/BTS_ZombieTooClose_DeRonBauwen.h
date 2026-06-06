#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_ZombieTooClose_DeRonBauwen.generated.h"

UCLASS()
class DERONBAUWENZOMBIERUNTIME_API UBTS_ZombieTooClose_DeRonBauwen : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IsZombieTooCloseKey;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ClosestZombieKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Detection")
	double ShotgunDistance{200.0};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Detection")
	double PistolDistance{500.0};
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
