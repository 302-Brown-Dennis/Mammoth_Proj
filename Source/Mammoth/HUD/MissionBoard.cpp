// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionBoard.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/Button.h"
#include "Mammoth/PlayerState/MammothPlayerState.h"
#include "Components/CheckBox.h"

void UMissionBoard::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	if (CloseButton && !CloseButton->OnClicked.IsBound())
	{
		CloseButton->OnClicked.AddDynamic(this, &UMissionBoard::CloseButtonClicked);
	}
	if (StartMissionButton && !StartMissionButton->OnClicked.IsBound())
	{
		StartMissionButton->OnClicked.AddDynamic(this, &UMissionBoard::StartMissionButtonClicked);
	}
}
bool UMissionBoard::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	ReadyBox1->SetVisibility(ESlateVisibility::Hidden);
	ReadyBox2->SetVisibility(ESlateVisibility::Hidden);
	ReadyBox3->SetVisibility(ESlateVisibility::Hidden);
	ReadyBox4->SetVisibility(ESlateVisibility::Hidden);
	return true;
}
void UMissionBoard::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);

		}
	}
	if (CloseButton && CloseButton->OnClicked.IsBound())
	{
		CloseButton->OnClicked.RemoveDynamic(this, &UMissionBoard::CloseButtonClicked);
	}
	if (StartMissionButton && StartMissionButton->OnClicked.IsBound())
	{
		StartMissionButton->OnClicked.RemoveDynamic(this, &UMissionBoard::StartMissionButtonClicked);
	}
}



void UMissionBoard::CloseButtonClicked()
{
	MenuTearDown();
}

void UMissionBoard::StartMissionButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found Player Controller!!"));
			AMammothPlayerState* MammothPlayerState = Cast<AMammothPlayerState>(PlayerController->PlayerState);
			if (MammothPlayerState)
			{
				ReadyBox1->SetVisibility(ESlateVisibility::Visible);
				ReadyBox2->SetVisibility(ESlateVisibility::Visible);
				ReadyBox3->SetVisibility(ESlateVisibility::Visible);
				ReadyBox4->SetVisibility(ESlateVisibility::Visible);

				ReadyBox1->SetIsChecked(true);

				//UE_LOG(LogTemp, Warning, TEXT("Found player state!!!"));
				// Set Player ready up value to true
				MammothPlayerState->SetPlayerIsReady();
			}
			
			
		}
	}
	
	bStartMissionWasClicked = true;
	StartMissionButton->SetIsEnabled(false);
	
}