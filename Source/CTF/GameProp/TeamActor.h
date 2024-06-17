// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameProp/ReplicatedCollisionActor.h"
#include "CTFEnum.h"

#include "TeamActor.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;

UCLASS()
class CTF_API ATeamActor : public AReplicatedCollisionActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeamActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintPure, Category = Team)
	FORCEINLINE ETeamID GetTeamID() const { return TeamID; }

	UFUNCTION(BlueprintCallable, Category = Team)
	void SetTeamID(ETeamID ID);

	UFUNCTION(BlueprintCallable, Category = "Team|Mesh")
	void ApplyTeamMaterial();

protected:
	UFUNCTION()
	void OnRep_TeamID();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Team|Material")
	UMaterialInterface* TeamAMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Team|Material")
	UMaterialInterface* TeamBMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team|Mesh")
	TArray<UStaticMeshComponent*> TeamMeshComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_TeamID, Category = Team)
	TEnumAsByte<ETeamID> TeamID;
};
