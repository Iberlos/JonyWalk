#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JonyWalk_5_7MovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_Skating    UMETA(DisplayName = "Skating"),
	CMOVE_Rail   UMETA(DisplayName = "RailSlide"),
};

UCLASS()
class JONYWALK_5_7_API UJonyWalk_5_7MovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UJonyWalk_5_7MovementComponent();

	ECustomMovementMode CustomMovementMode = CMOVE_Skating;
	float MaxSkatingSpeed = 1200.0f;
	float Drag = 10.0f;
	float Break = 2400.0f;
	FVector PreviousGroundNormal = FVector::ZeroVector;
	EMovementMode OldMovementMode = MOVE_None;

	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	void PhysSkating(float DeltaTime, int32 Iterations);
	void PhysRailSlide(float DeltaTime, int32 Iterations);
	virtual bool IsMovingOnGround() const override;
	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) override;

public:
	void DoMovement(float DeltaTime, FVector2D MovementVector);
	void DoWalkingMovement(float DeltaTime, FVector2D MovementVector);
	void DoSkatingMovement(float DeltaTime, FVector2D MovementVector);
	void DoRailMovement(float DeltaTime, FVector2D MovementVector);
};
