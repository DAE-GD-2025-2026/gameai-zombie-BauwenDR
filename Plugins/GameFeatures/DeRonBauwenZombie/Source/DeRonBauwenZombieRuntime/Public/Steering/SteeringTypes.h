#pragma once

#include "CoreMinimal.h"

//SteeringParams (alias TargetData)
USTRUCT(BlueprintType)
struct FSteeringParams final //Also used as Target for SteeringBehaviors
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Orientation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LinearVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngularVelocity;

	explicit FSteeringParams(const FVector& Position = FVector::ZeroVector, float Orientation = 0.f, 
	                         const FVector& LinearVel = FVector::ZeroVector, float AngularVel = 0.f) :
		Position(Position),
		Orientation(Orientation),
		LinearVelocity(LinearVel),
		AngularVelocity(AngularVel)
	{}

#pragma region Functions
	void Clear()
	{
		Position = FVector::ZeroVector;
		LinearVelocity = FVector::ZeroVector;

		Orientation = 0.f;
		AngularVelocity = 0.f;
	}
#pragma endregion

#pragma region Operator Overloads
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
#pragma endregion

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
