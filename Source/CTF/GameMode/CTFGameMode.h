// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CTFEnum.h"

#include "CTFGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCTFGameMode, Log, All);

class ACTFCharacter;
class ACTFPlayerController;
class ACTFPlayerState;
class APlayerStart;

UCLASS(minimalapi)
class ACTFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACTFGameMode();

protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;


public:
	UFUNCTION(BlueprintCallable, Category = "Team|Character")
	void SpawnCharacterBySelectedTeam(ACTFPlayerController* CTFPlayerController, ETeamID TeamID);

	UFUNCTION(BlueprintCallable, Category = "Team|Score")
	void AddTeamScore(ETeamID TeamID);

	UFUNCTION(BlueprintCallable, Category = "Team|Kill")
	void AddTeamKill(ETeamID TeamID);

	UFUNCTION(BlueprintCallable, Category = "Team|Kill")
	void OnPlayerKilled(ACTFPlayerController* CTFPlayerController, ETeamID TeamID);

protected:
	UFUNCTION(BlueprintCallable, Category = Time)
	void StartMatchTimer();

	UFUNCTION(BlueprintCallable, Category = Time)
	void UpdateMatchTime();

	UFUNCTION(BlueprintCallable, Category = Time)
	void EndMatch();


public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn|Character")
	TSubclassOf<ACTFCharacter> CTFCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn|Time")
	int32 RespawnTimeSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Player Start")
	TArray<APlayerStart*> TeamAPlayerStarts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Player Start")
	TArray<APlayerStart*> TeamBPlayerStarts;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	TArray<ACTFPlayerController*> PlayerControllers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	TArray<ACTFPlayerState*> PlayerStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
	int32 MatchTimeRemainingSeconds;

	FTimerHandle MatchTimerHandle;
};



