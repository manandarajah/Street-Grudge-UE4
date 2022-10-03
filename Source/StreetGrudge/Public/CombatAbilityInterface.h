// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatAbilityInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STREETGRUDGE_API ICombatAbilityInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//Checks weather player is attacking or not, rendering it unable to move if true
	bool IsPunching();

	//End the punch combos at anypoint depending on player input
	void EndPunch();

	void Punch();

	//Called when player presses the punch button, handling the punch combo system. Currently evoked in blueprints
	void PunchCombo();

	//Called when player is done attacking, called through the end of each attack animation
	void StopPunch();

	int GetPunchIndex();
};
