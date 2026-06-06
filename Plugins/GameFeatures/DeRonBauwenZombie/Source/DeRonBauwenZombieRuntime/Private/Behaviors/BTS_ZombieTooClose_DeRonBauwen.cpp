#include "Behaviors/BTS_ZombieTooClose_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "DeRonBauwenZombieRuntime/StudentPerceptor_DeRonBauwen.h"
#include "Items/Shotgun.h"
#include "Survivor/SurvivorPawn.h"

void UBTS_ZombieTooClose_DeRonBauwen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp{OwnerComp.GetBlackboardComponent()};

	AAIController const *AIController{OwnerComp.GetAIOwner()};
	if (!AIController) return;
	
	ASurvivorPawn const *Survivor{Cast<ASurvivorPawn>(AIController->GetPawn())};
	if (!Survivor) return;
	
	UStudentPerceptor_DeRonBauwen* Perceptor{Survivor->GetComponentByClass<UStudentPerceptor_DeRonBauwen>()};
	if (!Perceptor) return;
	
	UInventoryComponent const *Inventory{Survivor->GetComponentByClass<UInventoryComponent>()};
	if (!Inventory) return;

	ABaseItem const *GunItem{Inventory->GetInventory()[1]};
	ABaseZombie *ClosestZombie{Perceptor->GetClosestZombie()};

	if (!GunItem || !ClosestZombie)
	{
		BlackboardComp->SetValueAsBool(IsZombieTooCloseKey.SelectedKeyName, false);
		return;
	}

	bool const IsShotGun{Cast<AShotgun>(GunItem) != nullptr};
	double const Distance{FVector::DistSquared(ClosestZombie->GetActorLocation(), Survivor->GetActorLocation())};

	bool const ShouldShoot{
		(IsShotGun && Distance < ShotgunDistance * ShotgunDistance)
		|| (!IsShotGun && Distance < PistolDistance * PistolDistance)
	};

	BlackboardComp->SetValueAsBool(IsZombieTooCloseKey.SelectedKeyName, ShouldShoot);
	BlackboardComp->SetValueAsVector(ClosestZombieKey.SelectedKeyName, ClosestZombie->GetActorLocation());
}
