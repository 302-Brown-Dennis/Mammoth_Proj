// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "GameFramework/PlayerController.h"
ALobbyGameMode::ALobbyGameMode()
{
	AllPlayersReadyDelegate.AddDynamic(this, &ALobbyGameMode::CallServerTravel);
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerInputStatus.Init(false, 4);

	// Bind the input check function to player controllers
	

}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{

	Super::PostLogin(NewPlayer);

	AGameStateBase* CurrentGameState = GetGameState<AGameStateBase>();
	if (CurrentGameState)
	{
		int32 NumberOfPlayers = CurrentGameState->PlayerArray.Num();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers));
		}

		APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
		if (PlayerState)
		{
			FString PlayerName = PlayerState->GetPlayerName();
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("%s joined!"), *PlayerName));
			}                                    
		}

	}
}

void ALobbyGameMode::CheckAllPlayersInput()
{
	NumPlayersReady++;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString(TEXT("Player has readied! successful broadcast!")));
	}
	// Check if all players are ready
	if (NumPlayersReady == 1)
	{
		AllPlayersReadyDelegate.Broadcast();
	}
}

void ALobbyGameMode::CallServerTravel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel(FString("/Game/Levels/Level_01?listen"));
	}
}




