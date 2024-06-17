// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameProp/FlagActor.h"
#include "DroppedFlagActor.generated.h"

UCLASS()
class CTF_API ADroppedFlagActor : public AFlagActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroppedFlagActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	void OnActorOverlapBeginWithAuth(AActor* OtherActor) override;
};
