// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/CombatAbilityInterface.h"
#include "StreetGrudgeCharacter.generated.h"

class UAnimMontage;
class USceneComponent;
class UBoxComponent;
class UHandComponent;

UCLASS(config=Game)
class AStreetGrudgeCharacter : public ACharacter, public ICombatAbilityInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

private:

	bool _CanPunch = false;

	//Used to control the number of jumps a player can make, and alter the punch input to simulate an air punch
	int _JumpCount = 0, _JumpVelocity = -1;

	//Manages player punch combo animations
	int _Index = -1;

	//Calculates wall jump direction based on character rotation and left/right collision detection
	void Internal_GetJumpDirection(FVector &Vector);

	//Checks raycats between the player's left and right side to the wall. If true, it will enable the wall jump & arieal punch options
	void Internal_DetectSideCollision();

	//Set left side/right side collision
	void Internal_SetLeftRightSideRaycast(FHitResult& Hit);

	/*Resets collision configurations if collision raycast is pointing to main character, else if right side it colliding with a wall, then left side
	automatically defaults to false*/
	void Internal_ResetLeftRightSideCollision(FHitResult& Hit);

	//Configs handler for the main character
	void Internal_SetPlayerEventHandlers();

	//Handles the internal task of enemy being in player's range.
	void Internal_EnemyInRangeHandler(AActor* OtherActor, bool IsEnemyInRange);

	//Updates original rotation value to reset character rotation after aerial jump animation is done playing
	void Internal_UpdateRotation();

	//Returns the max jump height of the main character
	float Internal_GetCharacterMaxJumpHeight();

	float OriginalZValue = GetActorLocation().Z;

	FVector HitBoxLocation;

	float HitBoxSize = 32;

public:
	AStreetGrudgeCharacter();

	//Sphere components attached to the hand sockets of player models for punch simulations
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Model, meta = (AllowPrivateAccess = "true"))
	UHandComponent* LeftHandCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Model, meta = (AllowPrivateAccess = "true"))
	UHandComponent* RightHandCollision;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Model, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* HitBox;

	//Empty scene components that are used to easily detect a player's side collision with the wall
	UPROPERTY(VisibleAnywhere, Category = Model, meta = (AllowPrivateAccess = "true"))
	USceneComponent* LeftSide;

	UPROPERTY(VisibleAnywhere, Category = Model, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RightSide;

	//Store in direction values to check if the player is within constraints to perform a wall jump or air punch
	UPROPERTY(EditAnywhere, Category = Ability)
	float DirectionForwardVal;

	UPROPERTY(EditAnywhere, Category = Ability)
	float DirectionRightVal;

	UPROPERTY(EditAnywhere, Category = Ability)
	float DirectionRangeVal;

	//Determines how far a player will jump from the wall
	UPROPERTY(EditAnywhere, Category=Ability)
	float JumpBounceValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Ability)
	FRotator OriginalRotation;

	//Used to alter between original and mirrored animations
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ability)
	UAnimMontage* DoubleJumpMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ability)
	UAnimMontage* AirPunchMontage;

	//Attack animations
	UPROPERTY(VisibleAnywhere, Category = Ability)
	UAnimMontage* JabMontage;

	UPROPERTY(VisibleAnywhere, Category = Ability)
	UAnimMontage* CrossMontage;

	UPROPERTY(VisibleAnywhere, Category = Ability)
	UAnimMontage* HookMontage;

	UPROPERTY(VisibleAnywhere, Category = Ability)
	UAnimMontage* RoundKickMontage;

	UPROPERTY(VisibleAnywhere, Category = "HitReact")
	UAnimMontage* FaceHit;

	UPROPERTY(VisibleAnywhere, Category = "HitReact")
	UAnimMontage* StomachHit;

	//Checks if player is colliding with a wall, and on which side, enabling options for a wall jump or an off wall air punch
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ability)
	bool CanWallJump = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CollisionDetection)
	bool RightSideCollide = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CollisionDetection)
	bool LeftSideCollide = false;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//Invoked from the punch input button, decides to simulate punch combos or aerial punch
	void Punch();

	//Checks weather player is attacking or not, rendering it unable to move if true
	UFUNCTION(BlueprintPure, Category = Ability)
	bool IsPunching();

	//End the punch combos at anypoint depending on player input
	UFUNCTION(BlueprintCallable, Category = Ability)
	void EndPunch();

	//Called when player presses the punch button, handling the punch combo system. Currently evoked in blueprints
	UFUNCTION(BlueprintCallable, Category = Ability)
	void PunchCombo();

	//Called when player is done attacking, called through the end of each attack animation
	UFUNCTION(BlueprintCallable, Category = Ability)
	void StopPunch();

	UFUNCTION(BlueprintPure, Category = Ability)
	int GetPunchIndex();

	//Called when player hands are colliding with actor
	UFUNCTION()
	void EnemyInRange(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EnemyOutOfRange(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void ApplyHit(ACharacter* Char);

	bool IsJumping();

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//Override unreal jump function to simulate normal jump and to allow an interrupt so jump can
	virtual void Jump() override;

	//Resets jump configurations when player hits the ground
	virtual void Landed(const FHitResult& Hit) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

