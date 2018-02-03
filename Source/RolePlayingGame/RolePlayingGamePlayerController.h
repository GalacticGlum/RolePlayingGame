// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RolePlayingGamePlayerController.generated.h"

UCLASS()
class ROLEPLAYINGGAME_API ARolePlayingGamePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ARolePlayingGamePlayerController();

	/** The minimum angle that the player can turn to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumTurnAngle = -60.0f;

	/** The maximum angle that the player can turn to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaximumTurnAngle = 20.0f;

	/** The minimum distance between the camera and the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumCameraDistance = 150.0f;

	/** The minimum distance between the camera and the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaximumCameraDistance = 650.0f;

	/** The sensitivity of the zoom */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZoomSensitivity = 20.0f;

	/** The controlled pawn as a ARolePlayingGameCharacter */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ARolePlayingGameCharacter* ControlledCharacter;

	/** Cancels the current movement command */
	UFUNCTION(BlueprintCallable)
	void CancelMovementCommand();
protected:
	virtual void BeginPlay();

private:
	virtual void SetupInputComponent() override;

	FORCEINLINE void EnableTurning() { m_CanTurn = true; }
	FORCEINLINE void DisableTurning() { m_CanTurn = false; }

	void Jump();
	void StopJumping();
	void LeftClick();

	void Lookup(float value);
	void Turn(float value);
	void Zoom(float value);

	/** Called for forwards/backward input */
	void MoveForward(float value);

	/** Called for side to side input */
	void MoveRight(float value);

	UClass* m_GoalDecalBlueprint;
	AActor* m_CurrentGoalDecal;

	bool m_CanTurn;
	bool m_MoveToMousePosition;
};
