#pragma once
#include "SteeringDataTypes.h"

// SteeringBehavior base, all steering behaviors should derive from this.
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual FSteeringOutput CalculateSteering(float DeltaT, AActor& Agent) = 0;

	void SetTarget(const FTargetData& NewTarget) { Target = NewTarget; }

protected:
	FTargetData Target;
};

class FSeek : public ISteeringBehavior
{
public:
	virtual FSteeringOutput CalculateSteering(float DeltaT, AActor& Agent) override;
};

class FFlee final : public FSeek
{
public:
	virtual FSteeringOutput CalculateSteering(float DeltaT, AActor& Agent) override;
};

class FPursuit : public FSeek
{
public:
	virtual FSteeringOutput CalculateSteering(float DeltaT, AActor& Agent) override;
};

class FEvade final : public FPursuit
{
public:
	explicit FEvade(float EvadeRadius = 200.0f) : EvadeRadius(EvadeRadius) {}
	
	virtual FSteeringOutput CalculateSteering(float DeltaT, AActor& Agent) override;
	
private:
	float EvadeRadius{};
};

class FWander final : public FSeek
{
public:
	virtual FSteeringOutput CalculateSteering(float DeltaT, AActor& Agent) override;

private:
	float OffsetDistance{6.0f};
	float Radius{4.0f};
	float MaxAngleChange{FMath::DegreesToRadians(45.0f)};
	float WanderAngle{0.0f};
};
