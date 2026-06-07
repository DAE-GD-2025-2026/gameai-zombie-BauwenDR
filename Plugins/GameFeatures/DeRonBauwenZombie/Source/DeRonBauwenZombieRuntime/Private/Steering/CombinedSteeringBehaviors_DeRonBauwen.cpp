#include "Steering/CombinedSteeringBehaviors_DeRonBauwen.h"
#include <algorithm>

FBlendedSteering_DeRonBauwen::FBlendedSteering_DeRonBauwen(const std::vector<FWeightedBehavior*>& WeightedBehaviors)
	: WeightedBehaviors(WeightedBehaviors)
{
};

//****************
//BLENDED STEERING
float* FBlendedSteering_DeRonBauwen::GetWeight(ISteeringBehavior_DeRonBauwen* const SteeringBehavior)
{
	const auto It{
		std::ranges::find_if(WeightedBehaviors, [SteeringBehavior](const FWeightedBehavior *Elem)
		{
			return Elem->Behavior.get() == SteeringBehavior;
		})
	};

	if (It != WeightedBehaviors.end())
	{
		return &(*It)->Weight;
	}

	return nullptr;
}

FSteeringOutput_DeRonBauwen FBlendedSteering_DeRonBauwen::CalculateSteering(float DeltaT, AActor& Agent)
{
	FSteeringOutput_DeRonBauwen BlendedOutput{};

	for (const auto& Behaviour : WeightedBehaviors)
	{
		FSteeringOutput_DeRonBauwen Steering{Behaviour->Behavior->CalculateSteering(DeltaT, Agent)};

		if (Steering.IsValid)
		{
			BlendedOutput.LinearVelocity += Steering.LinearVelocity.GetSafeNormal() * Behaviour->Weight;
		}
	}

	BlendedOutput.LinearVelocity.Normalize();
	return BlendedOutput;
}

//*****************
//PRIORITY STEERING
FSteeringOutput_DeRonBauwen FPrioritySteering::CalculateSteering(float DeltaT, AActor& Agent)
{
	FSteeringOutput_DeRonBauwen Steering = {};

	for (const auto& Behavior : PriorityBehaviors)
	{
		Steering = Behavior->CalculateSteering(DeltaT, Agent);

		if (Steering.IsValid)
		{
			break;
		}
	}

	//If none of the behavior return a valid output, last behavior is returned
	return Steering;
}
