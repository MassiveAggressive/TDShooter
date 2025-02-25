// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAbilityInputID: uint8
{
	None,
	Confirm,
	Cancel,
	PrimaryAbility,
	SecondaryAbility,
	MovementAbility,
	UtilityAbility,
	WeaponFire,
	WeaponAlt,
	WeaponReload
};