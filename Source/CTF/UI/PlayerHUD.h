// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "CTFEnum.h"

#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class CTF_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Team | Score")
	void UpdateTeamScore(ETeamID TeamID, int32 Score);

	UFUNCTION(BlueprintCallable, Category = "Team | Kill")
	void UpdateTeamKill(ETeamID TeamID, int32 Kill);

	UFUNCTION(BlueprintCallable, Category = "Match | Result")
	void ShowMatchResult(EMatchResult MatchResult);

	UFUNCTION(BlueprintCallable, Category = Health)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "Match | Time")
	FText GetRemainingTimeText() const;
};
