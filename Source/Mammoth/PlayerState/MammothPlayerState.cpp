// Each players state
// Author: Dennis Brown


#include "MammothPlayerState.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "Net/UnrealNetwork.h"

AMammothPlayerState::AMammothPlayerState()
{
	bReplicates = true;
}


void AMammothPlayerState::SetPlayerIsReady()
{
	if (HasAuthority())
	{
		bPlayerReady = true;
	}
	else
	{
		Server_SetPlayerRead(true);
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("My Boolean Value: %s"), bPlayerReady ? TEXT("true") : TEXT("false"));
	
	//UE_LOG(LogTemp, Warning, TEXT("IN PLAYER STATE"));

	// Changed APlayerCharacter_cpp* BCharacter = Cast <APlayerCharacter_cpp>(GetPawn());
	Character = Cast <APlayerCharacter_cpp>(GetPawn());
	if (Character)
	{
		
		
			//UE_LOG(LogTemp, Warning, TEXT("ON CLIENT CALLING SERVER FUNC"));
			Character->UpdatePlayerReady();
		
		//UE_LOG(LogTemp, Warning, TEXT("CALLING SET PLAYER READY!!"));
		//Character->Server_UpdatePlayersReady();
		Character->Server_SetPlayerIsReady();
		//Character->PlayerHasReadyUp();
	}

}

void AMammothPlayerState::Server_SetPlayerRead_Implementation(bool bNewReadyState)
{
	bPlayerReady = bNewReadyState;
}

bool AMammothPlayerState::GetPlayerIsReady() const
{
	return bPlayerReady;
}

float AMammothPlayerState::GetPlayerHealth() 
{
	APlayerCharacter_cpp* PlayerCharacter = Cast <APlayerCharacter_cpp>(GetPawn());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("failed to get character"));
		// PlayerPawn is invalid
		return 0.0f;
	}


	// Retrieve health
	float Health = PlayerCharacter->GetHealth();
	return Health;
}