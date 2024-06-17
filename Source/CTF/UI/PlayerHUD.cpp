// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "Components/TextBlock.h"

#include "GameMode/CTFPlayerController.h"
#include "GameMode/CTFPlayerState.h"
#include "Character/BaseCharacter.h"
#include "Character/CharInfoComponent.h"

void UPlayerHUD::UpdateTeamScore(ETeamID TeamID, int32 Score)
{
    FName TeamScoreTextBlockName;
    switch (TeamID)
    {
    case TeamA:
        TeamScoreTextBlockName = TEXT("Text_TeamA_Score");
        break;
    case TeamB:
        TeamScoreTextBlockName = TEXT("Text_TeamB_Score");
        break;
    default:
        return;
        break;
    }

    if (UTextBlock* TeamScoreTextBlock = Cast<UTextBlock>(GetWidgetFromName(TeamScoreTextBlockName)))
    {
        TeamScoreTextBlock->SetText(FText::FromString(FString::FromInt(Score)));
    }
}

void UPlayerHUD::UpdateTeamKill(ETeamID TeamID, int32 Kill)
{
    FName TeamKillTextBlockName;
    switch (TeamID)
    {
    case TeamA:
        TeamKillTextBlockName = TEXT("Text_TeamA_Kill");
        break;
    case TeamB:
        TeamKillTextBlockName = TEXT("Text_TeamB_Kill");
        break;
    default:
        return;
        break;
    }

    if (UTextBlock* TeamKillTextBlock = Cast<UTextBlock>(GetWidgetFromName(TeamKillTextBlockName)))
    {
        TeamKillTextBlock->SetText(FText::FromString(FString::FromInt(Kill)));
    }
}

void UPlayerHUD::ShowMatchResult(EMatchResult MatchResult)
{
    FName MatchResultViewName;
    switch (MatchResult)
    {
    case Draw:
    default:
        MatchResultViewName = TEXT("DrawScreen");
        break;
    case Win:
        MatchResultViewName = TEXT("WinScreen");
        break;
    case Lose:
        MatchResultViewName = TEXT("LoseScreen");
        break;
    }

    if (UWidget* ResultWidget = GetWidgetFromName(MatchResultViewName))
    {
        ResultWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

float UPlayerHUD::GetHealthPercent() const
{
    if (ACTFPlayerController* CTFPlayerController = Cast<ACTFPlayerController>(GetOwningPlayer()))
    {
        if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(CTFPlayerController->GetPawn()))
        {
            if (BaseCharacter->CharInfoComponent)
            {
                return BaseCharacter->CharInfoComponent->GetCurrentHealth() / BaseCharacter->CharInfoComponent->GetMaxHealth();
            }
        }
    }

    return 0.0f;
}

FText UPlayerHUD::GetRemainingTimeText() const
{
    FString TimeString = TEXT("00:00");
    
    if (ACTFPlayerController* CTFPlayerController = Cast<ACTFPlayerController>(GetOwningPlayer()))
    {
        if (ACTFPlayerState* CTFPlayerState = Cast<ACTFPlayerState>(CTFPlayerController->PlayerState))
        {
            int32 RemainingTime = CTFPlayerState->GetMatchTimeRemaining();
            TimeString = FString::Printf(TEXT("%02d:%02d"), RemainingTime / 60, RemainingTime % 60);
        }
    }

    return FText::FromString(TimeString);
}
