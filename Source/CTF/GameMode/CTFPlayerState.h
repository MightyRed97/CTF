// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "CTFEnum.h"

#include "CTFPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CTF_API ACTFPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACTFPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintPure, Category = Team)
	FORCEINLINE ETeamID GetTeamID() const { return TeamID; }

	UFUNCTION(BlueprintCallable, Category = Team)
	void SetTeamID(ETeamID ID);

	UFUNCTION(BlueprintCallable, Category = Time)
	void SetMatchTimeRemaining(int32 TimeRemaining);

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Match")
	int32 MatchTimeRemainingSeconds;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = Team)
	TEnumAsByte<ETeamID> TeamID;
};
