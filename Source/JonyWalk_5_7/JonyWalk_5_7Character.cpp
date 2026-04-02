// Copyright Epic Games, Inc. All Rights Reserved.

#include "JonyWalk_5_7Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "JonyWalk_5_7.h"

AJonyWalk_5_7Character::AJonyWalk_5_7Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AJonyWalk_5_7Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJonyWalk_5_7Character::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AJonyWalk_5_7Character::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJonyWalk_5_7Character::Look);
	}
	else
	{
		UE_LOG(LogJonyWalk_5_7, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AJonyWalk_5_7Character::Tick(float DeltaTime)
{
	if (GetController() != nullptr)
	{
		//Rotation
		if (!GetCharacterMovement()->Velocity.IsNearlyZero() && MovementVector.X > 0.5 || MovementVector.X < -0.5)
		{
			FRotator CurrentRotation = GetActorRotation();
			FRotator TargetRotation = (GetActorRightVector() * MovementVector.X).Rotation(); // Your target based on input

			// Smoothly interpolate
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 1.0f);
			SetActorRotation(NewRotation);

			FVector2D Forward2D(GetActorForwardVector());
			float currentVelocityZ = GetCharacterMovement()->Velocity.Z;
			FVector2D Velocity2D(GetCharacterMovement()->Velocity);
			float Speed2D = Velocity2D.Size();
			GetCharacterMovement()->Velocity = FVector((Forward2D*Speed2D).X, (Forward2D * Speed2D).Y, currentVelocityZ);
		}

		//Acceleration
		if (MovementVector.Y > 0.5)
		{
			// find out which way is forward
			const FRotator Rotation = GetActorRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement
			AddMovementInput(ForwardDirection, MovementVector.Y);
		}

		//Slope
		if (GetCharacterMovement()->IsMovingOnGround()) {
			// Access the current floor result directly
			FFindFloorResult CurrentFloor = GetCharacterMovement()->CurrentFloor;

			// Get the surface normal (ground normal)
			FVector GroundNormal = CurrentFloor.HitResult.ImpactNormal;

			//TODO: Calculate backward vector on ground plane and apply dot product gravity along it. 
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, CurrentFloor.HitResult.ImpactNormal.ToString());
		}

		//Deceleration (Drag and Break)
		FVector currentVelocity = GetCharacterMovement()->Velocity;
		float speedReduction = Drag + (MovementVector.Y < -0.5 ? Break : 0.0f);
		currentVelocity -= currentVelocity.GetSafeNormal() * speedReduction * DeltaTime;
		GetCharacterMovement()->Velocity = currentVelocity.GetClampedToSize(0.0f, GetCharacterMovement()->MaxWalkSpeed);

		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, GetCharacterMovement()->Velocity.ToString());
		MovementVector = FVector2D::ZeroVector;
	}
}

void AJonyWalk_5_7Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();
}

void AJonyWalk_5_7Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AJonyWalk_5_7Character::DoMove(float Right, float Forward)
{

}

void AJonyWalk_5_7Character::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AJonyWalk_5_7Character::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AJonyWalk_5_7Character::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
