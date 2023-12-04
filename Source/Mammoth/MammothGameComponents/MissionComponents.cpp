// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionComponents.h"
#include "Mammoth/HUD/MissionBoard.h"

UMissionComponents::UMissionComponents()
{

	PrimaryComponentTick.bCanEverTick = false;

	
}

void UMissionComponents::ShowMissionBoardWidget()
{
	MissionBoard->MenuSetup();
}

void UMissionComponents::BeginPlay()
{
	Super::BeginPlay();

}

void UMissionComponents::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

