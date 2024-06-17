// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProp/TeamActor.h"

// Sets default values
ATeamActor::ATeamActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATeamActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeamActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

