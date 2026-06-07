#include "Behaviors/BTS_DetectLowHealth_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/Medkit.h"
#include "Survivor/SurvivorPawn.h"

void UBTS_DetectLowHealth_DeRonBauwen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                float DeltaSeconds)
{
	auto const Blackboard{OwnerComp.GetBlackboardComponent()};
	if (!Blackboard) return;
	
	AAIController const *AIController{OwnerComp.GetAIOwner()};
	if (!AIController) return;
		
	ASurvivorPawn const *Survivor{Cast<ASurvivorPawn>(AIController->GetPawn())};
	if (!Survivor) return;

	UInventoryComponent const *Inventory{Survivor->GetComponentByClass<UInventoryComponent>()};
	UHealthComponent const *Health{Survivor->GetComponentByClass<UHealthComponent>()};
	if (!Inventory || !Health) return;

	auto const HealthItem{Cast<AMedkit>(Inventory->GetInventory()[0])};
	if (!HealthItem) return;
	
	auto const HealAmount{HealthItem->GetValue()};
	auto const HealthDifference{Health->GetMaxHealth() - Health->GetHealth()};

	if (HealthDifference >= HealAmount)
	{
		Blackboard->SetValueAsInt(ItemToUseKey.SelectedKeyName, 0);
		Blackboard->SetValueAsBool(IsLowOnHealthKey.SelectedKeyName, true);
	}
}
