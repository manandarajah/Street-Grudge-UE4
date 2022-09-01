// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGEnemy.generated.h"

class UPawnSensingComponent;
class AAIController;

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

	SGAIState EnemyState;

	APawn* Target;

	AAIController* AICont;

	bool isMoving = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category="Enemy")
	bool IsMoving();

	UFUNCTION()
	void ChangeState(APawn* SeenPawn);
};
