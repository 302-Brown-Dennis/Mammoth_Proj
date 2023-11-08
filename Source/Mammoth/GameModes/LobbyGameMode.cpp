// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{

	Super::PostLogin(NewPlayer);

	AGameStateBase* CurrentGameState = GetGameState<AGameStateBase>();
	if (CurrentGameState)
	{
		int32 NumberOfPlayers = CurrentGameState->PlayerArray.Num();
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers));
		}

		APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
		if (PlayerState)
		{
			FString PlayerName = PlayerState->GetPlayerName();
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("%s joined!"), *PlayerName));

		}


	}


	// If statement here to check if all players have accepted a mission start, then travel to mission level
	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	bUseSeamlessTravel = true;
	//	World->ServerTravel(FString("/Game/Levels/Level_01?listen"));
	//}

}
