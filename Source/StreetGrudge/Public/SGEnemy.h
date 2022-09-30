// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGEnemy.generated.h"

class UPawnSensingComponent;
class AAIController;
class UAnimMontage;
class AStreetGrudgeCharacter;

UENUM(BlueprintType)
enum class SGAIState : uint8 {
	Idle,
	Alert
};

UCLASS()
class STREETGRUDGE_API ASGEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASGEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

private:
	SGAIState _EnemyState;

	APawn* _Target;

	AAIController* _AICont;

	bool _IsMoving = false, _IsInPlayerRange = false, _CanPunch = false;

	int _Index = -1;

	//Set AI component configuration
	void Internal_SetAIConfig();

	//Handles enemy movement
	void Internal_Move();

public:

	UPROPERTY(VisibleAnywhere, Category = "HitReact")
	UAnimMontage* FaceHit;

	UPROPERTY(VisibleAnywhere, Category = "HitReact")
	UAnimMontage* SideFaceHit;

	UPROPERTY(VisibleAnywhere, Category = "HitReact")
	UAnimMontage* RibHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HitReact")
	UAnimMontage* AerialHit;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	UAnimMontage* Jab;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	UAnimMontage* Cross;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	UAnimMontage* Knee;

	//Checks weather player is attacking or not, rendering it unable to move if true
	UFUNCTION(BlueprintPure, Category = Ability)
	bool IsPunching();

	//End the punch combos at anypoint depending on player input
	UFUNCTION(BlueprintCallable, Category = Ability)
	void EndPunch();

	UFUNCTION(BlueprintCallable, Category = Ability)
	void Punch();

	//Called when player presses the punch button, handling the punch combo system. Currently evoked in blueprints
	UFUNCTION(BlueprintCallable, Category = Ability)
	void PunchCombo();

	//Called when player is done attacking, called through the end of each attack animation
	UFUNCTION(BlueprintCallable, Category = Ability)
	void StopPunch();

	UFUNCTION(BlueprintPure, Category = Ability)
	int GetPunchIndex();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category="Enemy")
	bool IsMoving();

	UFUNCTION()
	void ChangeState(APawn* SeenPawn);

	void SetInPlayerRange(bool InPlayerRange);

	UFUNCTION(BlueprintCallable, Category="Enemy")
	void ApplyHit(int Index, bool IsInAir);

};
