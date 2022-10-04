// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAbilityInterface.h"

// Add default functionality here for any ICombatAbilityInterface functions that are not pure virtual.
void ICombatAbilityInterface::ApplyHit(ACharacter* Char) {}

void ICombatAbilityInterface::Punch() {}

void ICombatAbilityInterface::PunchCombo() {}

//End the punch combos at anypoint depending on player input
void ICombatAbilityInterface::EndPunch() {}

//Checks weather player is attacking or not, rendering it unable to move if true
bool ICombatAbilityInterface::IsPunching() { return false; }

int ICombatAbilityInterface::GetPunchIndex() { return 0;  }

//Called when player is done attacking, called through the end of each attack animation
void ICombatAbilityInterface::StopPunch() {}