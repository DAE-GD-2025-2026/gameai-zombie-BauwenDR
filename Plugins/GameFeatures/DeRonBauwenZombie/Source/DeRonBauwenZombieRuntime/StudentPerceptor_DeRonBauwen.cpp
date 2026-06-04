#include "StudentPerceptor_DeRonBauwen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/BaseItem.h"

UStudentPerceptor_DeRonBauwen::UStudentPerceptor_DeRonBauwen()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor_DeRonBauwen::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto const PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>(); PerceptionComp != nullptr)
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor_DeRonBauwen::OnPerceptionUpdated);
	}
	
	if (auto const AIController = Cast<AAIController>(GetOwner()->GetInstigatorController()); AIController != nullptr)
	{
		BlackboardComp = AIController->GetBlackboardComponent();
	}
}

void UStudentPerceptor_DeRonBauwen::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (ABaseItem* Item = Cast<ABaseItem>(Actor); Item != nullptr)
	{
		ItemPerceptor(Item);
	}
}

void UStudentPerceptor_DeRonBauwen::ItemPerceptor(ABaseItem* NewItem)
{
	FBlackboardKeySelector *ItemTypeKey{nullptr};
	switch (NewItem->GetItemType()) {
	case EItemType::Food:
		ItemTypeKey = &ClosestFoodPtrKey;
		break;
	case EItemType::Medkit:
		ItemTypeKey = &ClosestMedkitPtrKey;
		break;
	case EItemType::Shotgun:
		ItemTypeKey = &ClosestWeaponPtrKey;
		break;
	case EItemType::Pistol:
		ItemTypeKey = &ClosestWeaponPtrKey;
		break;
	case EItemType::Garbage:
		break;
	}
	
	if (ItemTypeKey == nullptr)
	{
		return;
	}
	
	APawn* Self{Cast<APawn>(BlackboardComp->GetValueAsObject(SelfActorKey.SelectedKeyName))};
	if (Self == nullptr)
	{
		return;
	}
	
	ABaseItem *CurrentItem{Cast<ABaseItem>(BlackboardComp->GetValueAsObject(ItemTypeKey->SelectedKeyName))};
	
	if (CurrentItem == nullptr 
		|| FVector::DistSquared(Self->GetActorLocation(), CurrentItem->GetActorLocation()) < FVector::DistSquared(Self->GetActorLocation(), NewItem->GetActorLocation()))
	{
		BlackboardComp->SetValueAsObject(SelfActorKey.SelectedKeyName, NewItem);
	}
}
