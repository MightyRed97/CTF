// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CTFGameState.h"

DEFINE_LOG_CATEGORY(LogCTFGameState);

ACTFGameState::ACTFGameState()
{
	MatchDurationSeconds = 30;
}

int32 ACTFGameState::GetTeamScore(ETeamID TeamID) const
{
	if (!TeamScores.Contains(TeamID)) return 0;

	return TeamScores[TeamID];
}

int32 ACTFGameState::GetTeamKill(ETeamID TeamID) const
{
	if (!TeamKills.Contains(TeamID)) return 0;

	return TeamKills[TeamID];
}

int32 ACTFGameState::AddTeamScore(ETeamID TeamID)
{
	if (!TeamScores.Contains(TeamID))
		TeamScores.Add(TeamID, 0);

	return ++TeamScores[TeamID];

	/*UE_LOG(LogCTFGameState, Log, TEXT("%d"), TeamScores[TeamID]);*/
}

int32 ACTFGameState::AddTeamKill(ETeamID TeamID)
{
	if (!TeamKills.Contains(TeamID))
		TeamKills.Add(TeamID, 0);

	return ++TeamKills[TeamID];
}

ETeamID ACTFGameState::GetTeamWon() const
{
	int32 TeamAScore = GetTeamScore(ETeamID::TeamA);
	int32 TeamBScore = GetTeamScore(ETeamID::TeamB);

	if (TeamAScore > TeamBScore)
	{
		return ETeamID::TeamA;
	}
	else if (TeamAScore < TeamBScore)
	{
		return ETeamID::TeamB;
	}

	int32 TeamAKill = GetTeamKill(ETeamID::TeamA);
	int32 TeamBKill = GetTeamKill(ETeamID::TeamB);

	if (TeamAKill > TeamBKill)
	{
		return ETeamID::TeamA;
	}
	else if (TeamAKill < TeamBKill)
	{
		return ETeamID::TeamB;
	}
	else
	{
		return ETeamID::NoTeam;
	}
}
