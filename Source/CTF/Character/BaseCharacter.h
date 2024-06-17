// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CTFEnum.h"

#include "BaseCharacter.generated.h"

class UCharInfoComponent;
class UPrimitiveComponent;
class USphereComponent;
class ACTFProjectile;
class ADroppedFlagActor;
class ATeamBaseZone;

UCLASS()
class CTF_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UFUNCTION(BlueprintCallable, Category = Health)
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure, Category = Team)
	ETeamID GetTeamID() const;

	UFUNCTION(BlueprintPure, Category = Health)
	FORCEINLINE bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = Flag)
	void TakeFlag(ETeamID TeamID);

	UFUNCTION(BlueprintCallable, Category = Flag)
	void DropFlagInBaseZone();

protected:
	UFUNCTION()
	virtual void OnHealthChanged(float CurrentHealth);

	UFUNCTION()
	virtual void OnDeath();

	UFUNCTION(BlueprintCallable, Category = Death)
	void ToRagdoll();

	UFUNCTION(BlueprintCallable, Category = Death)
	void SpawnDroppedFlag();

	UFUNCTION()
	virtual void OnHoldingFlagChanged(bool bIsHoldingFlag);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ResetCanFire();

	UFUNCTION(Server, Reliable)
	void HandleFire();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Components)
	UCharInfoComponent* CharInfoComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<ACTFProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate;

	FTimerHandle FireRateTimerHandle;

	bool bCanFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flag")
	TSubclassOf<ADroppedFlagActor> DroppedFlagActorClass;
};
