// Default level game mode
// Author: Dennis Brown


#include "LevelsGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "GameFramework/PlayerController.h"
#include "Mammoth/PlayerController/MammothPlayerController.h"
#include "Net/UnrealNetwork.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ALevelsGameMode::ALevelsGameMode()
{
	PlayerControllerClass = AMammothPlayerController::StaticClass();
	bDelayedStart = true;
}
void ALevelsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

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
		CountdownTime = WarmUpTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		//CountdownTime = LevelStartingTime - GetWorld()->GetTimeSeconds();
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
		AMammothPlayerController* MammothPlayerController = Cast<AMammothPlayerController>(*It);
		if (MammothPlayerController)
		{
			MammothPlayerController->OnMatchStateSet(MatchState);
		}
	}
	
}
void ALevelsGameMode::Server_GetServerTime_Implementation()
{
	FDateTime ServerTime = FDateTime::Now();
}

void ALevelsGameMode::LobbyServerTravel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel(FString("/Game/Levels/") + LobbyName + ("?listen"));
	}
}


