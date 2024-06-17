// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTFEnum.h"
#include "CharInfoComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoldingFlag, bool, bIsHoldingFlag);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CTF_API UCharInfoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharInfoComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintPure, Category = Health)
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = Health)
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = Health)
	FORCEINLINE bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintCallable, Category = Health)
	float TakeDamage(float DamageTaken);

	UFUNCTION(BlueprintCallable, Category = Health)
	void SetCurrentHealth(float HealthValue);

	UFUNCTION(BlueprintPure, Category = Flag)
	FORCEINLINE bool IsHoldingFlag() const { return bIsHoldingFlag; }

	UFUNCTION(BlueprintPure, Category = Flag)
	FORCEINLINE ETeamID GetHoldingFlagTeamID() const { return HoldingFlagTeamID; }

	UFUNCTION(BlueprintCallable, Category = Flag)
	void SetHoldingFlag(bool bIsHolding, ETeamID TeamID);

	//UFUNCTION(BlueprintPure, Category = "Base Zone")
	//FORCEINLINE bool IsBaseZoneEntered() const { return bIsBaseZoneEntered; }

	//UFUNCTION(BlueprintCallable, Category = "Base Zone")
	//void SetBaseZoneEntered(bool bIsEntered);

protected:
	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_IsDead();

	UFUNCTION()
	void OnRep_HoldingFlag();

	UFUNCTION()
	void OnRep_BaseZoneEntered();


public:
	UPROPERTY(BlueprintAssignable, Category = "Health|Event")
	FOnHoldingFlag OnHoldingFlag;

	UPROPERTY(BlueprintAssignable, Category = "Health|Event")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health|Event")
	FOnDeath OnDeath;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentHealth, Category = Health)
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_IsDead, Category = Health)
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Flag)
	TEnumAsByte<ETeamID> HoldingFlagTeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_HoldingFlag, Category = Flag)
	bool bIsHoldingFlag;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_BaseZoneEntered, Category = "Base Zone")
	//bool bIsBaseZoneEntered;
};
