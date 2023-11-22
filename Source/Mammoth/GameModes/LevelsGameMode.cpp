// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelsGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"

void ALevelsGameMode::Server_GetServerTime_Implementation()
{
	FDateTime ServerTime = FDateTime::Now();
}
