// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameProp/ReplicatedCollisionActor.h"
#include "CTFEnum.h"

#include "FlagActor.generated.h"

class USphereComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogFlag, Log, All);

UCLASS()
class CTF_API AFlagActor : public AReplicatedCollisionActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlagActor();

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


protected:
	void OnActorOverlapBeginWithAuth(AActor* OtherActor) override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* StaticMeshComponent;

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Team)
	TEnumAsByte<ETeamID> TeamID;
};
