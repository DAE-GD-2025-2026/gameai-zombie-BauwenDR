#pragma once

#include "CoreMinimal.h"

//SteeringParams (alias TargetData)
struct FSteeringParams final //Also used as Target for SteeringBehaviors
{
	FVector Position;
	float Orientation;
	
	FVector LinearVelocity;
	float AngularVelocity;

	explicit FSteeringParams(const FVector& Position = FVector::ZeroVector, float Orientation = 0.f, 
	                         const FVector& LinearVel = FVector::ZeroVector, float AngularVel = 0.f) :
		Position(Position),
		Orientation(Orientation),
		LinearVelocity(LinearVel),
		AngularVelocity(AngularVel)
	{}

	void Clear()
	{
		Position = FVector::ZeroVector;
		LinearVelocity = FVector::ZeroVector;

		Orientation = 0.f;
		AngularVelocity = 0.f;
	}

	FSteeringParams(const FSteeringParams& Other)
	{
		Position = Other.Position;
		Orientation = Other.Orientation;
		LinearVelocity = Other.LinearVelocity;
		AngularVelocity = Other.AngularVelocity;
	}

	FSteeringParams& operator=(const FSteeringParams& Other)
	{
		Position = Other.Position;
		Orientation = Other.Orientation;
		LinearVelocity = Other.LinearVelocity;
		AngularVelocity = Other.AngularVelocity;

		return *this;
	}

	bool operator==(const FSteeringParams& Other) const
	{
		return Position == Other.Position && Orientation == Other.Orientation && LinearVelocity == Other.LinearVelocity && AngularVelocity == Other.AngularVelocity;
	}

	bool operator!=(const FSteeringParams& Other) const
	{
		return Position != Other.Position || Orientation != Other.Orientation || LinearVelocity != Other.LinearVelocity || AngularVelocity != Other.AngularVelocity;
	}
};

using FTargetData = FSteeringParams; //Alias for SteeringBehavior usage ( Bit clearer in its context ;) )

struct FSteeringOutput final
{
	FVector LinearVelocity{};
	float AngularVelocity{0.f};
	bool IsValid{true};

	FSteeringOutput(const FVector& LinearVelocity = {0.0f, 0.0f, 0.0f}, float AngularVelocity = 0.f)
		: LinearVelocity(LinearVelocity)
		  , AngularVelocity(AngularVelocity)
	{}

	FSteeringOutput& operator=(const FSteeringOutput& Other)
	{
		LinearVelocity = Other.LinearVelocity;
		AngularVelocity = Other.AngularVelocity;
		IsValid = Other.IsValid;

		return *this;
	}

	FSteeringOutput& operator+(const FSteeringOutput& Other)
	{
		LinearVelocity += Other.LinearVelocity;
		AngularVelocity += Other.AngularVelocity;

		return *this;
	}

	FSteeringOutput& operator*=(const FSteeringOutput& Other)
	{
		LinearVelocity = LinearVelocity * Other.LinearVelocity;
		AngularVelocity = AngularVelocity * Other.AngularVelocity;

		return *this;
	}

	FSteeringOutput& operator*=(float Factor)
	{
		LinearVelocity = Factor * LinearVelocity;
		AngularVelocity = Factor * AngularVelocity;

		return *this;
	}

	FSteeringOutput& operator/=(float Factor)
	{
		LinearVelocity = LinearVelocity / Factor;
		AngularVelocity = AngularVelocity / Factor;

		return *this;
	}
};
