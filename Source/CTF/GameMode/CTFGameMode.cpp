// Copyright Epic Games, Inc. All Rights Reserved.

#include "CTFGameMode.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerStart.h"

#include "GameMode/CTFPlayerState.h"
#include "GameMode/CTFGameState.h"
#include "GameMode/CTFPlayerController.h"
#include "Character/CTFCharacter.h"

DEFINE_LOG_CATEGORY(LogCTFGameMode);

ACTFGameMode::ACTFGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/CTF/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
        CTFCharacterClass = PlayerPawnBPClass.Class;
	}

    PlayerControllerClass = ACTFPlayerController::StaticClass();
	GameStateClass = ACTFGameState::StaticClass();
	PlayerStateClass = ACTFPlayerState::StaticClass();

    RespawnTimeSeconds = 4;
}

void ACTFGameMode::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
    for (AActor* Actor : Actors)
    {
        if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
        {
            if (PlayerStart->PlayerStartTag == TEXT("TeamA"))
            {
                TeamAPlayerStarts.Add(PlayerStart);
            }
            else if (PlayerStart->PlayerStartTag == TEXT("TeamB"))
            {
                TeamBPlayerStarts.Add(PlayerStart);
            }
        }
    }

    StartMatchTimer();
}

void ACTFGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

    if (ACTFPlayerController* CTFPlayerController = Cast<ACTFPlayerController>(NewPlayer))
    {
        UE_LOG(LogCTFGameMode, Warning, TEXT("New Player Logged In: %s"), *CTFPlayerController->GetName());

        PlayerControllers.Add(CTFPlayerController);

        if (ACTFPlayerState* CTFPlayerState = Cast<ACTFPlayerState>(CTFPlayerController->PlayerState))
        {
            PlayerStates.Add(CTFPlayerState);
        }

        DestroyPossessedPawn(CTFPlayerController);
    }
}

void ACTFGameMode::Logout(AController* Exiting)
{
    if (ACTFPlayerController* CTFPlayerController = Cast<ACTFPlayerController>(Exiting))
    {
        PlayerControllers.Remove(CTFPlayerController);

        if (ACTFPlayerState* CTFPlayerState = Cast<ACTFPlayerState>(CTFPlayerController->PlayerState))
        {
            PlayerStates.Remove(CTFPlayerState);
        }
    }
}

void ACTFGameMode::SpawnCharacterBySelectedTeam(ACTFPlayerController* CTFPlayerController, ETeamID TeamID)
{
    if (!CTFPlayerController) return;

    DestroyPossessedPawn(CTFPlayerController);

    FVector SpawnLocation = FVector::ZeroVector;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    TArray<APlayerStart*> TeamPlayerStarts;
    
    if (TeamID == TeamA)
    {
        TeamPlayerStarts = TeamAPlayerStarts;
    }
    else if (TeamID == TeamB)
    {
        TeamPlayerStarts = TeamBPlayerStarts;
    }

    if (TeamPlayerStarts.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, TeamPlayerStarts.Num() - 1);
        APlayerStart* ChosenStart = Cast<APlayerStart>(TeamPlayerStarts[RandomIndex]);

        if (ChosenStart)
        {
            SpawnLocation = ChosenStart->GetActorLocation();
            SpawnRotation = ChosenStart->GetActorRotation();

            ACTFCharacter* NewCharacter = GetWorld()->SpawnActor<ACTFCharacter>(CTFCharacterClass, SpawnLocation, SpawnRotation);
            if (NewCharacter)
            {
                // Possess the character with the player controller
                CTFPlayerController->Possess(NewCharacter);
            }
        }
    }
}

void ACTFGameMode::AddTeamScore(ETeamID TeamID)
{
    ACTFGameState* CTFGameState = Cast<ACTFGameState>(GameState);
    if (!CTFGameState) return;
	
    int32 Score = CTFGameState->AddTeamScore(TeamID);
	

    for (ACTFPlayerController* CTFPlayerController : PlayerControllers)
    {
        CTFPlayerController->UpdateTeamScore(TeamID, Score);
    }
}

void ACTFGameMode::AddTeamKill(ETeamID TeamID)
{
    ACTFGameState* CTFGameState = Cast<ACTFGameState>(GameState);
    if (!CTFGameState) return;

    int32 Kill = CTFGameState->AddTeamKill(TeamID);

    for (ACTFPlayerController* CTFPlayerController : PlayerControllers)
    {
        CTFPlayerController->UpdateTeamKill(TeamID, Kill);
    }
}

void ACTFGameMode::OnPlayerKilled(ACTFPlayerController* CTFPlayerController, ETeamID TeamID)
{
    ACTFGameState* CTFGameState = Cast<ACTFGameState>(GameState);
    if (!CTFGameState) return;

    ETeamID KillerTimeID;
    switch (TeamID)
    {
    case TeamA:
        KillerTimeID = ETeamID::TeamB;
        break;
    case TeamB:
        KillerTimeID = ETeamID::TeamA;
        break;
    case NoTeam:
    default:
        KillerTimeID = ETeamID::NoTeam;
        break;
    }
    
    AddTeamKill(KillerTimeID);

    FTimerDelegate RespawnTimerDel;
    RespawnTimerDel.BindUFunction(this, FName("SpawnCharacterBySelectedTeam"), CTFPlayerController, TeamID);
    FTimerHandle RespawnHandler;
    GetWorldTimerManager().SetTimer(RespawnHandler, RespawnTimerDel, (float) RespawnTimeSeconds, false);
}

void ACTFGameMode::StartMatchTimer()
{
    ACTFGameState* CTFGameState = Cast<ACTFGameState>(GameState);
    if (!CTFGameState) return;

    MatchTimeRemainingSeconds = CTFGameState->MatchDurationSeconds;

    GetWorldTimerManager().SetTimer(MatchTimerHandle, this, &ACTFGameMode::UpdateMatchTime, 1.0f, true);
}

void ACTFGameMode::UpdateMatchTime()
{
    MatchTimeRemainingSeconds--;

    for (ACTFPlayerState* CTFPlayerState : PlayerStates)
    {
        CTFPlayerState->SetMatchTimeRemaining(MatchTimeRemainingSeconds);
    }

    if (MatchTimeRemainingSeconds <= 0)
    {
        EndMatch();
    }
}

void ACTFGameMode::EndMatch()
{
    GetWorldTimerManager().ClearTimer(MatchTimerHandle);

    ACTFGameState* CTFGameState = Cast<ACTFGameState>(GameState);
    if (!CTFGameState) return;

    ETeamID TeamIDWon = CTFGameState->GetTeamWon();

    for (ACTFPlayerController* CTFPlayerController : PlayerControllers)
    {
        DestroyPossessedPawn(CTFPlayerController);
        
        if (ACTFPlayerState* CTFPlayerState = Cast<ACTFPlayerState>(CTFPlayerController->PlayerState))
        {
            ETeamID TeamID = CTFPlayerState->GetTeamID();
            EMatchResult MatchResult;
            
            if (TeamIDWon == ETeamID::NoTeam)
            {
                MatchResult = EMatchResult::Draw;
            }
            else if (TeamIDWon == TeamID)
            {
                MatchResult = EMatchResult::Win;
            }
            else
            {
                MatchResult = EMatchResult::Lose;
            }

            CTFPlayerController->MatchEnded(MatchResult);
        }
    }
}

void ACTFGameMode::DestroyPossessedPawn(APlayerController* PlayerController)
{
    if (APawn* Pawn = PlayerController->GetPawn())
    {
        PlayerController->UnPossess();
        Pawn->Destroy();
    }
}
