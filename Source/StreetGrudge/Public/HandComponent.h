// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "HandComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHandCollideSignature, UHandComponent*, CompName, AActor*, OtherActor);

/**
 * 
 */
UCLASS(ClassGroup=(HandComp), meta=(BlueprintSpawnableComponent))
class STREETGRUDGE_API UHandComponent : public USphereComponent
{
	GENERATED_BODY()

public:

	UHandComponent();

private:

	UPROPERTY(VisibleAnywhere, Category = "Component Properties")
	FString name;

public:

	void SetName(FString name);

	FString GetHandComponentName();

	UFUNCTION()
	void HandleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(BlueprintAssignable, Category=Events)
	FOnHandCollideSignature OnHandCollide;

	virtual void BeginPlay() override;
};
