// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharInfoComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCharInfoComponent::UCharInfoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bIsDead = false;

	bIsHoldingFlag = false;
	HoldingFlagTeamID = ETeamID::NoTeam;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UCharInfoComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharInfoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UCharInfoComponent::TakeDamage(float DamageTaken)
{
	float HealthChanged = CurrentHealth - DamageTaken;
	SetCurrentHealth(HealthChanged);

	return DamageTaken;
}

void UCharInfoComponent::SetCurrentHealth(float HealthValue)
{
	if (GetOwnerRole() == ROLE_Authority && !bIsDead)
	{
		CurrentHealth = FMath::Clamp(HealthValue, 0.f, MaxHealth);
		if (CurrentHealth <= 0.0f)
		{
			bIsDead = true;
			OnRep_IsDead();
		}

		OnRep_CurrentHealth();
	}
}

void UCharInfoComponent::SetHoldingFlag(bool bIsHolding, ETeamID TeamID)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		bIsHoldingFlag = bIsHolding;
		HoldingFlagTeamID = TeamID;

		OnRep_HoldingFlag();
	}
}

void UCharInfoComponent::OnRep_CurrentHealth()
{
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UCharInfoComponent::OnRep_IsDead()
{
	if (bIsDead)
	{
		OnDeath.Broadcast();
	}
}

void UCharInfoComponent::OnRep_HoldingFlag()
{
	OnHoldingFlag.Broadcast(bIsHoldingFlag);
}

void UCharInfoComponent::OnRep_BaseZoneEntered()
{

}

void UCharInfoComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharInfoComponent, CurrentHealth);
	DOREPLIFETIME(UCharInfoComponent, bIsDead);
	DOREPLIFETIME(UCharInfoComponent, bIsHoldingFlag);
}
