#pragma once
#include <vector>
#include <memory>

#include "Steering/SteeringBehaviors_DeRonBauwen.h"

//****************
//BLENDED STEERING
class FBlendedSteering_DeRonBauwen final: public ISteeringBehavior_DeRonBauwen
{
public:
	struct FWeightedBehavior
	{
		std::unique_ptr<ISteeringBehavior_DeRonBauwen> Behavior{};
		float Weight{0.0f};

		FWeightedBehavior(std::unique_ptr<ISteeringBehavior_DeRonBauwen> Behavior, float Weight) :
			Behavior(std::move(Behavior)),
			Weight(Weight)
		{}
	};

	explicit FBlendedSteering_DeRonBauwen(const std::vector<FWeightedBehavior*>& WeightedBehaviors);

	void AddBehaviour(FWeightedBehavior *WeightedBehavior) { WeightedBehaviors.push_back(WeightedBehavior); }
	void RemoveBehaviour(FWeightedBehavior *Behavior) { auto _{std::ranges::remove(WeightedBehaviors, Behavior)}; }

	float* GetWeight(ISteeringBehavior_DeRonBauwen* const SteeringBehavior);
	
	// returns a reference to the weighted behaviors, can be used to adjust weighting. Is not intended to alter the behaviors themselves.
	std::vector<FWeightedBehavior*>& GetWeightedBehaviorsRef() { return WeightedBehaviors; }
	
	virtual FSteeringOutput_DeRonBauwen CalculateSteering(float DeltaT, AActor& Agent) override;
	
private:
	std::vector<FWeightedBehavior*> WeightedBehaviors{};
};

//*****************
//PRIORITY STEERING
class FPrioritySteering final: public ISteeringBehavior_DeRonBauwen
{
public:
	explicit FPrioritySteering(const std::vector<ISteeringBehavior_DeRonBauwen*>& PriorityBehaviors)
		:PriorityBehaviors(PriorityBehaviors) 
	{}

	void AddBehaviour(ISteeringBehavior_DeRonBauwen* const Behavior) { PriorityBehaviors.push_back(Behavior); }
	void RemoveBehaviour(ISteeringBehavior_DeRonBauwen* Behavior) { auto _{std::ranges::remove(PriorityBehaviors, Behavior)}; }
	
	virtual FSteeringOutput_DeRonBauwen CalculateSteering(float DeltaT, AActor& Agent) override;
	
private:
	std::vector<ISteeringBehavior_DeRonBauwen*> PriorityBehaviors = {};
};