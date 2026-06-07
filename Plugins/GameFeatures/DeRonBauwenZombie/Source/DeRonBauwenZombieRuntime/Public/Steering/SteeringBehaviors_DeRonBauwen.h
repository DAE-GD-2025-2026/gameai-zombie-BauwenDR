#pragma once
#include "SteeringDataTypes_DeRonBauwen.h"

// SteeringBehavior base, all steering behaviors should derive from this.
class ISteeringBehavior_DeRonBauwen
{
public:
	ISteeringBehavior_DeRonBauwen() = default;
	virtual ~ISteeringBehavior_DeRonBauwen() = default;

	virtual FSteeringOutput_DeRonBauwen CalculateSteering(float DeltaT, AActor& Agent) = 0;

	void SetTarget(const FTargetData_DeRonBauwen& NewTarget) { Target = NewTarget; }

protected:
	FTargetData_DeRonBauwen Target;
};

class FSeek_DeRonBauwen : public ISteeringBehavior_DeRonBauwen
{
public:
	virtual FSteeringOutput_DeRonBauwen CalculateSteering(float DeltaT, AActor& Agent) override;
};

class FFlee_DeRonBauwen final : public FSeek_DeRonBauwen
{
public:
	virtual FSteeringOutput_DeRonBauwen CalculateSteering(float DeltaT, AActor& Agent) override;
};

class FPursuit_DeRonBauwen : public FSeek_DeRonBauwen
{
public:
	virtual FSteeringOutput_DeRonBauwen CalculateSteering(float DeltaT, AActor& Agent) override;
};

class FEvade_DeRonBauwen final : public FPursuit_DeRonBauwen
{
public:
	explicit FEvade_DeRonBauwen(float EvadeRadius = 200.0f) : EvadeRadius(EvadeRadius) {}
	
	virtual FSteeringOutput_DeRonBauwen CalculateSteering(float DeltaT, AActor& Agent) override;
	
private:
	float EvadeRadius{};
};

class FWander_DeRonBauwen final : public FSeek_DeRonBauwen
{
public:
	virtual FSteeringOutput_DeRonBauwen CalculateSteering(float DeltaT, AActor& Agent) override;
	void SetAngle(float NewAngle) {WanderAngle = NewAngle;}

private:
	float OffsetDistance{6.0f};
	float Radius{3.0f};
	float MaxAngleChange{FMath::DegreesToRadians(22.0f)};
	float WanderAngle{0.0f};
};
