// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StreetGrudgeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"
//#include "Animation/AnimInstance.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Public/HandComponent.h"
#include "Public/SGEnemy.h"

// AStreetGrudgeCharacter

AStreetGrudgeCharacter::AStreetGrudgeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	LeftSide = CreateDefaultSubobject<USceneComponent>(TEXT("Left Side"));
	LeftSide->SetupAttachment(RootComponent);

	RightSide = CreateDefaultSubobject<USceneComponent>(TEXT("Right Side"));
	RightSide->SetupAttachment(RootComponent);

	//LeftHandCollision = CreateDefaultSubobject<UHandComponent>(TEXT("Left Hand Collision"));
	////LeftHandCollision->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetIncludingScale, "LeftHand");
	//LeftHandCollision->SetName("LeftHand");
	//LeftHandCollision->SetupAttachment(RootComponent);

	//RightHandCollision = CreateDefaultSubobject<UHandComponent>(TEXT("Right Hand Collision"));
	////RightHandCollision->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetIncludingScale, "RightHand");
	//RightHandCollision->SetName("RightHand");
	//RightHandCollision->SetupAttachment(RootComponent);

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box Component"));
	HitBox->SetupAttachment(RootComponent);

	//OnActorBeginOverlap.AddDynamic(this, &AStreetGrudgeCharacter::PunchHit);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStreetGrudgeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AStreetGrudgeCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStreetGrudgeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStreetGrudgeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AStreetGrudgeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AStreetGrudgeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AStreetGrudgeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AStreetGrudgeCharacter::TouchStopped);

	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AStreetGrudgeCharacter::Punch);
	//PlayerInputComponent->BindAction("Punch", IE_Released, this, &AStreetGrudgeCharacter::StopPunch);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AStreetGrudgeCharacter::OnResetVR);
}


void AStreetGrudgeCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AStreetGrudgeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AStreetGrudgeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AStreetGrudgeCharacter::BeginPlay() {
	Super::BeginPlay();

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	/*LeftHandCollision->AttachToComponent(GetMesh(), AttachmentRules, "LeftHandSocket");
	RightHandCollision->AttachToComponent(GetMesh(), AttachmentRules, "RightHandSocket");*/

	/*LeftHandCollision->OnHandCollide.AddDynamic(this, &AStreetGrudgeCharacter::PunchHit);
	RightHandCollision->OnHandCollide.AddDynamic(this, &AStreetGrudgeCharacter::PunchHit);*/

	OnActorBeginOverlap.AddDynamic(this, &AStreetGrudgeCharacter::EnemyInRange);
}

void AStreetGrudgeCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	FHitResult Hit;
	int RaycastLength = 100;
	FVector Start = RightSide->GetComponentLocation();

	//Gets raycast trace from a player's right side, determining if right side is colliding with wall or not
	RightSideCollide = GetWorld()->LineTraceSingleByChannel(Hit, Start, Start + (GetActorRightVector() * RaycastLength), 
		ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams::DefaultQueryParam);
	
	//DrawDebugLine(GetWorld(), Start, Start + (GetActorRightVector() * RaycastLength), FColor::Green, false, 1, 0, 5);
	
	Start = LeftSide->GetComponentLocation();

	//Gets raycast trace from a player's left side, determining if right side is colliding with wall or not
	LeftSideCollide = GetWorld()->LineTraceSingleByChannel(Hit, Start, Start + (GetActorRightVector() * -RaycastLength), 
		ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams::DefaultQueryParam);
	
	//DrawDebugLine(GetWorld(), Start, Start + (GetActorRightVector() * -RaycastLength), FColor::Red, false, 1, 0, 5);

	/*Resets collision configurations if collision raycast is pointing to main character, else if right side it colliding with a wall, then left side 
		automatically defaults to false*/
	if (Hit.GetActor() != NULL && Hit.GetActor()->GetName().Contains("Character")) {
		LeftSideCollide = false;
		RightSideCollide = false;
	}
	else if (RightSideCollide) LeftSideCollide = false;
	else if (LeftSideCollide) RightSideCollide = false;

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0, FColor::Blue, FString::Printf(TEXT("Right Side Collide: %d\nLeft Side Collide: %d\nJump Velocity: %d"), RightSideCollide,
		LeftSideCollide, _JumpVelocity));
}

