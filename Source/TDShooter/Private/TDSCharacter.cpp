// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSCharacter.h"
#include "AbilitySystemComponent.h"
#include "TDSPlayerState.h"
#include "EnhancedInputComponent.h"
#include "TDSGameplayAbility.h"
#include "TDSHealthSet.h"
#include "TDSWeapon.h"
#include "..\TDShooter.h"

// Sets default values
ATDSCharacter::ATDSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();

	ATDSPlayerState* PS = GetPlayerState<ATDSPlayerState>();
	if(!PS)
	{
		return;
	}

	UTDSHealthSet* HealthSet = PS->HealthSet;
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetHealthAttribute()).AddUObject(this, &ATDSCharacter::OnHealthAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetShieldAttribute()).AddUObject(this, &ATDSCharacter::OnShieldAttributeChanged);

	UWorld* const World = GetWorld();
	if(World && DefaultWeaponClass)
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParameters.Owner = this;

		ATDSWeapon* DefaultWeapon = World->SpawnActor<ATDSWeapon>(DefaultWeaponClass, GetActorLocation(), GetActorRotation(), ActorSpawnParameters);
		EquipWeapon(DefaultWeapon);
	}
}

void ATDSCharacter::OnPrimaryAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::PrimaryAbility));
}

void ATDSCharacter::OnSecondaryAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::SecondaryAbility));
}

void ATDSCharacter::OnMovementAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::MovementAbility));
}

void ATDSCharacter::OnUtilityAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::UtilityAbility));
}

void ATDSCharacter::OnWeaponFire(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::WeaponFire));
}

void ATDSCharacter::OnWeaponAlt(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::WeaponAlt));
}

void ATDSCharacter::OnWeaponReload(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::WeaponReload));
}

void ATDSCharacter::SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID)
{
	if(!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if(Value.Get<bool>())
	{
		AbilitySystemComponent->AbilityLocalInputPressed(InputID);
	}
	else
	{
		AbilitySystemComponent->AbilityLocalInputReleased(InputID);
	}
}

// Called every frame
void ATDSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATDSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(PrimaryAbilityAction, ETriggerEvent::Triggered, this, &ATDSCharacter::OnPrimaryAbility);
		EnhancedInputComponent->BindAction(SecondaryAbilityAction, ETriggerEvent::Triggered, this, &ATDSCharacter::OnSecondaryAbility);
		EnhancedInputComponent->BindAction(MovementAbilityAction, ETriggerEvent::Triggered, this, &ATDSCharacter::OnMovementAbility);
		EnhancedInputComponent->BindAction(UtilityAbilityAction, ETriggerEvent::Triggered, this, &ATDSCharacter::OnUtilityAbility);
		EnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Triggered, this, &ATDSCharacter::OnWeaponFire);
		EnhancedInputComponent->BindAction(WeaponAltAction, ETriggerEvent::Triggered, this, &ATDSCharacter::OnWeaponAlt);
		EnhancedInputComponent->BindAction(WeaponReloadAction, ETriggerEvent::Triggered, this, &ATDSCharacter::OnWeaponReload);
	}
}

UAbilitySystemComponent* ATDSCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ATDSCharacter::InitAbilitySystemComponent()
{
	ATDSPlayerState* PS = GetPlayerState<ATDSPlayerState>();
	if (!PS)
	{
		return;
	}

	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(PS, this);
}

void ATDSCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystemComponent();

	InitializeEffects();
}

void ATDSCharacter::InitializeAbilities()
{
	if(!HasAuthority() || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	for(TSubclassOf<UTDSGameplayAbility>& Ability : DefaultAbilities)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID)));
		GivenAbilities.Add(SpecHandle);
	}
}

void ATDSCharacter::InitializeEffects()
{
	if(!AbilitySystemComponent.IsValid())
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for(TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if(SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATDSCharacter::ClearGivenAbilities()
{
	if(!HasAuthority() || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	for(FGameplayAbilitySpecHandle AbilitySpecHandle : GivenAbilities)
	{
		AbilitySystemComponent->ClearAbility(AbilitySpecHandle);
	}
}

void ATDSCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

void ATDSCharacter::OnShieldAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnShieldChanged(Data.OldValue, Data.NewValue);
}

void ATDSCharacter::EquipWeapon(ATDSWeapon* NewWeapon)
{
	if(!NewWeapon)
	{
		return;
	}

	if (Weapon)
	{
		Weapon->SetActorLocation(NewWeapon->GetActorLocation());
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->UnEquip();
	}

	Weapon = NewWeapon;
	Weapon->SetOwner(this);
	Weapon->SetActorLocation(GetActorLocation());
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	Weapon->AttachToActor(this, AttachmentRules);
	Weapon->Equip();
}

void ATDSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystemComponent();
	
	InitializeAbilities();
	InitializeEffects();
}
