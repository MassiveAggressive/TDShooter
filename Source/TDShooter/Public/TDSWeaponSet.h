// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDSBaseSet.h"
#include "AbilitySystemComponent.h"
#include "TDSWeaponSet.generated.h"

UCLASS()
class TDSHOOTER_API UTDSWeaponSet : public UTDSBaseSet
{
	GENERATED_BODY()

public:
	UTDSWeaponSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UTDSWeaponSet, Damage);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData FiringRate;
	ATTRIBUTE_ACCESSORS(UTDSWeaponSet, FiringRate);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
    FGameplayAttributeData LoadedAmmo;
    ATTRIBUTE_ACCESSORS(UTDSWeaponSet, LoadedAmmo);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
    FGameplayAttributeData MaxLoadedAmmo;
    ATTRIBUTE_ACCESSORS(UTDSWeaponSet, MaxLoadedAmmo);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData CarriedAmmo;
	ATTRIBUTE_ACCESSORS(UTDSWeaponSet, CarriedAmmo);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxCarriedAmmo;
	ATTRIBUTE_ACCESSORS(UTDSWeaponSet, MaxCarriedAmmo);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ShotCount;
	ATTRIBUTE_ACCESSORS(UTDSWeaponSet, ShotCount);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ShotSpread;
	ATTRIBUTE_ACCESSORS(UTDSWeaponSet, ShotSpread);
};
