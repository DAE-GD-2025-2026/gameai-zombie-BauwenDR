#include "Behaviors/BTT_SteerTowardAvoidingZombies_DeRonBauwen.h"

UBTT_SteerTowardAvoidingZombies_DeRonBauwen::UBTT_SteerTowardAvoidingZombies_DeRonBauwen()
{
}

EBTNodeResult::Type UBTT_SteerTowardAvoidingZombies_DeRonBauwen::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                                                             uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
