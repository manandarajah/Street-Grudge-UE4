// Fill out your copyright notice in the Description page of Project Settings.


#include "SGEnemy.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "Animation/AnimMontage.h"
#include "StreetGrudgeCharacter.h"
#include "Engine/Engine.h"

// Sets default values
ASGEnemy::ASGEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASGEnemy::ChangeState);

	_EnemyState = SGAIState::Idle;
}

// Called when the game starts or when spawned
void ASGEnemy::BeginPlay()
{
	Super::BeginPlay();

	Internal_SetAIConfig();
}

// Called every frame
void ASGEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Internal_MoveEnemy();
}

void ASGEnemy::Internal_SetAIConfig() {
	_AICont = GetWorld()->SpawnActor<AAIController>();
	_AICont->Possess(this);
}

void ASGEnemy::Internal_MoveEnemy() {
	if (_EnemyState == SGAIState::Alert) {
		_isMoving = true;

		_AICont->MoveToActor(_Target, 130);

		if (_AICont->GetMoveStatus() == EPathFollowingStatus::Idle) _EnemyState = SGAIState::Idle;

		UE_LOG(LogTemp, Log, TEXT("_Target: %s"), *_Target->GetName());
	}

	else
		_isMoving = false;
}

// Called to bind functionality to input
void ASGEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ASGEnemy::IsMoving() {
	return _isMoving;
}

void ASGEnemy::ChangeState(APawn* SeenPawn) {
	_Target = SeenPawn;
	_EnemyState = SGAIState::Alert;
}

void ASGEnemy::ApplyHit(int Index) {
	
	if (_SGC->IsEnemyInRange()) {

		switch (Index) {
		case 2:
			PlayAnimMontage(SideFaceHit);
			break;
		case 3:
			PlayAnimMontage(RibHit);
			break;
		default:
			PlayAnimMontage(FaceHit);
		}
	}
}

void ASGEnemy::SetSGC(AStreetGrudgeCharacter* SGC) {
	_SGC = SGC;
}