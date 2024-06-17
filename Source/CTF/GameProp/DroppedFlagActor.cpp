// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProp/DroppedFlagActor.h"

#include "Character/BaseCharacter.h"

// Sets default values
ADroppedFlagActor::ADroppedFlagActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlagMeshComponent1->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	FlagMeshComponent2->SetRelativeLocation(FVector(0.f, 0.f, -20.f));

	TeamMeshComponents.AddUnique(FlagMeshComponent2);
	ApplyTeamMaterial();
}

// Called when the game starts or when spawned
void ADroppedFlagActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADroppedFlagActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADroppedFlagActor::OnActorOverlapBeginWithAuth(AActor* OtherActor)
{
	if (ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor))
	{
		//UE_LOG(LogFlag, Log, TEXT("%s Entered!"), *OtherActor->GetName());
		if (!Player->IsDead() && Player->GetTeamID() != TeamID)
		{
			Player->TakeFlag(GetTeamID());
			Destroy();
		}
	}
}

