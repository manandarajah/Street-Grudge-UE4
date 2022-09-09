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

	bool _isMoving = false;

	AStreetGrudgeCharacter* _SGC;

public:

	UPROPERTY(EditAnywhere, Category = "Enemy")
	UAnimMontage* FaceHit;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	UAnimMontage* SideFaceHit;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	UAnimMontage* RibHit;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category="Enemy")
	bool IsMoving();

	UFUNCTION()
	void ChangeState(APawn* SeenPawn);

	void SetSGC(AStreetGrudgeCharacter* SGC);

	UFUNCTION(BlueprintCallable, Category="Enemy")
	void ApplyHit(int Index);
};
