// Fill out your copyright notice in the Description page of Project Settings.


#include "JonyWalk_5_7MovementComponent.h"

DECLARE_CYCLE_STAT(TEXT("Char PhysWalking"), STAT_CharPhysCustom, STATGROUP_Character);

UJonyWalk_5_7MovementComponent::UJonyWalk_5_7MovementComponent()
{
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	JumpZVelocity = 500.f;
	AirControl = 0.35f;
	MaxWalkSpeed = 500.f;
	MinAnalogWalkSpeed = 20.f;
	BrakingDecelerationWalking = 2000.f;
	BrakingDecelerationFalling = 1500.0f;
	MaxCustomMovementSpeed = MaxSkatingSpeed*2.0f;
	MaxAcceleration = 2000.0f;
}

void UJonyWalk_5_7MovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);
	PhysWalking(DeltaTime, Iterations);
}

void UJonyWalk_5_7MovementComponent::PhysSkating(float DeltaTime, int32 Iterations)
{

}

void UJonyWalk_5_7MovementComponent::PhysRailSlide(float DeltaTime, int32 Iterations)
{

}

bool UJonyWalk_5_7MovementComponent::IsMovingOnGround() const
{
	return ((MovementMode == MOVE_Walking) || (MovementMode == MOVE_NavWalking) || (MovementMode == MOVE_Custom)) && UpdatedComponent;
}

void UJonyWalk_5_7MovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	if (MovementMode == MOVE_Falling && OldMovementMode == MOVE_Custom) NewMovementMode = MOVE_Custom;
	if (MovementMode == MOVE_Walking || MovementMode == MOVE_Custom) OldMovementMode = MovementMode;
	if (NewMovementMode == MOVE_Walking || NewMovementMode == MOVE_Custom) {
		if (NewMovementMode == MOVE_Custom) {
			bOrientRotationToMovement = false;
			AirControl = SkatingAirControl;
			GroundFriction = SkatingGroundFriction;
			MaxAcceleration = MaxSkatingAcceleration;
			if (MovementMode == MOVE_Falling)
			{
				KeepVelocityAfterJumping = true;
			}
		}
		else
		{
			bOrientRotationToMovement = true;
			RotationRate = FRotator(0.0f, 500.0f, 0.0f);
			JumpZVelocity = 500.f;
			AirControl = 0.35f;
			MaxWalkSpeed = 500.f;
			MinAnalogWalkSpeed = 20.f;
			BrakingDecelerationWalking = 2000.f;
			BrakingDecelerationFalling = 1500.0f;
			MaxAcceleration = 2000.0f;
		}
		SetGroundMovementMode(NewMovementMode);
	}
	Super::SetMovementMode(NewMovementMode, NewCustomMode);
}

void UJonyWalk_5_7MovementComponent::DoMovement(float DeltaTime, FVector2D MovementVector)
{
	switch (MovementMode) {
	case MOVE_Walking: {
		DoWalkingMovement(DeltaTime, MovementVector);
		break;
	}
	case MOVE_Falling: {
		//Keep velocity while on air.
		Velocity = FVector(VelocityPriorToJumping.X, VelocityPriorToJumping.Y, Velocity.Z) ;
		break;
	}
	default: {
		switch (CustomMovementMode) {
		case CMOVE_Rail: {
			DoRailMovement(DeltaTime, MovementVector);
			break;
		}
		default: {
			DoSkatingMovement(DeltaTime, MovementVector);
			break;
		}
		}
		break;
	}
	}
}

void UJonyWalk_5_7MovementComponent::DoWalkingMovement(float deltaTime, FVector2D MovementVector)
{
	//Simple walking basically the same as what was provided by the template
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		GetPawnOwner()->AddMovementInput(ForwardDirection, MovementVector.Y);
		GetPawnOwner()->AddMovementInput(RightDirection, MovementVector.X);
	}
	VelocityPriorToJumping = Velocity;
}

void UJonyWalk_5_7MovementComponent::DoSkatingMovement(float DeltaTime, FVector2D MovementVector)
{
	if (GetController() != nullptr)
	{
		//Hard fix for a jump issue
		if (KeepVelocityAfterJumping) 
		{
			Velocity = FVector(VelocityPriorToJumping.X, VelocityPriorToJumping.Y, 0.0f);
			KeepVelocityAfterJumping = false;
		}

		FVector CurrentVelocity = Velocity;
		//Rotation
		if (!CurrentVelocity.IsNearlyZero() && MovementVector.X > 0.5 || MovementVector.X < -0.5)
		{
			FRotator CurrentRotation = GetPawnOwner()->GetActorRotation();
			FRotator TargetRotation = (GetPawnOwner()->GetActorRightVector() * MovementVector.X).Rotation(); // Your target based on input

			// Smoothly interpolate
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 1.0f);
			GetPawnOwner()->SetActorRotation(NewRotation);

			FVector2D Direction2D(GetPawnOwner()->GetActorForwardVector());
			float currentVelocityZ = CurrentVelocity.Z;
			FVector2D Velocity2D(CurrentVelocity);
			Direction2D = Direction2D * FMath::Sign(Direction2D.Dot(Velocity2D));
			float Speed2D = Velocity2D.Size();
			CurrentVelocity = CurrentVelocity.GetSafeNormal() * FVector((Direction2D * Speed2D).X, (Direction2D * Speed2D).Y, currentVelocityZ).Size();
			Velocity = FVector((Direction2D * Speed2D).X, (Direction2D * Speed2D).Y, currentVelocityZ);
		}

		//Acceleration
		if (MovementVector.Y > 0.5)
		{
			// find out which way is forward
			const FRotator Rotation = GetPawnOwner()->GetActorRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement
			GetPawnOwner()->AddMovementInput(ForwardDirection, MovementVector.Y);
		}

		//Deceleration (Drag and Break)
		CurrentVelocity = Velocity;
		float speedReduction = Drag + (MovementVector.Y < -0.5 ? Break : 0.0f);
		CurrentVelocity -= CurrentVelocity.GetSafeNormal() * speedReduction * DeltaTime;
		Velocity = CurrentVelocity.GetClampedToSize(0.0f, MaxSkatingSpeed);

		//Slope
		CurrentVelocity = Velocity;
		if (IsMovingOnGround()) {
			// Get the surface normal (ground normal)
			FVector GroundNormal = CurrentFloor.HitResult.ImpactNormal;
			//Only apply if the slope is consistent. Missing some frames won't matter.
			if (PreviousGroundNormal.Dot(GroundNormal) > 0.9f)
			{
				float InclineFraction = 1.0f - GetPawnOwner()->GetActorUpVector().Dot(GroundNormal);
				FVector ProjectedForward = FVector::VectorPlaneProject(GetPawnOwner()->GetActorForwardVector(), GroundNormal);

				//Apply partial gravity along projected forward
				FVector GravityAlongSlope = FMath::Sign(ProjectedForward.Z) * ProjectedForward.GetSafeNormal() * GetGravityZ() * InclineFraction * DeltaTime;
				CurrentVelocity += GravityAlongSlope;
				Velocity = CurrentVelocity;
				//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, GravityAlongSlope.ToString());
			}
			PreviousGroundNormal = GroundNormal;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, Velocity.ToString());
	}
	VelocityPriorToJumping = Velocity;
}

void UJonyWalk_5_7MovementComponent::DoRailMovement(float deltaTime, FVector2D MovementVector)
{
	//TODO: Implement if time allows
	VelocityPriorToJumping = Velocity;
}
