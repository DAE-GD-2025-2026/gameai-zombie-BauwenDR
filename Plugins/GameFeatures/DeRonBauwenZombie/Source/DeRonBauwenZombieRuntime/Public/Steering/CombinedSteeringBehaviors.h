#pragma once
#include <vector>

#include "Steering/SteeringBehaviors.h"

//****************
//BLENDED STEERING
class FBlendedSteering final: public ISteeringBehavior
{
public:
	struct FWeightedBehavior
	{
		ISteeringBehavior* Behavior{};
		float Weight{0.0f};

		FWeightedBehavior(ISteeringBehavior* Behavior, float Weight) :
			Behavior(Behavior),
			Weight(Weight)
		{};
	};

	explicit FBlendedSteering(const std::vector<FWeightedBehavior>& WeightedBehaviors);

	void AddBehaviour(const FWeightedBehavior& WeightedBehavior) { WeightedBehaviors.push_back(WeightedBehavior); }

	float* GetWeight(ISteeringBehavior* const SteeringBehavior);
	
	// returns a reference to the weighted behaviors, can be used to adjust weighting. Is not intended to alter the behaviors themselves.
	std::vector<FWeightedBehavior>& GetWeightedBehaviorsRef() { return WeightedBehaviors; }
	
	virtual FSteeringOutput CalculateSteering(float DeltaT, AActor& Agent) override;
	
private:
	std::vector<FWeightedBehavior> WeightedBehaviors = {};
};

//*****************
//PRIORITY STEERING
class FPrioritySteering final: public ISteeringBehavior
{
public:
	explicit FPrioritySteering(const std::vector<ISteeringBehavior*>& PriorityBehaviors)
		:PriorityBehaviors(PriorityBehaviors) 
	{}

	void AddBehaviour(ISteeringBehavior* const Behavior) { PriorityBehaviors.push_back(Behavior); }
	
	virtual FSteeringOutput CalculateSteering(float DeltaT, AActor& Agent) override;
	
private:
	std::vector<ISteeringBehavior*> PriorityBehaviors = {};
};