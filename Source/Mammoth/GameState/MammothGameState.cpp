// Game state class
// Author: Dennis Brown


#include "MammothGameState.h"
#include "Net/UnrealNetwork.h"
#include "Mammoth/PlayerState/MammothPlayerState.h"

AMammothGameState::AMammothGameState()
{
	bReplicates = true;
}

void AMammothGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMammothGameState, MissionReadyPlayers);
}

TArray<AMammothPlayerState*> AMammothGameState::GetPlayerArray() const
{
    TArray<AMammothPlayerState*> CustomPlayerArray;

    for (APlayerState* PlayerState : PlayerArray)
    {
        AMammothPlayerState* MammothPlayerState = Cast<AMammothPlayerState>(PlayerState);
        if (MammothPlayerState)
        {
            //UE_LOG(LogTemp, Warning, TEXT("Found player state!!!!!!!"));

            CustomPlayerArray.Add(MammothPlayerState);
        }
    }

    return CustomPlayerArray;
}

int32 AMammothGameState::GetNumPlayers() const
{
	return PlayerArray.Num();
}

void AMammothGameState::GetRead()
{
    for (AMammothPlayerState* PlayerState : MissionReadyPlayers)
    {
        if (PlayerState)
        {
            // Check the desired variable in each player state
            bool bIsMissionReady = PlayerState->GetPlayerIsReady(); // Replace GetIsMissionReady() with the actual function or variable you want to check

            // Now you can use bIsMissionReady as needed
            if (bIsMissionReady)
            {
                NumOfPlayersReady++;
                // Do something when the player is mission ready
                UE_LOG(LogTemp, Warning, TEXT("Player %s is mission ready!"), *PlayerState->GetPlayerName());
                UE_LOG(LogTemp, Warning, TEXT("NUM PLAYERS READY: %d!"), NumOfPlayersReady);
            }
        }
    }
}
