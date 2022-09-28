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

	bool _IsMoving = false, _IsInPlayerRange = false;

	void Internal_SetAIConfig();
	void Internal_MoveEnemy();

public:

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	UAnimMontage* FaceHit;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	UAnimMontage* SideFaceHit;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	UAnimMontage* RibHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	UAnimMontage* AerialHit;

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
