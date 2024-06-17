// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Character/CharInfoComponent.h"
#include "GameMode/CTFPlayerController.h"
#include "GameMode/CTFPlayerState.h"
#include "GameProp/CTFProjectile.h"
#include "GameProp/FlagActor.h"
#include "GameProp/DroppedFlagActor.h"
#include "GameProp/TeamBaseZone.h"

#define COLLISION_WEAPON		ECC_GameTraceChannel1

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    bAlwaysRelevant = true;

    CharInfoComponent = CreateDefaultSubobject<UCharInfoComponent>(TEXT("CharInfoComponent"));
    CharInfoComponent->OnHoldingFlag.AddDynamic(this, &ABaseCharacter::OnHoldingFlagChanged);
    CharInfoComponent->OnHealthChanged.AddDynamic(this, &ABaseCharacter::OnHealthChanged);
    CharInfoComponent->OnDeath.AddDynamic(this, &ABaseCharacter::OnDeath);

	FireRate = 0.25f;
	bCanFire = true;

	ProjectileClass = ACTFProjectile::StaticClass();
    DroppedFlagActorClass = ADroppedFlagActor::StaticClass();
}

float ABaseCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    return CharInfoComponent->TakeDamage(DamageTaken);
}

ETeamID ABaseCharacter::GetTeamID() const
{
    if (ACTFPlayerState* CTFPlayerState = Cast<ACTFPlayerState>(GetPlayerState())) return CTFPlayerState->GetTeamID();

    return ETeamID::NoTeam;
}

bool ABaseCharacter::IsDead() const
{
    if (CharInfoComponent)
    {
        return CharInfoComponent->IsDead();
    }
    
    return true;
}

void ABaseCharacter::TakeFlag(ETeamID TeamID)
{
    if (!CharInfoComponent->IsHoldingFlag()) CharInfoComponent->SetHoldingFlag(true, TeamID);
}

void ABaseCharacter::DropFlagInBaseZone()
{
    if (CharInfoComponent->IsHoldingFlag()) CharInfoComponent->SetHoldingFlag(false, ETeamID::NoTeam);
}

void ABaseCharacter::OnHealthChanged(float CurrentHealth)
{
    if (IsLocallyControlled())
    {
        FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

        if (CurrentHealth <= 0)
        {
            FString deathMessage = FString::Printf(TEXT("You have been killed."));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
        }
    }

    if (HasAuthority())
    {
        FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, healthMessage);
    }
}

void ABaseCharacter::OnDeath()
{
    ACTFPlayerController* CTFPlayerController = Cast<ACTFPlayerController>(GetController());
    CTFPlayerController->OnCharacterDeath();
    
    GetCharacterMovement()->DisableMovement();

    if (UCapsuleComponent* CapComp = GetCapsuleComponent())
        CapComp->SetCollisionProfileName(TEXT("NoCollision"));

    if (USkeletalMeshComponent* SkelMesh = GetMesh())
    {
        // Enable physics simulation on the skeletal mesh
        SkelMesh->SetSimulatePhysics(true);

        // Set collision profile to ragdoll to handle physics interactions properly
        SkelMesh->SetCollisionProfileName(TEXT("Ragdoll"));

        // Optionally, apply an impulse to the character for immediate effect
        FVector Impulse = FVector(0, 0, 1000.0f); // Example impulse
        SkelMesh->AddImpulse(Impulse, NAME_None, true);
    }

    if (HasAuthority() && CharInfoComponent->IsHoldingFlag())
    {
        SpawnDroppedFlag();
    }
}

void ABaseCharacter::SpawnDroppedFlag()
{
    if (DroppedFlagActorClass)
    {
        if (UWorld* World = GetWorld())
        {
            ADroppedFlagActor* SpawnDroppedFlagActor = GetWorld()->SpawnActorDeferred<ADroppedFlagActor>(DroppedFlagActorClass, FTransform::Identity);
            SpawnDroppedFlagActor->SetTeamID(CharInfoComponent->GetHoldingFlagTeamID());

            FTransform FlagTransform = GetActorTransform();
            FVector Location = FlagTransform.GetLocation();
            Location.Z = 0.0f;
            FlagTransform.SetLocation(Location);
            UGameplayStatics::FinishSpawningActor(SpawnDroppedFlagActor, FlagTransform);

            SpawnDroppedFlagActor->ApplyTeamMaterial();
        }
    }
}

void ABaseCharacter::Fire()
{
    if (!bCanFire || !ProjectileClass) return;

    if (UWorld* World = GetWorld())
    {
        bCanFire = false;
        GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ABaseCharacter::ResetCanFire, FireRate, false);
        HandleFire();
    }
}

void ABaseCharacter::ResetCanFire()
{
    bCanFire = true;
}

void ABaseCharacter::OnHoldingFlagChanged(bool bIsHoldingFlag)
{
    // GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BaseCharacter Holding Flag"));
}

void ABaseCharacter::HandleFire_Implementation()
{
    // Now send a trace from the end of our gun to see if we should hit anything
    APlayerController* PlayerController = Cast<APlayerController>(GetController());

    FVector ShootDir = FVector::ZeroVector;
    FVector StartTrace = FVector::ZeroVector;

    if (PlayerController)
    {
        // Calculate the direction of fire and the start location for trace
        FRotator CamRot;
        PlayerController->GetPlayerViewPoint(StartTrace, CamRot);
        ShootDir = CamRot.Vector();

        // Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
        StartTrace = StartTrace + ShootDir * ((GetActorLocation() - StartTrace) | ShootDir);
    }

    // Calculate endpoint of trace
    const FVector EndTrace = StartTrace + ShootDir * 10000.0f;

    // Perform trace to retrieve hit info
    FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
    TraceParams.bReturnPhysicalMaterial = true;

    FHitResult OutHit(ForceInit);
    GetWorld()->LineTraceSingleByChannel(OutHit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams);

    // Calculate spawn location and rotation of projectile
    FVector SpawnLocation = GetActorLocation() + (GetControlRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
    FRotator SpawnRotation = GetControlRotation();
    if (OutHit.bBlockingHit) SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, OutHit.Location);

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Instigator = GetInstigator();
    SpawnParameters.Owner = this;

    ACTFProjectile* spawnedProjectile = GetWorld()->SpawnActor<ACTFProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseCharacter, CharInfoComponent);
}