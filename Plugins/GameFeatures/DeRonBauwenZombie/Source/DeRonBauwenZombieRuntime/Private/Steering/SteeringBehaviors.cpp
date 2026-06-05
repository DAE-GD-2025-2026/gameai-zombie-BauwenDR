#include "Steering/SteeringBehaviors.h"

#include "Steering/SteeringTypes.h"

// Seek
//*******
FSteeringOutput FSeek::CalculateSteering(const float DeltaT, AActor& Agent)
{
	FSteeringOutput Steering{};

	Steering.LinearVelocity = Target.Position - Agent.GetActorLocation();

	return Steering;
}

// Flee
//*******
FSteeringOutput FFlee::CalculateSteering(const float DeltaT, AActor& Agent)
{
	FSteeringOutput Steering{FSeek::CalculateSteering(DeltaT, Agent)};
	Steering.LinearVelocity = -Steering.LinearVelocity;

	return Steering;
}

// Pursuit
//*******
FSteeringOutput FPursuit::CalculateSteering(float DeltaT, AActor& Agent)
{
	const FTargetData OriginalTarget{Target};
	
	const FVector Distance{Target.Position - Agent.GetActorLocation()};
	const float SeekSpeed = Distance.Length() / Agent.GetVelocity().Length();
	const FVector FuturePosition{Target.Position + Target.LinearVelocity * SeekSpeed};

	SetTarget(FTargetData(FuturePosition));
	auto Steering{FSeek::CalculateSteering(DeltaT, Agent)};
	SetTarget(OriginalTarget);

	return Steering;
}

// Evade
//*******
FSteeringOutput FEvade::CalculateSteering(float DeltaT, AActor& Agent)
{
	const bool IsValid = (Target.Position - Agent.GetActorLocation()).SquaredLength() <= EvadeRadius * EvadeRadius;
	
	FSteeringOutput Steering{FPursuit::CalculateSteering(DeltaT, Agent)};

	Steering.LinearVelocity = -Steering.LinearVelocity;
	Steering.IsValid = IsValid;

	return Steering;
}

// Wander
//*******
FSteeringOutput FWander::CalculateSteering(float DeltaT, AActor& Agent)
{
	const FVector CircleCenter{Agent.GetVelocity().GetSafeNormal() * OffsetDistance};
	const FVector Displacement{FMath::Cos(WanderAngle) * Radius, FMath::Sin(WanderAngle) * Radius, 0.0f};

	WanderAngle += FMath::FRand() * MaxAngleChange - MaxAngleChange * 0.5f;
	SetTarget(FTargetData(Agent.GetActorLocation() + CircleCenter + Displacement));
	
	return FSeek::CalculateSteering(DeltaT, Agent);
}
