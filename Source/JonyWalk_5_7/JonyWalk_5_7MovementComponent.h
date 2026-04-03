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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Custom", meta = (AllowPrivateAccess = "true"))
	float MaxSkatingSpeed = 1200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Custom", meta = (AllowPrivateAccess = "true"))
	float MaxSkatingAcceleration = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Custom", meta = (AllowPrivateAccess = "true"))
	float Drag = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Custom", meta = (AllowPrivateAccess = "true"))
	float Break = 2400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Custom", meta = (AllowPrivateAccess = "true"))
	float SkatingAirControl = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Custom", meta = (AllowPrivateAccess = "true"))

	float SkatingGroundFriction = 0.0f;

	ECustomMovementMode CustomMovementMode = CMOVE_Skating;
	FVector PreviousGroundNormal = FVector::ZeroVector;
	EMovementMode OldMovementMode = MOVE_None;
	FVector VelocityPriorToJumping = FVector::ZeroVector;
	bool KeepVelocityAfterJumping = false;
	FVector2D InputVector = FVector2D::ZeroVector;

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
