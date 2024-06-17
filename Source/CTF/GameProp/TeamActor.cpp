// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProp/TeamActor.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ATeamActor::ATeamActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TeamAMaterialAsset(TEXT("/Game/CTF/Materials/M_TeamA"));
	if (TeamAMaterialAsset.Succeeded())
	{
		TeamAMaterial = TeamAMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TeamBMaterialAsset(TEXT("/Game/CTF/Materials/M_TeamB"));
	if (TeamBMaterialAsset.Succeeded())
	{
		TeamBMaterial = TeamBMaterialAsset.Object;
	}

	TeamID = ETeamID::NoTeam;

	ApplyTeamMaterial();
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

void ATeamActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamActor, TeamID);
}

void ATeamActor::SetTeamID(ETeamID ID)
{
	TeamID = ID;

	OnRep_TeamID();
}

void ATeamActor::ApplyTeamMaterial()
{
	for (UStaticMeshComponent* MeshComponent : TeamMeshComponents)
	{
		if (TeamID == ETeamID::TeamA)
		{
			MeshComponent->SetMaterial(0, TeamAMaterial);
		}
		else if (TeamID == ETeamID::TeamB)
		{
			MeshComponent->SetMaterial(0, TeamBMaterial);
		}
	}
}

void ATeamActor::OnRep_TeamID()
{
	ApplyTeamMaterial();
}

