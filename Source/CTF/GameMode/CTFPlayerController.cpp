// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CTFPlayerController.h"

#include "UI/PlayerHUD.h"
#include "GameMode/CTFPlayerState.h"
#include "GameMode/CTFGameMode.h"


ACTFPlayerController::ACTFPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPlayerHUD> WidgetClass(TEXT("/Game/CTF/UMG/WB_PlayerHUD"));
	if (WidgetClass.Succeeded())
	{
		PlayerHUDClass = WidgetClass.Class;
	}
}

void ACTFPlayerController::BeginPlay()
{
	Super::BeginPlay();

    if (IsLocalPlayerController() && PlayerHUDClass)
    {
        PlayerHUDWidgetInstance = CreateWidget<UPlayerHUD>(this, PlayerHUDClass);
        if (PlayerHUDWidgetInstance)
        {
            PlayerHUDWidgetInstance->AddToViewport();
        }

        ToggleInputMode(true);
    }
}

void ACTFPlayerController::MatchEnded_Implementation(EMatchResult MatchResult)
{
    if (PlayerHUDWidgetInstance)
    {
        PlayerHUDWidgetInstance->ShowMatchResult(MatchResult);

        ToggleInputMode(true);
    }
}

void ACTFPlayerController::UpdateTeamScore_Implementation(ETeamID TeamID, int32 Score)
{
    if (PlayerHUDWidgetInstance)
    {
        PlayerHUDWidgetInstance->UpdateTeamScore(TeamID, Score);
    }
}

void ACTFPlayerController::UpdateTeamKill_Implementation(ETeamID TeamID, int32 Kill)
{
    if (PlayerHUDWidgetInstance)
    {
        PlayerHUDWidgetInstance->UpdateTeamKill(TeamID, Kill);
    }
}

void ACTFPlayerController::SelectTeamID_Implementation(ETeamID TeamID)
{
    if (ACTFPlayerState* CTFPlayerState = Cast<ACTFPlayerState>(PlayerState))
    {
        CTFPlayerState->SetTeamID(TeamID);
    }
    
    if (UWorld* World = GetWorld())
    {
        if (ACTFGameMode* CTFGameMode = Cast<ACTFGameMode>(World->GetAuthGameMode()))
        {
            CTFGameMode->SpawnCharacterBySelectedTeam(this, TeamID);
        }
    }
}

void ACTFPlayerController::ToggleInputMode(bool bIsUIMode)
{
    if (bIsUIMode)
    {
        SetInputMode(FInputModeUIOnly());
    }
    else
    {
        SetInputMode(FInputModeGameOnly());
    }
    
    SetShowMouseCursor(bIsUIMode);
}

void ACTFPlayerController::OnCharacterDeath()
{
    ACTFPlayerState* CTFPlayerState = Cast<ACTFPlayerState>(PlayerState);
    if (!CTFPlayerState) return;

    ETeamID TeamID = CTFPlayerState->GetTeamID();

    if (UWorld* World = GetWorld())
    {
        if (ACTFGameMode* CTFGameMode = Cast<ACTFGameMode>(World->GetAuthGameMode()))
        {
            CTFGameMode->OnPlayerKilled(this, TeamID);
        }
    }
}
