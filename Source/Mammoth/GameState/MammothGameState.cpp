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

