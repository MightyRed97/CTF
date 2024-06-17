// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "CTFEnum.h"

#include "CTFGameState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCTFGameState, Log, All);
/**
 * 
 */
UCLASS()
class CTF_API ACTFGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ACTFGameState();

	UFUNCTION(BlueprintPure, Category = "Team|Score")
	int32 GetTeamScore(ETeamID TeamID) const;

	UFUNCTION(BlueprintCallable, Category = "Team|Kill")
	int32 GetTeamKill(ETeamID TeamID) const;

	UFUNCTION(BlueprintCallable, Category = "Team|Score")
	int32 AddTeamScore(ETeamID TeamID);

	UFUNCTION(BlueprintCallable, Category = "Team|Kill")
	int32 AddTeamKill(ETeamID TeamID);

	UFUNCTION(BlueprintCallable, Category = "Team")
	ETeamID GetTeamWon() const;

protected:


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	int32 MatchDurationSeconds;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team|Score")
	TMap<TEnumAsByte<ETeamID>, int32> TeamScores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team|Kill")
	TMap<TEnumAsByte<ETeamID>, int32> TeamKills;
};
