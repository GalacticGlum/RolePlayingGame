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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MinimumTurnAngle = -40.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaximumTurnAngle = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	/** The controlled pawn as a ARolePlayingGameCharacter */
	class ARolePlayingGameCharacter* ControlledCharacter;

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

	/** Called for forwards/backward input */
	void MoveForward(float value);

	/** Called for side to side input */
	void MoveRight(float value);

	UClass* m_GoalDecalBlueprint;
	AActor* m_CurrentGoalDecal;

	bool m_CanTurn;
	bool m_MoveToMousePosition;
};
