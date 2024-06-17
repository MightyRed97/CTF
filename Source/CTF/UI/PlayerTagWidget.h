// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerTagWidget.generated.h"

/**
 * 
 */
UCLASS()
class CTF_API UPlayerTagWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Player")
	void UpdateHoldingFlagStatus(bool bIsHolding);
};
