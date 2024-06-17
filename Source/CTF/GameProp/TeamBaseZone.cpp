// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProp/TeamBaseZone.h"

#include "Components/SphereComponent.h"

#include "Character/BaseCharacter.h"
#include "GameProp/FlagActor.h"
#include "GameMode/CTFGameMode.h"

DEFINE_LOG_CATEGORY(LogBaseZone);

// Sets default values
ATeamBaseZone::ATeamBaseZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent->InitSphereRadius(400.0f);
    
	TeamID = ETeamID::NoTeam;
}

// Called when the game starts or when spawned
void ATeamBaseZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeamBaseZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeamBaseZone::OnActorOverlapBeginWithAuth(AActor* OtherActor)
{
    if (ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor))
    {
        //UE_LOG(LogBaseZone, Log, TEXT("%s Entered!"), *OtherActor->GetName());
        if (!Player->IsDead() && Player->GetTeamID() == TeamID)
        {
            UE_LOG(LogBaseZone, Log, TEXT("Flag Dropped From %s!"), *OtherActor->GetName());
            Player->DropFlagInBaseZone();

            AddTeamScore();
        }
    }
}

void ATeamBaseZone::AddTeamScore()
{
    if (UWorld* World = GetWorld())
    {
        if (ACTFGameMode* GameMode = Cast<ACTFGameMode>(World->GetAuthGameMode()))
        {
            GameMode->AddTeamScore(TeamID);
        }
    }
}