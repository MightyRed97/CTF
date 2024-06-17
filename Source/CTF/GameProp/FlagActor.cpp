// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProp/FlagActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

#include "Character/BaseCharacter.h"

DEFINE_LOG_CATEGORY(LogFlag);

// Sets default values
AFlagActor::AFlagActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent->InitSphereRadius(200.0f);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	TeamID = ETeamID::NoTeam;
}

// Called when the game starts or when spawned
void AFlagActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlagActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlagActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlagActor, TeamID);
}

void AFlagActor::SetTeamID(ETeamID ID)
{
	TeamID = ID;
}

void AFlagActor::OnActorOverlapBeginWithAuth(AActor* OtherActor)
{
	if (ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor))
	{
		//UE_LOG(LogFlag, Log, TEXT("%s Entered!"), *OtherActor->GetName());
		if (!Player->IsDead() && Player->GetTeamID() != TeamID)
		{
			Player->TakeFlag(GetTeamID());
		}
	}
}

