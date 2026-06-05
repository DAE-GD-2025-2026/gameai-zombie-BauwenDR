#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UObject/Object.h"
#include "BTT_SteerTowardAvoidingZombies_DeRonBauwen.generated.h"

UCLASS()
class DERONBAUWENZOMBIERUNTIME_API UBTT_SteerTowardAvoidingZombies_DeRonBauwen : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SteerTowardAvoidingZombies_DeRonBauwen();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
