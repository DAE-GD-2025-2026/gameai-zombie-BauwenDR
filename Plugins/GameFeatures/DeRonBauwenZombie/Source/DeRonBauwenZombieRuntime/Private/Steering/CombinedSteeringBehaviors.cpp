#include "Steering/CombinedSteeringBehaviors.h"
#include <algorithm>

FBlendedSteering::FBlendedSteering(const std::vector<FWeightedBehavior>& WeightedBehaviors)
	: WeightedBehaviors(WeightedBehaviors)
{
};

//****************
//BLENDED STEERING
float* FBlendedSteering::GetWeight(ISteeringBehavior* const SteeringBehavior)
{
	const auto It{
		std::ranges::find_if(WeightedBehaviors, [SteeringBehavior](const FWeightedBehavior& Elem)
		{
			return Elem.Behavior == SteeringBehavior;
		})
	};

	if (It != WeightedBehaviors.end())
	{
		return &It->Weight;
	}

	return nullptr;
}

FSteeringOutput FBlendedSteering::CalculateSteering(float DeltaT, AActor& Agent)
{
	FSteeringOutput BlendedOutput{};

	for (const auto& Behaviour : WeightedBehaviors)
	{
		FSteeringOutput Steering{Behaviour.Behavior->CalculateSteering(DeltaT, Agent)};
		BlendedOutput.LinearVelocity += Steering.LinearVelocity.GetSafeNormal() * Behaviour.Weight;
		BlendedOutput.AngularVelocity += Steering.AngularVelocity * Behaviour.Weight;
	}

	return BlendedOutput;
}

//*****************
//PRIORITY STEERING
FSteeringOutput FPrioritySteering::CalculateSteering(float DeltaT, AActor& Agent)
{
	FSteeringOutput Steering = {};

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