//Calculates wall jump direction based on character rotation and left/right collision detection
void AStreetGrudgeCharacter::Internal_GetJumpDirection(FVector &Vector) {

	//Gets character Yaw value and calculates all 4 player direction ranges
	float Yaw = GetActorRotation().Yaw, HalfRotation = (DirectionForwardVal + 180) - DirectionRangeVal,
		RForwardDirectionRange = DirectionForwardVal + DirectionRangeVal, LForwardDirectionRange = DirectionForwardVal - DirectionRangeVal,
		RRightDirectionRange = DirectionRightVal + DirectionRangeVal, LRightDirectionRange = DirectionRightVal - DirectionRangeVal;
	
	//Runs comparison to determine character's current direction, player side wall collision, and set velocity to respective axis
	if ((LeftSideCollide && Yaw > LForwardDirectionRange && Yaw < RForwardDirectionRange)
		|| (RightSideCollide && (Yaw > HalfRotation || Yaw < -HalfRotation))) {

		Vector.Y = JumpBounceValue;
	}

	else if ((RightSideCollide && Yaw > LForwardDirectionRange && Yaw < RForwardDirectionRange)
		|| (LeftSideCollide && (Yaw > HalfRotation || Yaw < -HalfRotation))) {

		Vector.Y = JumpBounceValue * -1;
	}

	else if ((RightSideCollide && Yaw > -RRightDirectionRange && Yaw < -LRightDirectionRange)
		|| (LeftSideCollide && Yaw > LRightDirectionRange && Yaw < RRightDirectionRange)) {

		Vector.X = JumpBounceValue * -1;
	}

	else if ((RightSideCollide && Yaw > LRightDirectionRange && Yaw < RRightDirectionRange)
		|| (LeftSideCollide && Yaw > -RRightDirectionRange && Yaw < -LRightDirectionRange)) {

		Vector.X = JumpBounceValue;
	}
}

//Override unreal jump function to simulate normal jump and to allow an interrupt so jump can
void AStreetGrudgeCharacter::Jump() {

	//Determines weather to jump or wall jump
	if (_JumpCount == 0 || (CanWallJump && _JumpCount > 0 && _JumpCount < 2)) {
		FVector ZVelocity(0, 0, GetCharacterMovement()->JumpZVelocity);

		//Checks if character is jumping to simulate a wall jump
		if (_JumpCount == 1) {

			Internal_GetJumpDirection(ZVelocity);

			//if (RightSideCollide) ZVelocity.Y *= -1;

			if (DoubleJumpMontage != nullptr) PlayAnimMontage(DoubleJumpMontage);
		}
		
		LaunchCharacter(ZVelocity, false, true);
		++_JumpCount;
	}
	
}

//Resets jump configurations when player hits the ground
void AStreetGrudgeCharacter::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);

	CanWallJump = false;
	_JumpCount = 0;
}

void AStreetGrudgeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStreetGrudgeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AStreetGrudgeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !_CanPunch)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AStreetGrudgeCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && !_CanPunch)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//Invoked from the punch input button, decides to simulate punch combos or aerial punch
void AStreetGrudgeCharacter::Punch() {
	_CanPunch = true;
	
	if (CanWallJump && _JumpCount > 0 && _JumpCount < 2) {
		FVector ZVelocity(0, 0, 0);

		Internal_GetJumpDirection(ZVelocity);

		if (AirPunchMontage != nullptr) PlayAnimMontage(AirPunchMontage);

		LaunchCharacter(ZVelocity, false, true);
	}

	else if (_Index == -1) PunchCombo();
}

//Called when player presses the punch button, handling the punch combo system in case player button mashes
void AStreetGrudgeCharacter::PunchCombo() {
	++_Index;

	if (JabMontage == nullptr || CrossMontage == nullptr || HookMontage == nullptr || RoundKickMontage == nullptr) return;

	switch (_Index) {
	case 0:
		PlayAnimMontage(JabMontage, 1.3f);
		break;
	case 1:
		PlayAnimMontage(CrossMontage, 1.3f);
		break;
	case 2:
		PlayAnimMontage(HookMontage, 1.3f);
		break;
	case 3:
		PlayAnimMontage(RoundKickMontage, 1.3f);
		break;
	}
}

//Called when player is done attacking, called through the end of each attack animation
void AStreetGrudgeCharacter::StopPunch() {
	_CanPunch = false;
}

void AStreetGrudgeCharacter::EnemyInRange(AActor* OverlappedActor, AActor* OtherActor) {

	UE_LOG(LogTemp, Log, TEXT("Can collide with component: %s  actor: %s can punch: %d object is an enemy: %d"), *HitBox->GetName(), *OtherActor->GetName(), _CanPunch, OtherActor->GetName().Contains("BP_Enemy"));

	if (OtherActor->GetName().Contains("BP_Enemy")) {
		UGameplayStatics::ApplyDamage(OtherActor, 5, OtherActor->GetInstigatorController(), this, nullptr);
		ASGEnemy* SGEnemy = dynamic_cast<ASGEnemy*>(OtherActor);

		if (SGEnemy) SGEnemy->SetSGC(this);
		_IsEnemyInRange = true;
	}

	else
		_IsEnemyInRange = false;
}

bool AStreetGrudgeCharacter::IsEnemyInRange() {
	return _IsEnemyInRange;
}

//End the punch combos at anypoint depending on player input
void AStreetGrudgeCharacter::EndPunch() {
	_Index = -1;
}

//Checks weather player is attacking or not, rendering it unable to move if true
bool AStreetGrudgeCharacter::IsPunching() {
	return _CanPunch;
}

int AStreetGrudgeCharacter::GetPunchIndex() {
	return _Index;
}