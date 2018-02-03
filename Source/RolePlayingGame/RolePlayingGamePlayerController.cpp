// Fill out your copyright notice in the Description page of Project Settings.

#include "RolePlayingGamePlayerController.h"
#include "RolePlayingGameCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

ARolePlayingGamePlayerController::ARolePlayingGamePlayerController()
{
	// Load the goal decal
	static ConstructorHelpers::FObjectFinder<UClass> goalDecalObjectFinder(TEXT("/Game/Blueprints/BP_Goal_Decal.BP_Goal_Decal_C"));
	if (goalDecalObjectFinder.Object)
	{
		m_GoalDecalBlueprint = goalDecalObjectFinder.Object;
	}
}

void ARolePlayingGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	ControlledCharacter = Cast<ARolePlayingGameCharacter>(GetPawn());

	EnableInput(this);
}

void ARolePlayingGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	InputComponent->BindAction("Turning", IE_Pressed, this, &ARolePlayingGamePlayerController::EnableTurning);
	InputComponent->BindAction("Turning", IE_Released, this, &ARolePlayingGamePlayerController::DisableTurning);
	InputComponent->BindAction("Left Click", IE_Pressed, this, &ARolePlayingGamePlayerController::LeftClick);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ARolePlayingGamePlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ARolePlayingGamePlayerController::StopJumping);

	InputComponent->BindAxis("Lookup", this, &ARolePlayingGamePlayerController::Lookup);
	InputComponent->BindAxis("Turn", this, &ARolePlayingGamePlayerController::Turn);
	InputComponent->BindAxis("Zoom", this, &ARolePlayingGamePlayerController::Zoom);

	InputComponent->BindAxis("MoveForward", this, &ARolePlayingGamePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARolePlayingGamePlayerController::MoveRight);
}

void ARolePlayingGamePlayerController::Lookup(float value)
{
	// If we can't turn or the axis value is zero then we can bail from this function.
	if (!m_CanTurn || value == 0) return;

	FRotator newRotation = ControlledCharacter->GetCameraBoom()->RelativeRotation;
	newRotation.Pitch = FMath::Clamp(newRotation.Pitch + value * 1.5f, MinimumTurnAngle, MaximumTurnAngle);

	ControlledCharacter->GetCameraBoom()->SetRelativeRotation(newRotation);
}

void ARolePlayingGamePlayerController::Turn(float value)
{
	// If we can't turn or the axis value is zero then we can bail from this function.
	if (!m_CanTurn || value == 0) return;

	FRotator newRotation = ControlledCharacter->GetCameraBoom()->RelativeRotation;
	newRotation.Yaw += value * 2;

	ControlledCharacter->GetCameraBoom()->SetRelativeRotation(newRotation);
}

void ARolePlayingGamePlayerController::Zoom(float value)
{
	if (ControlledCharacter == nullptr) return;

	USpringArmComponent* cameraBoomComponent = ControlledCharacter->GetCameraBoom();
	cameraBoomComponent->TargetArmLength = FMath::Clamp(cameraBoomComponent->TargetArmLength - value * ZoomSensitivity, MinimumCameraDistance, MaximumCameraDistance);
}

void ARolePlayingGamePlayerController::LeftClick()
{
	// If our player is moving and we don't have a move command initiated, we don't want to initiate a new move command.
	if (ControlledCharacter->GetVelocity().Size() != 0 && !m_MoveToMousePosition) return;

	// If we have already initiated a movement command, we need to cancel it.
	CancelMovementCommand();

	FHitResult result;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, result);

	UWorld* world = GetWorld();

	FVector distance = ControlledCharacter->GetActorLocation() - result.Location;
	float angle = FMath::RadiansToDegrees(FMath::Atan2(distance.Y, distance.X));
	m_CurrentGoalDecal = world->SpawnActor<AActor>(m_GoalDecalBlueprint, result.Location, FRotator(0, angle - 90, 0));

	m_MoveToMousePosition = true;
	world->GetNavigationSystem()->SimpleMoveToLocation(this, result.Location);
}

void ARolePlayingGamePlayerController::CancelMovementCommand()
{
	if(IsValid(m_CurrentGoalDecal))
	{
		StopMovement();

		GetWorld()->DestroyActor(m_CurrentGoalDecal);
		m_CurrentGoalDecal = nullptr;
		m_MoveToMousePosition = false;
	}
}

void ARolePlayingGamePlayerController::MoveForward(float value)
{
	if ((ControlledCharacter != NULL) && (value != 0.0f))
	{
		if(m_MoveToMousePosition)
		{
			CancelMovementCommand();
		}

		const FRotator yawRotation(0, ControlledCharacter->GetCameraBoom()->RelativeRotation.Yaw, 0);

		// Get forward vector
		const FVector direction  = UKismetMathLibrary::GetForwardVector(yawRotation);

		// Add movement in that direction
		ControlledCharacter->AddMovementInput(direction, value);
	}
}

void ARolePlayingGamePlayerController::MoveRight(float value)
{
	if ((ControlledCharacter != NULL) && (value != 0.0f))
	{
		if (m_MoveToMousePosition)
		{
			CancelMovementCommand();
		}

		const FRotator yawRotation(0, ControlledCharacter->GetCameraBoom()->RelativeRotation.Yaw, 0);

		// Get right vector 
		const FVector direction = UKismetMathLibrary::GetRightVector(yawRotation);

		// Add movement in that direction
		ControlledCharacter->AddMovementInput(direction, value);
	}
}

void ARolePlayingGamePlayerController::Jump()
{
	if (m_MoveToMousePosition)
	{
		CancelMovementCommand();
	}

	ControlledCharacter->Jump();
}

void ARolePlayingGamePlayerController::StopJumping()
{
	ControlledCharacter->StopJumping();
}