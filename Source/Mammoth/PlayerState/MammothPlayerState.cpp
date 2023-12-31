// Fill out your copyright notice in the Description page of Project Settings.


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