// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CTFPlayerState.h"

#include "Net/UnrealNetwork.h"

ACTFPlayerState::ACTFPlayerState()
{
	TeamID = ETeamID::NoTeam;

	SetReplicates(true);
}

void ACTFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACTFPlayerState, TeamID);
	DOREPLIFETIME(ACTFPlayerState, MatchTimeRemainingSeconds);
}

void ACTFPlayerState::SetTeamID(ETeamID ID)
{
	if (HasAuthority())
	{
		TeamID = ID;
	}
}

void ACTFPlayerState::SetMatchTimeRemaining(int32 TimeRemaining)
{
	if (HasAuthority())
	{
		MatchTimeRemainingSeconds = TimeRemaining;
	}
}
