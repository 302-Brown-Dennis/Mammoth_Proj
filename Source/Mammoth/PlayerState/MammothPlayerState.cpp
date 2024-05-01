// Each players state
// Author: Dennis Brown


#include "MammothPlayerState.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "Net/UnrealNetwork.h"

AMammothPlayerState::AMammothPlayerState()
{
	bReplicates = true;
}

float AMammothPlayerState::GetPlayerHealth() 
{
	APlayerCharacter_cpp* PlayerCharacter = Cast <APlayerCharacter_cpp>(GetPawn());
	if (!PlayerCharacter)
	{
		//UE_LOG(LogTemp, Warning, TEXT("failed to get character"));
		// PlayerPawn is invalid
		return 0.0f;
	}


	// Retrieve health
	float Health = PlayerCharacter->GetHealth();
	return Health;
}