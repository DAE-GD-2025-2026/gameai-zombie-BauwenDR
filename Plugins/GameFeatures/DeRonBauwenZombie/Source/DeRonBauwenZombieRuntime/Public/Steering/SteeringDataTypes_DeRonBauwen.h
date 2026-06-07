#pragma once

#include "CoreMinimal.h"

//SteeringParams (alias TargetData)
struct FSteeringParams_DeRonBauwen final //Also used as Target for SteeringBehaviors
{
	FVector Position;
	float Orientation;
	
	FVector LinearVelocity;
	float AngularVelocity;

	explicit FSteeringParams_DeRonBauwen(const FVector& Position = FVector::ZeroVector, float Orientation = 0.f, 
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

	FSteeringParams_DeRonBauwen(const FSteeringParams_DeRonBauwen& Other)
	{
		Position = Other.Position;
		Orientation = Other.Orientation;
		LinearVelocity = Other.LinearVelocity;
		AngularVelocity = Other.AngularVelocity;
	}

	FSteeringParams_DeRonBauwen& operator=(const FSteeringParams_DeRonBauwen& Other)
	{
		Position = Other.Position;
		Orientation = Other.Orientation;
		LinearVelocity = Other.LinearVelocity;
		AngularVelocity = Other.AngularVelocity;

		return *this;
	}

	bool operator==(const FSteeringParams_DeRonBauwen& Other) const
	{
		return Position == Other.Position && Orientation == Other.Orientation && LinearVelocity == Other.LinearVelocity && AngularVelocity == Other.AngularVelocity;
	}

	bool operator!=(const FSteeringParams_DeRonBauwen& Other) const
	{
		return Position != Other.Position || Orientation != Other.Orientation || LinearVelocity != Other.LinearVelocity || AngularVelocity != Other.AngularVelocity;
	}
};

using FTargetData_DeRonBauwen = FSteeringParams_DeRonBauwen; //Alias for SteeringBehavior usage ( Bit clearer in its context ;) )

struct FSteeringOutput_DeRonBauwen final
{
	FVector2D LinearVelocity{};
	bool IsValid{true};

	FSteeringOutput_DeRonBauwen(const FVector& LinearVelocity = {0.0f, 0.0f, 0.0f})
		: LinearVelocity(LinearVelocity)
	{}

	FSteeringOutput_DeRonBauwen& operator=(const FSteeringOutput_DeRonBauwen& Other)
	{
		LinearVelocity = Other.LinearVelocity;
		IsValid = Other.IsValid;

		return *this;
	}

	FSteeringOutput_DeRonBauwen& operator+(const FSteeringOutput_DeRonBauwen& Other)
	{
		LinearVelocity += Other.LinearVelocity;

		return *this;
	}

	FSteeringOutput_DeRonBauwen& operator*=(const FSteeringOutput_DeRonBauwen& Other)
	{
		LinearVelocity = LinearVelocity * Other.LinearVelocity;

		return *this;
	}

	FSteeringOutput_DeRonBauwen& operator*=(float Factor)
	{
		LinearVelocity = Factor * LinearVelocity;

		return *this;
	}

	FSteeringOutput_DeRonBauwen& operator/=(float Factor)
	{
		LinearVelocity = LinearVelocity / Factor;

		return *this;
	}
};
