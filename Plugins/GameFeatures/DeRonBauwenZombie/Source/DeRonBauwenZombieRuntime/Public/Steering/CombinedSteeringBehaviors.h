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
		std::unique_ptr<ISteeringBehavior> Behavior{};
		float Weight{0.0f};

		FWeightedBehavior(std::unique_ptr<ISteeringBehavior> Behavior, float Weight) :
			Behavior(std::move(Behavior)),
			Weight(Weight)
		{}
	};

	explicit FBlendedSteering(const std::vector<FWeightedBehavior*>& WeightedBehaviors);

	void AddBehaviour(FWeightedBehavior *WeightedBehavior) { WeightedBehaviors.push_back(WeightedBehavior); }
	void RemoveBehaviour(FWeightedBehavior *Behavior) { auto _{std::ranges::remove(WeightedBehaviors, Behavior)}; }

	float* GetWeight(ISteeringBehavior* const SteeringBehavior);
	
	// returns a reference to the weighted behaviors, can be used to adjust weighting. Is not intended to alter the behaviors themselves.
	std::vector<FWeightedBehavior*>& GetWeightedBehaviorsRef() { return WeightedBehaviors; }
	
	virtual FSteeringOutput CalculateSteering(float DeltaT, AActor& Agent) override;
	
private:
	std::vector<FWeightedBehavior*> WeightedBehaviors{};
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
	void RemoveBehaviour(ISteeringBehavior* Behavior) { auto _{std::ranges::remove(PriorityBehaviors, Behavior)}; }
	
	virtual FSteeringOutput CalculateSteering(float DeltaT, AActor& Agent) override;
	
private:
	std::vector<ISteeringBehavior*> PriorityBehaviors = {};
};