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

	Internal_Move();
}

void ASGEnemy::Internal_SetAIConfig() {
	_AICont = GetWorld()->SpawnActor<AAIController>();
	_AICont->Possess(this);
}

void ASGEnemy::Internal_Move() {
	if (_EnemyState == SGAIState::Alert) {
		_IsMoving = true;

		_AICont->MoveToActor(_Target, 20);

		if (_AICont->GetMoveStatus() == EPathFollowingStatus::Idle) _EnemyState = SGAIState::Idle;

		//UE_LOG(LogTemp, Log, TEXT("_Target: %s"), *_Target->GetName());
	}

	else
		_IsMoving = false;
}

// Called to bind functionality to input
void ASGEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ASGEnemy::IsMoving() {
	return _IsMoving;
}

void ASGEnemy::ChangeState(APawn* SeenPawn) {
	_Target = SeenPawn;
	_EnemyState = SGAIState::Alert;
}

void ASGEnemy::ApplyHit(ACharacter* Char) {

	if (Char->GetName().Contains("ThirdPersonCharacter")) {
		AStreetGrudgeCharacter* SGC = dynamic_cast<AStreetGrudgeCharacter*>(Char);

		if (SGC && this->_IsInPlayerRange) {

			if (_Index >= 0) EndPunch();
			if (_CanPunch) StopPunch();

			if (SGC->IsJumping()) PlayAnimMontage(AerialHit);
			else {
				switch (SGC->GetPunchIndex()) {
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
	}
}

void ASGEnemy::SetInPlayerRange(bool InPlayerRange) {
	_IsInPlayerRange = InPlayerRange;
}

bool ASGEnemy::IsInPlayerRange() {
	return _IsInPlayerRange;
}

void ASGEnemy::Punch() {

	if (_IsInPlayerRange) {
		//UE_LOG(LogTemp, Log, TEXT("Something is wrong here, Index: %d"), _Index);
		_CanPunch = true;

		if (_Index == -1) PunchCombo();
	}
}

void ASGEnemy::PunchCombo() {
	++_Index;

	if (Jab == nullptr || Cross == nullptr || Knee == nullptr) return;

	switch (_Index) {
	case 0:
		PlayAnimMontage(Jab, 1.3f);
		break;
	case 1:
		PlayAnimMontage(Cross, 1.3f);
		break;
	case 2:
		PlayAnimMontage(Knee, 1.3f);
		break;
	}
}

//End the punch combos at anypoint depending on player input
void ASGEnemy::EndPunch() {
	_Index = -1;
}

//Checks weather player is attacking or not, rendering it unable to move if true
bool ASGEnemy::IsPunching() {
	return _CanPunch;
}

int ASGEnemy::GetPunchIndex() {
	return _Index;
}

//Called when player is done attacking, called through the end of each attack animation
void ASGEnemy::StopPunch() {
	_CanPunch = false;
}