// Fill out your copyright notice in the Description page of Project Settings.


#include "PunchingBag.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"

// Sets default values
APunchingBag::APunchingBag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Comp"));
	CapsuleComp->InitCapsuleSize(42.f, 96.0f);
	CapsuleComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void APunchingBag::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &APunchingBag::HandleTakeDamage);
}

// Called every frame
void APunchingBag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0, FColor::Green, CollideActorName != "" ? CollideActorName : "No collisions");
}

void APunchingBag::HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {

	CollideActorName = DamageCauser->GetName();
}