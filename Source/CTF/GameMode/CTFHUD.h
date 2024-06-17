// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CTFHUD.generated.h"

/**
 * 
 */
UCLASS()
class CTF_API ACTFHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACTFHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
};
