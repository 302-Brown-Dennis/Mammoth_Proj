// Class for handling starting missions
// Author: Dennis Brown


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
	
}



void UMissionBoard::CloseButtonClicked()
{
	MenuTearDown();
}

