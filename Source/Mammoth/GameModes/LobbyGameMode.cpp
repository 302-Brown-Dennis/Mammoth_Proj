// Based on Stephen Ulibarri Udemy course https://www.udemy.com/course/unreal-engine-5-cpp-multiplayer-shooter/
// Modified by: Dennis Brown

#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "GameFramework/PlayerController.h"
#include "Mammoth/GameState/MammothGameState.h"
#include "Mammoth/PlayerState/MammothPlayerState.h"
#include "MultiplayerSessionSubsystem.h"
#include "Net/UnrealNetwork.h"

ALobbyGameMode::ALobbyGameMode()
{

}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{

	Super::PostLogin(NewPlayer);

	class AMammothGameState* MammothGameState = GetWorld()->GetGameState<AMammothGameState>();

	if (MammothGameState)
	{
		NumOfPlayers = MammothGameState->GetNumPlayers();
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UMultiplayerSessionSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
		check(Subsystem);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Players in game: %d"), NumOfPlayers));
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

// Server uses this to check all player ready state
void ALobbyGameMode::CheckPlayersReady()
{
	AMammothGameState* MammothGameState = GetGameState<AMammothGameState>();

	for (APlayerState* PlayerState : MammothGameState->GetPlayerArray())
	{
		AMammothPlayerState* MammothGamePlayerState = Cast<AMammothPlayerState>(PlayerState);
		if (MammothGamePlayerState )
		{
			//UE_LOG(LogTemp, Warning, TEXT("IN LOBBY GAME MODE CHECKING READIES!!!: %s"), MammothGamePlayerState->GetPlayerIsReady() ? TEXT("true") : TEXT("false"));
			if (!MammothGamePlayerState->GetPlayerIsReady())
			{
				//UE_LOG(LogTemp, Warning, TEXT("FAILED TO GET PLAYER READY!!!"));
				return;
				
			}
		
			//UE_LOG(LogTemp, Warning, TEXT("Player %s is mission ready!"), *MammothGamePlayerState->GetPlayerName());
			//UE_LOG(LogTemp, Warning, TEXT("My Boolean Value: %s"), MammothGamePlayerState->GetPlayerIsReady() ? TEXT("true") : TEXT("false"));
		}

	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("All players ready! Starting mission")));
	}
	CallServerTravel();
	
	
	
}

void ALobbyGameMode::CallServerTravel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		//bUseSeamlessTravel = true;
		//UE_LOG(LogTemp, Warning, TEXT("Level Name: %s"), *LevelName);
		//UE_LOG(LogTemp, Warning, TEXT("/Game/Levels/%s"), *LevelName, TEXT("?listen"));

		//World->ServerTravel(FString("/Game/Levels/Level_01?listen"));
		World->ServerTravel(FString("/Game/Levels/") + LevelName + ("?listen"));
	}
}

