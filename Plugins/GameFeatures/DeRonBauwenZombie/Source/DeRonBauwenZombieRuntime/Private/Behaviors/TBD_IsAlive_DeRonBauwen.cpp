#include "Behaviors/TBD_IsAlive_DeRonBauwen.h"

#include "AIController.h"
#include "Common/HealthComponent.h"
#include "Survivor/SurvivorPawn.h"

class ASurvivorPawn;

bool UTBD_IsAlive_DeRonBauwen::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController const *AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;
		
	ASurvivorPawn const *Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return false;

	UHealthComponent const *Health{Survivor->GetComponentByClass<UHealthComponent>()};
	if (!Health) return false;

	return Health->GetHealth() > 0;
}
