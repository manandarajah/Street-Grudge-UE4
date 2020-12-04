// Fill out your copyright notice in the Description page of Project Settings.


#include "HandComponent.h"
#include <Components/ActorComponent.h>

UHandComponent::UHandComponent() {
	Super::SphereRadius = 5;
}

void UHandComponent::BeginPlay() {
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UHandComponent::HandleComponentBeginOverlap);
}

void UHandComponent::SetName(FString name) {
	this->name = name;
}

FString UHandComponent::GetHandComponentName() {
	return name;
}

void UHandComponent::HandleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	
	if (OtherActor != nullptr) {
		OnHandCollide.Broadcast(this, OtherActor);
	}
}