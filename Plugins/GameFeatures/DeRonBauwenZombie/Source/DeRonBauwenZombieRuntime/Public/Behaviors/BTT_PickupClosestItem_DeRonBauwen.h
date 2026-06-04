#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PickupClosestItem_DeRonBauwen.generated.h"

UCLASS()
class DERONBAUWENZOMBIERUNTIME_API UBTT_PickupClosestItem_DeRonBauwen : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector ClosestItemPtrKey;
};
