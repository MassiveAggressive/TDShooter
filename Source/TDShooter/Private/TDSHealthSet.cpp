// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSHealthSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UTDSHealthSet::UTDSHealthSet()
	: Health(40.f), MaxHealth(60.f), Shield(0.f), MaxShield(0.f), ShieldRegen(0.f), ShieldRegenDelay(1.f)
{
}

void UTDSHealthSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if(Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxShield());
	}
}

void UTDSHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if(Data.EvaluatedData.Attribute == GetInDamageAttribute())
	{
		float InDamageDone = GetInDamage();
		SetInDamage(0.f);
		
		if(InDamageDone > 0.f)
		{
			if (GetShield() > 0.f)
			{
				float NewShield = GetShield();
				const float ShieldDiff = FMath::Min(NewShield, InDamageDone);
				InDamageDone -= ShieldDiff;
				NewShield -= ShieldDiff;
				SetShield(NewShield);
			}

			if(InDamageDone > 0.f)
			{
				float NewHealth = GetHealth();
				const float HealthDiff = FMath::Min(NewHealth, InDamageDone);
				NewHealth -= HealthDiff;
				SetHealth(NewHealth);
			}

			if(OnDamageTakenEvent.IsBound())
			{
				const FGameplayEffectContextHandle& EffectContextHandle = Data.EffectSpec.GetEffectContext();
				AActor* Instigator = EffectContextHandle.GetInstigator();
				AActor* Causer = EffectContextHandle.GetEffectCauser();

				OnDamageTakenEvent.Broadcast(Instigator, Causer, Data.EffectSpec.CapturedSourceTags.GetSpecTags(), Data.EvaluatedData.Magnitude);
			}
		}
	}
}

void UTDSHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTDSHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDSHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDSHealthSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDSHealthSet, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDSHealthSet, ShieldRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDSHealthSet, ShieldRegenDelay, COND_None, REPNOTIFY_Always);
}

void UTDSHealthSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDSHealthSet, Health, OldHealth);
}

void UTDSHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDSHealthSet, MaxHealth, OldMaxHealth);
}

void UTDSHealthSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
}

void UTDSHealthSet::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield)
{
}

void UTDSHealthSet::OnRep_ShieldRegen(const FGameplayAttributeData& OldShieldRegen)
{
}

void UTDSHealthSet::OnRep_ShieldRegenDelay(const FGameplayAttributeData& OldShieldRegenDelay)
{
}

