// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDSBaseSet.h"
#include "AbilitySystemComponent.h"
#include "TDSHealthSet.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FTDSOnDamageTakenEvent, AActor*, AActor*, const FGameplayTagContainer&, float)

UCLASS()
class TDSHOOTER_API UTDSHealthSet : public UTDSBaseSet
{
	GENERATED_BODY()

public:
	UTDSHealthSet();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTDSHealthSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTDSHealthSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UTDSHealthSet, Shield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxShield, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UTDSHealthSet, MaxShield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShieldRegen, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ShieldRegen;
	ATTRIBUTE_ACCESSORS(UTDSHealthSet, ShieldRegen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShieldRegenDelay, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ShieldRegenDelay;
	ATTRIBUTE_ACCESSORS(UTDSHealthSet, ShieldRegenDelay);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData InDamage;
	ATTRIBUTE_ACCESSORS(UTDSHealthSet, InDamage);

	mutable FTDSOnDamageTakenEvent OnDamageTakenEvent;
	
protected:
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldShield);
	
	UFUNCTION()
	virtual void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield);

	UFUNCTION()
	virtual void OnRep_ShieldRegen(const FGameplayAttributeData& OldShieldRegen);

	UFUNCTION()
	virtual void OnRep_ShieldRegenDelay(const FGameplayAttributeData& OldShieldRegenDelay);
};
