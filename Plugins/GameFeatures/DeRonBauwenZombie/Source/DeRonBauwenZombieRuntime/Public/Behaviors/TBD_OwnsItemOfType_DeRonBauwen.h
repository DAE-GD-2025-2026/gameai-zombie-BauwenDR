#pragma once

#include "CoreMinimal.h"
#include "Items/ItemType.h"
#include "BehaviorTree/BTDecorator.h"
#include "TBD_OwnsItemOfType_DeRonBauwen.generated.h"

UCLASS()
class DERONBAUWENZOMBIERUNTIME_API UTBD_OwnsItemOfType_DeRonBauwen final : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	EItemType ItemType;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
