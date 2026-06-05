#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UObject/Object.h"
#include "BTT_Rotate360_DeRonBauwen.generated.h"

UCLASS()
class DERONBAUWENZOMBIERUNTIME_API UBTT_Rotate360_DeRonBauwen : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Rotate360_DeRonBauwen();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Spin")
	float Duration{1.0f};

	UPROPERTY(EditAnywhere, Category = "Spin")
	bool bUseRelativeStart{true};	
};
