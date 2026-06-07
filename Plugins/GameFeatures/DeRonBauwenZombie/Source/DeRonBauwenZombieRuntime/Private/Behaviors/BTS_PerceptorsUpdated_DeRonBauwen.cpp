#include "Behaviors/BTS_PerceptorsUpdated_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeRonBauwenZombieRuntime/StudentPerceptor_DeRonBauwen.h"
#include "Survivor/SurvivorPawn.h"

void UBTS_PerceptorsUpdated_DeRonBauwen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                  float DeltaSeconds)
{
	UBlackboardComponent* Blackboard{OwnerComp.GetBlackboardComponent()};
	if (!Blackboard) return;

	AAIController const *AiController{OwnerComp.GetAIOwner()};
	if (!AiController) return;
	
	ASurvivorPawn const *Survivor{Cast<ASurvivorPawn>(AiController->GetPawn())};
	if (!Survivor) return;
	
	UStudentPerceptor_DeRonBauwen* Perceptor{Survivor->GetComponentByClass<UStudentPerceptor_DeRonBauwen>()};
	if (!Perceptor) return;

	if (Perceptor->GetItems().Num() > LastItems || Perceptor->GetHouses().Num() > LastHouses)
	{
		Blackboard->SetValueAsBool(PerceptorsUpdatedKey.SelectedKeyName, true);
	}

	LastItems = Perceptor->GetItems().Num();
	LastHouses = Perceptor->GetHouses().Num();
}
