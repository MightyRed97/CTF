// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProp/FlagActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Character/BaseCharacter.h"

DEFINE_LOG_CATEGORY(LogFlag);

// Sets default values
AFlagActor::AFlagActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent->InitSphereRadius(200.0f);
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TeamAMaterialAsset(TEXT("/Game/CTF/Materials/M_Flag_TeamA"));
	if (TeamAMaterialAsset.Succeeded())
	{
		TeamAMaterial = TeamAMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TeamBMaterialAsset(TEXT("/Game/CTF/Materials/M_Flag_TeamB"));
	if (TeamBMaterialAsset.Succeeded())
	{
		TeamBMaterial = TeamBMaterialAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultFlagMesh1(TEXT("/Game/CTF/Meshes/SM_Flag_Pole.SM_Flag_Pole"));
	FlagMeshComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMeshComponent1"));
	FlagMeshComponent1->SetupAttachment(RootComponent);

	if (DefaultFlagMesh1.Succeeded())
	{
		FlagMeshComponent1->SetStaticMesh(DefaultFlagMesh1.Object);
		FlagMeshComponent1->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
	}
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultFlagMesh2(TEXT("/Game/CTF/Meshes/SM_Flag.SM_Flag"));
	FlagMeshComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMeshComponent2"));
	FlagMeshComponent2->SetupAttachment(RootComponent);

	if (DefaultFlagMesh2.Succeeded())
	{
		FlagMeshComponent2->SetStaticMesh(DefaultFlagMesh2.Object);
		FlagMeshComponent2->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
	}
	
	TeamMeshComponents.AddUnique(FlagMeshComponent2);
	ApplyTeamMaterial();
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

