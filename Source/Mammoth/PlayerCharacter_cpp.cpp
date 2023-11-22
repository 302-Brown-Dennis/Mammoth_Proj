// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter_cpp.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APlayerCharacter_cpp::APlayerCharacter_cpp():
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &APlayerCharacter_cpp::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &APlayerCharacter_cpp::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &APlayerCharacter_cpp::OnJoinSessionComplete))
	//AcceptDelegate(FPlayerAcceptanceDelegate::CreateUObject(this, &ThisClass::OnAccpetLevel))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//UE_LOG(LogTemp, Warning, TEXT("Character Constructor called!!"));
	
	// Accses our online sub system, Steam in this case
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	// Check if sub system is valid
	if (OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Found SubSystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
		}
	}

	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidget->SetupAttachment(RootComponent);

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

}

// Called when the game starts or when spawned
void APlayerCharacter_cpp::BeginPlay()
{
	Super::BeginPlay();

	//	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
	//PlayerStartMissionDelegate.AddDynamic(this, &APlayerCharacter_cpp::OnAccpetLevel);
	
	
	//PlayerAcceptanceDelegate.AddDynamic(this, &APlayerCharacter_cpp::OnAccpetLevel);
	//AcceptDelegate.BindUFunction(this, FName(On))

	//OnPlayerAccepted.AddDynamic(this, &APlayerCharacter_cpp::OnAccpetLevel);

	//UE_LOG(LogTemp, Warning, TEXT("Character begin play!"));
}

// Called every frame
void APlayerCharacter_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Character tick!"));
}

// Called to bind functionality to input
void APlayerCharacter_cpp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Open a Lobby by server travelling to the lobby/hub level
void APlayerCharacter_cpp::OpenLobby() {

	UWorld* World = GetWorld();
	if (World) {

		World->ServerTravel("/Game/Levels/Level_01?listen");
			//E:/Documents/Unreal Projects/Mammoth/Mammoth_Proj/Content/StarterContent/Maps/Hub_lvl.umap
	}

}

// Blueprint callable function for opening a level
void APlayerCharacter_cpp::CallOpenLevel(const FString& Address) {

	UGameplayStatics::OpenLevel(this, *Address);
}

// Blueprint callable function travelling a client to a session
void APlayerCharacter_cpp::CallClientTravel(const FString& Address) {

	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
}

void APlayerCharacter_cpp::PlayerHasReadyUp()
{
	
	PlayerReadyDelegate.Broadcast();
}


// Check if OnlineSessionInterface is vaild
// Check if a session already exsits, if so destroy session
// Store OnlineSessionInterface for later removal
// Set Session Settings of our new session and create a session w/settings
void APlayerCharacter_cpp::CreateGameSession()
{
	// Called on key press
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}

	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}

	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
	
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = false;
	SessionSettings->NumPublicConnections = 4;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->Set(FName("MatchType"), FString("TeamPlay"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}


// Check if OnlineSessionInterface is vaild
// Store OnlineSessionInterface for later removal
// Search for session
void APlayerCharacter_cpp::JoinGameSession()
{
	// Find a game session
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}

	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 1000;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);


	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

// If session created successfully travel to hub/level, display debug messages
void APlayerCharacter_cpp::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Created session: %s"), *SessionName.ToString()));
		}

		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel("/Game/Levels/Level_01?listen");
		}
	}
	else 
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Failed to crate a session")));
		}
	}
}

// Loop through search results
// Join Session with matching MatchType
void APlayerCharacter_cpp::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}
	for (auto Results : SessionSearch->SearchResults)
	{
		FString SessionId = Results.GetSessionIdStr();
		FString User = Results.Session.OwningUserName;
		FString MatchType;
		Results.Session.SessionSettings.Get(FName("MatchType"), MatchType);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Id: %s, User: %s"), *SessionId, *User));
		}

		if (MatchType == FString("TeamPlay"))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Joing Match of type: %s"), *MatchType));
			}

			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Results);

		}
	}
	
}

// Travel Client to found session, Debug message for server address
void APlayerCharacter_cpp::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}
	FString Address;
	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address)) 
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Connection String: %s"), *Address));
		}

		APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}

	}
}


