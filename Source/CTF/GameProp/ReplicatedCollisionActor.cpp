// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProp/ReplicatedCollisionActor.h"

#include "Components/SphereComponent.h"

// Sets default values
AReplicatedCollisionActor::AReplicatedCollisionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(200.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = CollisionComponent;
    if (HasAuthority())
    {
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AReplicatedCollisionActor::OnOverlapBegin);
        CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AReplicatedCollisionActor::OnOverlapEnd);
    }
}

// Called when the game starts or when spawned
void AReplicatedCollisionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReplicatedCollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReplicatedCollisionActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;

    if (OverlappedActors.Contains(OtherActor)) return;
    OverlappedActors.Add(OtherActor);

    OnActorOverlapBeginWithAuth(OtherActor);
}

void AReplicatedCollisionActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!HasAuthority()) return;

    if (!OverlappedActors.Contains(OtherActor)) return;
    OverlappedActors.Remove(OverlappedActors.FindId(OtherActor));

    OnActorOverlapEndWithAuth(OtherActor);
}

void AReplicatedCollisionActor::OnActorOverlapBeginWithAuth(AActor* OtherActor)
{

}

void AReplicatedCollisionActor::OnActorOverlapEndWithAuth(AActor* OtherActor)
{

}

