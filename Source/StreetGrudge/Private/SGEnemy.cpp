// Fill out your copyright notice in the Description page of Project Settings.


#include "SGEnemy.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"

// Sets default values
ASGEnemy::ASGEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASGEnemy::ChangeState);

	EnemyState = SGAIState::Idle;
}

// Called when the game starts or when spawned
void ASGEnemy::BeginPlay()
{
	Super::BeginPlay();

	AICont = GetWorld()->SpawnActor<AAIController>();
	AICont->Possess(this);
}

// Called every frame
void ASGEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState == SGAIState::Alert) {
		isMoving = true;
		
		AICont->MoveToActor(Target, 130);
		UE_LOG(LogTemp, Log, TEXT("Target: %s"), *Target->GetName());
	}
}

// Called to bind functionality to input
void ASGEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ASGEnemy::IsMoving() {
	return isMoving;
}

void ASGEnemy::ChangeState(APawn* SeenPawn) {
	Target = SeenPawn;
	EnemyState = SGAIState::Alert;
}