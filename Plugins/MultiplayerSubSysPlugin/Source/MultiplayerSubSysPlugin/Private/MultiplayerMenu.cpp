// Based on Stephen Ulibarri Udemy course https://www.udemy.com/course/unreal-engine-5-cpp-multiplayer-shooter/
// Modified by: Dennis Brown

#include "MultiplayerMenu.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

DEFINE_LOG_CATEGORY(LogMultiplayerSystem);

// Calls the MenuSetup blueprintableCallable function, saves path to hub/lobby, max num of public connections, match type
// Gets player controller, displays menu set input mode to UI only!
// Gets game instance checks if valid, then gets our multiplayer sub-system from game instance
// If multiplayer sub-system was found bind callback funcs in this class to delegates in MultiplayerSessionSubsystem class
void UMultiplayerMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeMatch;

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			// Input only affects UI
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			// Do not lock mouse to viewport
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
		else 
		{
			UE_LOG(LogMultiplayerSystem, Error, TEXT("Failed to get first player controller in Multiplayer Menu."));
		}

	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}

	if (MultiplayerSessionSubsystem)
	{
		// Bind callback functions in ThisClass to delegates in MultiplayerSessionSubsystem class
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}

}

// Check if menu was initialized, binds HostButton and JoinButton functions to buttons dynamic delegate
// NOTE: buttons dynamic deleget is encapsulated in button.h
bool UMultiplayerMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (HostButton)
	{
		// Bind HostButtonClicked function to buttons dynamic delegate
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}
	if (JoinButton)
	{
		// // Bind JoinButtonClicked function to buttons dynamic delegate
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	return true;
}

// Bind menu widget host button to create session function
void UMultiplayerMenu::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
	if (MultiplayerSessionSubsystem)
	{
		// Create session params are harded coded could change later, add varibles in menu.h
		MultiplayerSessionSubsystem->CreateSession(NumPublicConnections, MatchType);

	}
}

// Bind menu widget join button to find session function in MultiplayerSessionSubsystem.cpp
void UMultiplayerMenu::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->FindSessions(1000);
	}
}


// On Create Session call back function server travel to lobby lvl, display debug text
void UMultiplayerMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("Session successfully created!")));
		}*/
		UE_LOG(LogMultiplayerSystem, Warning, TEXT("Session successfully created in Multiplayer Menu."));

		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Failed to create session!")));
		}*/
		UE_LOG(LogMultiplayerSystem, Error, TEXT("Failed to create session in Multiplayer Menu."));
		HostButton->SetIsEnabled(true);
	}
}

// parse results, might have to change based on design i.e. server list?
void UMultiplayerMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{

	if (MultiplayerSessionSubsystem == nullptr)
	{
		return;
	}

	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType)
		{
			MultiplayerSessionSubsystem->JoinSession(Result);
			return;
		}
	}
	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		JoinButton->SetIsEnabled(true);
	}

}

// Join open session via ip address and travel client to session
void UMultiplayerMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}


	}
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		JoinButton->SetIsEnabled(true);
	}

}

// Currently unused
void UMultiplayerMenu::OnDestroySession(bool bWasSuccessful)
{
}

// Currently unused
void UMultiplayerMenu::OnStartSession(bool bWasSuccessful)
{
}


// Remove menu viaMenuTear and give input back to all players
void UMultiplayerMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();
	// Use super for inherited functions
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

// Give back player control after menu closes
void UMultiplayerMenu::MenuTearDown()
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
}
