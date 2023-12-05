// Fill out your copyright notice in the Description page of Project Settings.


#include "OverHeadWidget.h"
#include "Components/TextBlock.h"

void UOverHeadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverHeadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	if (bShowRoles)
	{
		ENetRole CurrentRole;
		if (bShowRemoteOrLocalRole) // True, Show Remote role
		{
			UE_LOG(LogTemp, Warning, TEXT("True checked! showing RemoteRole"));
			CurrentRole = InPawn->GetRemoteRole();
		}
		else // Else false, Shoe Local role
		{
			UE_LOG(LogTemp, Warning, TEXT("False checked! showing LocalRole"));
			CurrentRole = InPawn->GetLocalRole();
		}

		FString Role;
		switch (CurrentRole)
		{
		case ENetRole::ROLE_Authority:
			Role = FString("Authority");
			break;
		case ENetRole::ROLE_AutonomousProxy:
			Role = FString("Autonomous Proxy");
			break;
		case ENetRole::ROLE_SimulatedProxy:
			Role = FString("Simulated Proxy");
			break;
		case ENetRole::ROLE_None:
			Role = FString("None");
			break;

		}

		FString CurrentRoleString;
		if (bShowRemoteOrLocalRole) // True, Show Remote role
		{
			CurrentRoleString = FString::Printf(TEXT("Remote Role is: %s"), *Role);
		}
		else // Else false, Shoe Local role
		{
			CurrentRoleString = FString::Printf(TEXT("Local Role is: %s"), *Role);
		}

		SetDisplayText(CurrentRoleString);
	}
	else
	{
		DisplayText->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UOverHeadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}
