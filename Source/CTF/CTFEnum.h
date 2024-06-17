// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CTFEnum.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum ETeamID
{
	NoTeam = 0,
	TeamA = 1,
	TeamB = 2
};

UENUM(BlueprintType)
enum EMatchResult
{
	Draw = 0,
	Win = 1,
	Lose = 2
};
