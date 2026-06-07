#include "StudentPerceptor_DeRonBauwen.h"

#include "AIController.h"
#include "Village/House/House.h"

UStudentPerceptor_DeRonBauwen::UStudentPerceptor_DeRonBauwen()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UStudentPerceptor_DeRonBauwen::~UStudentPerceptor_DeRonBauwen()
{
	RememberedZombies.SetNum(0);
}

void UStudentPerceptor_DeRonBauwen::BeginPlay()
{
	Super::BeginPlay();

	if (auto *PerceptionComp{GetOwner()->GetComponentByClass<UAIPerceptionComponent>()})
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor_DeRonBauwen::OnPerceptionUpdated);
	}

	if (auto *AiController{Cast<AAIController>(GetOwner()->GetInstigatorController())})
	{
		BlackboardComp = AiController->GetBlackboardComponent();
		SurvivorPawn = Cast<ASurvivorPawn>(AiController->GetPawn());
	}
}

void UStudentPerceptor_DeRonBauwen::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	RememberedZombies.RemoveAll([](ABaseZombie const *Zombie)
	{
		return !IsValid(Zombie);
	});
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

	if (ABaseZombie* Zombie{Cast<ABaseZombie>(Actor)}; Zombie != nullptr)
	{
		if (!RememberedZombies.Contains(Zombie))
		{
			RememberedZombies.Add(Zombie);
		}
	}
}

ABaseItem* UStudentPerceptor_DeRonBauwen::GetClosestRememberedItemOfType(EItemType type)
{
	ABaseItem* Closest{};
	double ClosestDistance{FLT_MAX};

	for (const auto& Item : RememberedItems)
	{
		if (Item->GetItemType() != type)
		{
			continue;
		}

		const double Distance{FVector::DistSquared(Item->GetActorLocation(), SurvivorPawn->GetActorLocation())};

		if (Distance >= ClosestDistance)
		{
			continue;
		}

		Closest = Item;
		ClosestDistance = Distance;
	}

	return Closest;
}

AHouse* UStudentPerceptor_DeRonBauwen::GetClosestRememberedHouse(bool MarkVisited)
{
	AHouse* Closest{};
	double ClosestDistance{FLT_MAX};

	for (const auto& House : RememberedHouses)
	{
		const double Distance{FVector::DistSquared(House->GetActorLocation(), SurvivorPawn->GetActorLocation())};

		if (Distance >= ClosestDistance)
		{
			continue;
		}

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

ABaseZombie* UStudentPerceptor_DeRonBauwen::GetClosestZombie()
{
	ABaseZombie* Closest{};
	double ClosestDistance{FLT_MAX};

	for (const auto& Zombie : RememberedZombies)
	{
		const double Distance{FVector::DistSquared(Zombie->GetActorLocation(), SurvivorPawn->GetActorLocation())};

		if (Distance >= ClosestDistance)
		{
			continue;
		}

		Closest = Zombie;
		ClosestDistance = Distance;
	}

	return Closest;
}

TArray<ABaseItem*> const& UStudentPerceptor_DeRonBauwen::GetItems()
{
	return RememberedItems;
}

TArray<AHouse*> const& UStudentPerceptor_DeRonBauwen::GetHouses()
{
	return RememberedHouses;
}

TArray<ABaseZombie*> const &UStudentPerceptor_DeRonBauwen::GetZombies()
{
	return RememberedZombies;
}
