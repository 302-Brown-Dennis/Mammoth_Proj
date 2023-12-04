// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelsGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ALevelsGameMode::ALevelsGameMode()
{
	bDelayedStart = false;
}
void ALevelsGameMode::BeginPlay()
{
	Super::BeginPlay();
	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ALevelsGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
}
void ALevelsGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerCharacter_cpp* PlayerCharacter = Cast<APlayerCharacter_cpp>(*It);
		if (PlayerCharacter)
		{
			PlayerCharacter->OnMatchStateSet(MatchState);
		}
	}
	
}
void ALevelsGameMode::Server_GetServerTime_Implementation()
{
	FDateTime ServerTime = FDateTime::Now();
}


