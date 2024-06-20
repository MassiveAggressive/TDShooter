// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSWeapon.h"

// Sets default values
ATDSWeapon::ATDSWeapon()
	: IsAutoFire(false), Damage(5.f), FiringRate(2.f), MaxLoadedAmmo(24.f), ShotCount(1.f), ShotSpread(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ATDSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATDSWeapon::Equip()
{
	OnEquip();
}

void ATDSWeapon::UnEquip()
{
	OnUnEquip();
}