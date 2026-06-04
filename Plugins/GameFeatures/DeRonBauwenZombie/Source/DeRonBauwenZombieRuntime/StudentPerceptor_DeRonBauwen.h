#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/ActorComponent.h"
#include "Items/BaseItem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "StudentPerceptor_DeRonBauwen.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DERONBAUWENZOMBIERUNTIME_API UStudentPerceptor_DeRonBauwen : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStudentPerceptor_DeRonBauwen();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Blackboard")
	FBlackboardKeySelector SelfActorKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Blackboard")
	FBlackboardKeySelector ClosestMedkitPtrKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Blackboard")
	FBlackboardKeySelector ClosestWeaponPtrKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Blackboard")
	FBlackboardKeySelector ClosestFoodPtrKey;
	
private:
	TObjectPtr<UBlackboardComponent> BlackboardComp;
	
	void ItemPerceptor(ABaseItem *NewItem);
};
