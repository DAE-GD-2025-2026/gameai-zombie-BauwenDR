#include "Behaviors/BTS_DetectLowStamina_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/Food.h"
#include "Survivor/SurvivorPawn.h"

void UBTS_DetectLowStamina_DeRonBauwen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto Blackboard = OwnerComp.GetBlackboardComponent();
	
	AAIController const *AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;
		
	ASurvivorPawn const *Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (!Survivor) return;

	UInventoryComponent const *Inventory{Survivor->GetComponentByClass<UInventoryComponent>()};
	UStaminaComponent const *Stamina{Survivor->GetComponentByClass<UStaminaComponent>()};
	if (!Inventory || !Stamina) return;

	for (int It{4}; It > 1; --It)
	{
		auto const FoodItem{Cast<AFood>(Inventory->GetInventory()[It])};
		if (!FoodItem) continue;
	
		auto const StaminaValue{FoodItem->GetValue()};
		auto const StaminaDifference{Stamina->GetMaxStamina() - Stamina->GetCurrentStamina()};

		if (StaminaDifference >= StaminaValue)
		{
			Blackboard->SetValueAsInt(ItemToUseKey.SelectedKeyName, It);
			Blackboard->SetValueAsBool(IsLowOnStaminaKey.SelectedKeyName, true);
			break;
		}
	}
}
