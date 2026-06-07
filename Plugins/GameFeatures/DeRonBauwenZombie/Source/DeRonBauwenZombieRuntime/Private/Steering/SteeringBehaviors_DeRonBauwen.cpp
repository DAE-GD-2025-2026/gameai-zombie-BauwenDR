#include "Steering/SteeringBehaviors_DeRonBauwen.h"

#include "Steering/SteeringDataTypes_DeRonBauwen.h"

// Seek
//*******
FSteeringOutput_DeRonBauwen FSeek_DeRonBauwen::CalculateSteering(const float DeltaT, AActor& Agent)
{
	FSteeringOutput_DeRonBauwen Steering{};

	const FVector LinearVel{Target.Position - Agent.GetActorLocation()};
	Steering.LinearVelocity = FVector2D{LinearVel.X, LinearVel.Y};

	return Steering;
}

// Flee
//*******
FSteeringOutput_DeRonBauwen FFlee_DeRonBauwen::CalculateSteering(const float DeltaT, AActor& Agent)
{
	FSteeringOutput_DeRonBauwen Steering{FSeek_DeRonBauwen::CalculateSteering(DeltaT, Agent)};
	Steering.LinearVelocity = -Steering.LinearVelocity;

	return Steering;
}

// Pursuit
//*******
FSteeringOutput_DeRonBauwen FPursuit_DeRonBauwen::CalculateSteering(float DeltaT, AActor& Agent)
{
	const FTargetData_DeRonBauwen OriginalTarget{Target};
	
	const FVector Distance{Target.Position - Agent.GetActorLocation()};
	const float SeekSpeed = Distance.Length() / Agent.GetVelocity().Length();
	const FVector FuturePosition{Target.Position + Target.LinearVelocity * SeekSpeed};

	SetTarget(FTargetData_DeRonBauwen(FuturePosition));
	auto Steering{FSeek_DeRonBauwen::CalculateSteering(DeltaT, Agent)};
	SetTarget(OriginalTarget);

	return Steering;
}

// Evade
//*******
FSteeringOutput_DeRonBauwen FEvade_DeRonBauwen::CalculateSteering(float DeltaT, AActor& Agent)
{
	const bool IsValid = (Target.Position - Agent.GetActorLocation()).SquaredLength() <= EvadeRadius * EvadeRadius;
	
	FSteeringOutput_DeRonBauwen Steering{FPursuit_DeRonBauwen::CalculateSteering(DeltaT, Agent)};

	Steering.LinearVelocity = -Steering.LinearVelocity;
	Steering.IsValid = IsValid;

	return Steering;
}

// Wander
//*******
FSteeringOutput_DeRonBauwen FWander_DeRonBauwen::CalculateSteering(float DeltaT, AActor& Agent)
{
	const FVector CircleCenter{Agent.GetVelocity().GetSafeNormal() * OffsetDistance};
	const FVector Displacement{FMath::Cos(WanderAngle) * Radius, FMath::Sin(WanderAngle) * Radius, 0.0f};

	WanderAngle += FMath::FRand() * MaxAngleChange - MaxAngleChange * 0.5f;
	SetTarget(FTargetData_DeRonBauwen(Agent.GetActorLocation() + CircleCenter + Displacement));
	
	return FSeek_DeRonBauwen::CalculateSteering(DeltaT, Agent);
}
