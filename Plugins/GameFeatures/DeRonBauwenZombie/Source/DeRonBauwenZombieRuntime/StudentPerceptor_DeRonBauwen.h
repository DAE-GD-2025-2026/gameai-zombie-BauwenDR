#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/ActorComponent.h"
#include "Items/BaseItem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "Village/House/House.h"
#include "Zombies/BaseZombie.h"
#include "StudentPerceptor_DeRonBauwen.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DERONBAUWENZOMBIERUNTIME_API UStudentPerceptor_DeRonBauwen : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStudentPerceptor_DeRonBauwen();
	virtual ~UStudentPerceptor_DeRonBauwen() override;
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	ABaseItem* GetClosestRememberedItemOfType(EItemType type);
	AHouse* GetClosestRememberedHouse(bool MarkVisited = true);
	ABaseZombie* GetClosestZombie();
	
private:
	UBlackboardComponent* BlackboardComp;
	ASurvivorPawn* SurvivorPawn;

	TArray<ABaseItem*> RememberedItems{};
	TArray<AHouse*> RememberedHouses{};
	TArray<ABaseZombie*> RememberedZombies{};
	
	TArray<AHouse*> RememberedVisitedHouses{};
};
