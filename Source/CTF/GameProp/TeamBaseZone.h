// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CTFEnum.h"
#include "GameProp/ReplicatedCollisionActor.h"

#include "TeamBaseZone.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBaseZone, Log, All);

UCLASS()
class CTF_API ATeamBaseZone : public AReplicatedCollisionActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeamBaseZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UFUNCTION(BlueprintPure, Category = Team)
	FORCEINLINE ETeamID GetTeamID() const { return TeamID; };

	UFUNCTION(BlueprintCallable, Category = Flag)
	void AddTeamScore();

protected:
	void OnActorOverlapBeginWithAuth(AActor* OtherActor) override;


public:
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
	TEnumAsByte<ETeamID> TeamID;
};
