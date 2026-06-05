// Fill out your copyright notice in the Description page of Project Settings.


#include "Behaviors/BTT_TargetClosestHouse_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeRonBauwenZombieRuntime/StudentPerceptor_DeRonBauwen.h"
#include "Village/House/House.h"

EBTNodeResult::Type UBTT_TargetClosestHouse_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	AAIController* AIController{OwnerComp.GetAIOwner()};
	ASurvivorPawn const *Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	UStudentPerceptor_DeRonBauwen* Perceptor{Survivor->GetComponentByClass<UStudentPerceptor_DeRonBauwen>()};
	
	AHouse* ClosestHouse{Perceptor->GetClosestRememberedHouse()};
	if (!ClosestHouse)
	{
		return EBTNodeResult::Failed;
	}
	
	BlackboardComp->SetValueAsVector(ResultPositionKey.SelectedKeyName, ClosestHouse->GetActorLocation());
	return EBTNodeResult::Succeeded;
}
