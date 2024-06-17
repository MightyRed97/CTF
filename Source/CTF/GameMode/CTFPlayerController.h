// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "CTFEnum.h"

#include "CTFPlayerController.generated.h"

/**
 * 
 */

class UPlayerHUD;

UCLASS()
class CTF_API ACTFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACTFPlayerController();

protected:
	virtual void BeginPlay() override;


public:
    UFUNCTION(BlueprintCallable, Server, Reliable, Category = Team)
    void SelectTeamID(ETeamID TeamID);

    UFUNCTION(BlueprintCallable, Client, Reliable, Category = Team)
    void UpdateTeamScore(ETeamID TeamID, int32 Score);

    UFUNCTION(BlueprintCallable, Client, Reliable, Category = Team)
    void UpdateTeamKill(ETeamID TeamID, int32 Kill);

    UFUNCTION(BlueprintCallable, Client, Reliable, Category = Time)
    void MatchEnded(EMatchResult MatchResult);

    UFUNCTION(BlueprintCallable, Category = UI)
    void ToggleInputMode(bool bIsUIMode);

    UFUNCTION(BlueprintCallable, Category = Character)
    void OnCharacterDeath();

protected:
    


public:
    // The widget class to create
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
    TSubclassOf<UPlayerHUD> PlayerHUDClass;

private:
    // The instance of the widget
    UPROPERTY()
    UPlayerHUD* PlayerHUDWidgetInstance;
};
