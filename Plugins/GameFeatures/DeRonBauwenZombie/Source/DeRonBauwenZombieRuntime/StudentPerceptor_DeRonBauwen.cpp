#include "StudentPerceptor_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Village/House/House.h"

UStudentPerceptor_DeRonBauwen::UStudentPerceptor_DeRonBauwen()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor_DeRonBauwen::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor_DeRonBauwen::OnPerceptionUpdated);
	}
	
	if (auto AIController = Cast<AAIController>(GetOwner()->GetInstigatorController()))
	{
		BlackboardComp = AIController->GetBlackboardComponent();
		SurvivorPawn = Cast<ASurvivorPawn>(AIController->GetPawn());
	}
}

void UStudentPerceptor_DeRonBauwen::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (AHouse* House{Cast<AHouse>(Actor)}; House != nullptr)
	{
		if (!RememberedHouses.Contains(House) && !RememberedVisitedHouses.Contains(House))
		{
			RememberedHouses.Add(House);
		}
	}
	
	if (ABaseItem* Item = Cast<ABaseItem>(Actor); Item != nullptr)
	{
		if (!RememberedItems.Contains(Item))
		{
			RememberedItems.Add(Item);
		}
	}
}

ABaseItem* UStudentPerceptor_DeRonBauwen::GetClosestRememberedItemOfType(EItemType type)
{
	ABaseItem* Closest{};
	double ClosestDistance{FLT_MAX};

	for (auto const &Item : RememberedItems)
	{
		if (Item->GetItemType() != type) continue;
		
		double const Distance{FVector::DistSquared(Item->GetActorLocation(), SurvivorPawn->GetActorLocation())};

		if (Distance >= ClosestDistance) continue;
		
		Closest = Item;
		ClosestDistance = Distance;
	}

	return Closest;
}

AHouse* UStudentPerceptor_DeRonBauwen::GetClosestRememberedHouse(bool MarkVisited)
{
	AHouse* Closest{};
	double ClosestDistance{FLT_MAX};
	
	for (auto const &House : RememberedHouses)
	{
		double const Distance{FVector::DistSquared(House->GetActorLocation(), SurvivorPawn->GetActorLocation())};

		if (Distance >= ClosestDistance) continue;
		
		Closest = House;
		ClosestDistance = Distance;
	}

	if (MarkVisited)
	{
		RememberedVisitedHouses.Add(Closest);
		RememberedHouses.Remove(Closest);
	}
	
	return Closest;
}
